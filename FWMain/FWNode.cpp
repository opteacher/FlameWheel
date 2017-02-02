#pragma once
#include "stdafx.h"
#include "FWNode.h"
#include "FWSpace.h"
#include "FWFactory.h"
#include "FWMshIntf.h"
#include "FWLgtIntf.h"
#include "FWTxtIntf.h"
#include "FWManager.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
#include "../OPMath/Math.h"
using OPMath::CCoord;
using OPMath::CSphere;

INI_TYPID(CFWNode);

std::set<string> CFWNode::s_stUsedNams = std::set<string>();

CFWNode::CFWNode(string strNamI): ptd_dwObjID(0), ptd_tpAux(AUX_UNKNOWN)
{	
	if(strNamI.empty() || CHK_EXS_STL( s_stUsedNams, strNamI))
	{
		strNamI = "Node" + numToStr<DWORD>(s_stUsedNams.size());
	}
	ptd_strName = strNamI;
	s_stUsedNams.insert(ptd_strName);
}

HRESULT CFWNode::render( CMatrix matI, DWORD dwDispSetgI)
{
	//FUNC_BEG;
	// Check whether render this model
	if((dwDispSetgI&MAIN_OBJ) == 0)	{ return S_OK; }

	// Update self
	CFWMatrix matTsf;
	if(FAILED(update( matI, matTsf)))	{ return E_FAIL; }

	// Render self(If existed)
	if(FAILED(renderSelf(matTsf)))	{ return E_FAIL; }

	// Render auxiliary object
	if(ptd_tpAux == AUX_UNKNOWN)
	{
		if(FAILED(renderAux( matTsf, dwDispSetgI)))	{ return E_FAIL; }
	}

	// Render subset
	REVIEW_SUBSET_MAP	{ if(FAILED(iter->second->render( matTsf, dwDispSetgI)))	{ return E_FAIL; } }
	//FUNC_END;
	return S_OK;
}

HRESULT CFWNode::update( CMatrix matI, CMatrix& rMatO)
{
	// Generate transform matrix
	rMatO = (CFWMatrix&)ptd_spcLoc.GetWldTrfMat(matI);
	return S_OK;
}

HRESULT CFWNode::renderAux( CMatrix matI, DWORD dwDispSetgI)
{
	//FUNC_BEG;
	for( ItrAuxObj i = ptd_mpAuxObjs.begin(); i != ptd_mpAuxObjs.end(); ++i)
	{
		if((i->first&dwDispSetgI) != 0)
		{
			if(FAILED(i->second->render(matI)))	{ return E_FAIL; }
		}
	}
	//FUNC_END;
	return S_OK;
}

HRESULT CFWNode::preRender()
{
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();
	for( IterPam iter = ptd_mpEffect.begin(); iter != ptd_mpEffect.end(); ++iter)
	{
		CFWEffIntf& rEff = pEffMgr->GetEffect(iter->first);
		if(!rEff.Enable(iter->second))	{ return E_FAIL; }
	}
	return S_OK;
}

HRESULT CFWNode::sufRender()
{
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();
	for( IterPam iter = ptd_mpEffect.begin(); iter != ptd_mpEffect.end(); ++iter)
	{
		CFWEffIntf& rEff = pEffMgr->GetEffect(iter->first);
		if(iter->second.has("effNoChg"))
		{
			// Effect has not changed, don't need to active negative
			iter->second.rmv("effNoChg");	continue;
		}
		if(!rEff.Negative(iter->second))	{ return E_FAIL; }
	}
	return S_OK;
}

void CFWNode::resetSpc(DWORD dwSpcIdI)
{
	if(ptd_spcLoc.BeBound())	{ ptd_spcLoc.Release(); }
	ptd_spcLoc = *(CFWSpcIntf*)CFWSpcFcty::getInstance()->GetIntfByID(dwSpcIdI);
}

void CFWNode::setLocation(OPMath::CPoint posCenI)
{
	if(ptd_spcLoc.BeBound())	{ ptd_spcLoc.SetLocation(posCenI); }
}

void CFWNode::setCoordSys(OPMath::CCoord codLocI)
{
	if(ptd_spcLoc.BeBound())	{ ptd_spcLoc.SetCoordSys(codLocI); }
}

bool CFWNode::resetNam(string strNamI)
{
	if(CHK_EXS_STL( s_stUsedNams, strNamI))	{ return false; }
	s_stUsedNams.erase(ptd_strName);
	ptd_strName = strNamI;
	s_stUsedNams.insert(ptd_strName);
	return true;
}

string CFWNode::setIncObj(DWORD dwIdI)
{
	if(ptd_tpAux != 0)	{ return ""; }
	if(chkIsLgtAndAdd(dwIdI))	{ return ptd_strName; }
	if(chkIsMshAndAdd(dwIdI))	{ return ptd_strName; }
	if(chkIsCamAndAdd(dwIdI))	{ return ptd_strName; }
	return "";
}

void CFWNode::bindIncObj( DWORD dwIdI, string strObjTpI)
{
	ptd_dwObjID = dwIdI;
	if(ptd_strName.empty() || ptd_strName.substr( 0, 4) == "Node")
	{
		resetNam(strObjTpI + numToStr<DWORD>(s_stUsedNams.size()));
	}
}

bool CFWNode::chkIsLgtAndAdd(DWORD dwIdI)
{
	CFWLgtFcty* pLgtFcty = CFWLgtFcty::getInstance();
	CFWMdlMngr* pMdlMngr = CFWMdlMngr::getInstance();
	if(CFWLgtFcty::getInstance()->HasSuchObj(dwIdI))
	{
		bindIncObj( dwIdI, "Light");

		// Update node's position by light's location
		CFWLgtIntf* pLgt = (CFWLgtIntf*)pLgtFcty->GetIntfByID(dwIdI);
		// +_+: direction light haven't position, 
		// cant add world coordinate system and light display object
		if(pLgt->GetLgtType() != CFWLgtIntf::LGT_TP_POINT
		&& pLgt->GetLgtType() != CFWLgtIntf::LGT_TP_SPOT)	{ return true; }
		// Create a space object for light(+_+: not light model)
		CFWParam pamLgt( "spcPos", pLgt->GetLgtLoc().convertToStr());
		pamLgt.add( "mdlAuxTyp", numToStr<UINT>(CFWNode::AUX_LGT_LOC));
		ptd_spcLoc = (CFWSpcIntf&)CFWSpcFcty::getInstance()->Create(pamLgt);

		// Create light location object
		CFWModel* pLgtLoc = (CFWModel*)pMdlMngr->createLgtLoc(CFWParam( "mdlName", ptd_strName));
		CFWMshIntf mshLgt = pLgtLoc->GetMesh();
		SFWQuate sphOS = mshLgt.GeneOutSrdSph();
		ptd_mpAuxObjs.insert(VtpAuxObj( AUX_LGT_LOC, pLgtLoc));
		
		// Add world coordinate system
		genWldCoord(sphOS.m_fH);

		return true;
	}
	return false;
}

bool CFWNode::chkIsMshAndAdd(DWORD dwIdI)
{
	CFWMdlMngr* pMdlMgr = CFWMdlMngr::getInstance();
	if(CFWMshFcty::getInstance()->HasSuchObj(dwIdI))
	{
		bindIncObj( dwIdI, "Mesh");

		// Add world coordinate system
		genWldCoord();

		// Add sub grid model
		CFWParam pamGrid( "souNodeP", numToStr<DWORD>((DWORD)this));
		pamGrid.add( "mdlAuxTyp", numToStr<UINT>(AUX_GRID));
		CFWNode* pMdlGird = pMdlMgr->Copy(pamGrid);
		pMdlGird->setCoordSys(CCoord());
		string strMdlNam = ptd_strName;
		strMdlNam += CFWMdlMngr::s_cstrGrid;
		pMdlGird->resetNam(strMdlNam);
		pMdlGird->ptd_mpEffect.insert(VtpPam( CFWEffMngr::EFFECT_SHOW_EDGE, CFWParam()));
		ptd_mpAuxObjs.insert(VtpAuxObj( AUX_GRID, pMdlGird));

		// Add out-surround box, sphere
		// @_@ ..

		return true;
	}
	return false;
}

bool CFWNode::chkIsCamAndAdd(DWORD dwIdI)
{
	if(CFWCamFcty::getInstance()->HasSuchObj(dwIdI))
	{
		bindIncObj( dwIdI, "Camera");

		// @_@: Add FPS

		return true;
	}
	return false;
}

void CFWNode::genWldCoord(float fAxisI)
{
	if(ptd_tpAux == EFWAuxObj::AUX_COD_WLD)	{ return; }

	CFWParam pamWldCod( "mdlName", ptd_strName);
	if(fAxisI == 0)
	{
		// If identified mesh's out surround sphere can be generated,
		// take the radius as coordinate system's axis length
		CFWMshFcty* pFcty = CFWMshFcty::getInstance();
		if(pFcty->HasSuchObj(ptd_dwObjID))
		{
			CFWMshIntf* pMsh = (CFWMshIntf*)pFcty->GetIntfByID(ptd_dwObjID);
			SFWQuate sphOutSrd = pMsh->GeneOutSrdSph();
			if(sphOutSrd.m_fH != 0)
			{
				pamWldCod.add( "axisLen", numToStr<float>(sphOutSrd.m_fH));
			}
		}
	}
	CFWNode* pCod = CFWMdlMngr::getInstance()->createWldCod(pamWldCod);
	ptd_mpAuxObjs.insert(VtpAuxObj( AUX_COD_WLD, pCod));
}

void CFWNode::addSubNode( CFWNode* pNodeI )
{
	if(pNodeI == NULL)	{ return; }
	if(CHK_EXS_STL( ptd_mpSubset, pNodeI->ptd_strName))	{ return; }
	ptd_mpSubset.insert(VtpSub( pNodeI->ptd_strName, pNodeI));
}

bool CFWNode::rmvSubNode( string strNamI )
{
	ItrSub iterFnd = ptd_mpSubset.find(strNamI);
	if(iterFnd != ptd_mpSubset.end())
	{
		ptd_mpSubset.erase(iterFnd);	return true;
	}
	else
	{
		REVIEW_SUBSET_MAP	{ if(iter->second->rmvSubNode(strNamI))	{ return true; } }
	}
	return false;
}

bool CFWNode::rmvSubNode( CFWNode* pNodeI )
{
	REVIEW_SUBSET_MAP
	{
		if(iter->second == pNodeI)
		{
			ptd_mpSubset.erase(iter);	return true;
		}
		else
		{
			if(iter->second->rmvSubNode(pNodeI))	{ return true; }
		}
	}
	return false;
}

void CFWNode::clsSubNode()
{
	REVIEW_SUBSET_MAP	{ SAFE_DELETE(iter->second); }
	ptd_mpSubset.clear();
}

CFWNode* CFWNode::findNodeFromSub( string strNamI )
{
	// Find current node's subset
	ItrSub iterFnd = ptd_mpSubset.find(strNamI);
	if(iterFnd != ptd_mpSubset.end())
	{
		return iterFnd->second;
	}

	// Find current node's subsets' subset
	REVIEW_SUBSET_MAP
	{
		CFWNode* pNdFnd = iter->second->findNodeFromSub(strNamI);
		if(pNdFnd != NULL)	{ return pNdFnd; }
	}

	return NULL;
}

void CFWNode::AddEffect( UINT uEffIdI, CFWParam pamI)
{
	if(CFWEffMngr::getInstance()->ChkEffAva(uEffIdI))
	{
		ptd_mpEffect.insert(VtpPam( uEffIdI, pamI));
	}
}

CFWNode* CFWNode::Clone(CFWNode* pNodeI) const
{
	if(pNodeI == NULL)	{ pNodeI = new CFWNode(); }

	pNodeI->ptd_dwObjID = ptd_dwObjID;
	pNodeI->ptd_tpAux = ptd_tpAux;
	pNodeI->ptd_mpEffect = ptd_mpEffect;
	CFWParam pamSpc = ptd_spcLoc.GenCrteInfo();
	CFWSpcFcty* pSpcFcty = CFWSpcFcty::getInstance();
	pNodeI->ptd_spcLoc = (CFWSpcIntf&)pSpcFcty->Create(pamSpc);
	for( CiterSub i = ptd_mpSubset.begin(); i != ptd_mpSubset.end(); ++i)
	{
		CFWNode* pSub = i->second->Clone();
		pNodeI->ptd_mpSubset.insert(VtpSub( pSub->getNam(), pSub));
	}
	if(ptd_tpAux == 0)
	{
		pNodeI->setIncObj(pNodeI->ptd_dwObjID);
	}

	return pNodeI;
}
