#pragma once
#include "stdafx.h"
#include "FWManager.h"
using std::pair;
#include "FWFactory.h"
#include "FWExpParse.h"
#include "FWMisc.h"
#include "FWModel.h"
#include "FWTerrain.h"
#include "FWSkin.h"
using OPMath::CPoint;
using OPMath::CMatrix;
#include <fstream>
#include "../OPTools/String.h"
using OPTools::strToNum;

SINGLE_IMPLEMENT(CFWMdlMngr)

const string CFWMdlMngr::s_cstrWldCod("_WorldCoord");
const string CFWMdlMngr::s_cstrLgtLoc("_Location");
const string CFWMdlMngr::s_cstrGrid("_Grid");

std::set<string>::value_type ayUsedNam[] = 
{
	std::set<string>::value_type("#Model_Template_Light")
};
const std::set<string> CFWMdlMngr::s_cstrUsedNam( ayUsedNam, ayUsedNam + 1);

CFWNode* CFWMdlMngr::Create(CFWParam pamI)
{
	// If require copy, do copy
	if(pamI.has("souNode"))	{ return Copy(pamI); }

	if(pamI.has("mdlFile"))
	{
		CFWExpParse parseFWE;
		string strFile = pamI.getString( "mdlFile");
		CFWExpParse::PrNamPam pamInit = parseFWE.expMdlFile(strFile);
		
		CFWNode* pMdlNew = NULL;

		if(pamInit.second.size() == 1)
		{
			// Only one subset, create a model
			pMdlNew = new CFWModel();
			// Reset model's name
			if(pamI.has("mdlName"))			{ pMdlNew->resetNam(pamI.getString("mdlName")); }
			else if(!pamInit.first.empty())	{ pMdlNew->resetNam(pamInit.first); }

			// Create model required objects by parameter
			pamI.mrg(pamInit.second[0]);
			fillInMdlByPam( pamI, (CFWModel*)pMdlNew);
		}
		else
		{
			pMdlNew = new CFWNode();
			// Reset model's name
			if(pamI.has("mdlName"))			{ pMdlNew->resetNam(pamI.getString("mdlName")); }
			else if(!pamInit.first.empty())	{ pMdlNew->resetNam(pamInit.first); }

			// Create model required objects by parameter
			for( UINT i = 0; i < pamInit.second.size(); ++i)
			{
				CFWModel* pMdlSub = new CFWModel();
				CFWParam pamTmp = pamInit.second[i];
				pamTmp.mrg(pamI);
				fillInMdlByPam( pamTmp, pMdlSub);

				// Add sub into new node
				pMdlNew->addSubNode(pMdlSub);
			}
		}

		// Insert into nodes' map
		if(pMdlNew)
		{
			ptd_mpNodes.insert(VtpNode( pMdlNew->getNam(), pMdlNew));
		}
		return pMdlNew;
	}
	if(pamI.has("plnLength") && pamI.has("plnWidth")
	/***********************/
	&& pamI.has("hgtMapFile")
	&& pamI.has("hgtMapHgt") && pamI.has("hgtMapWid"))
	{
		return createTrnMdl(pamI);
	}
	// Check whether geometry model
	CFWParam pamCrteMdl = GetGeoCrtePam(pamI);
	if(!pamCrteMdl.isEmpty())
	{
		return createGeoMdl(pamCrteMdl);
	}

	return NULL;
}

CFWNode* CFWMdlMngr::Copy(CFWParam pamI)
{
	// Check whether model
	if(pamI.has("souNode"))
	{
		string strSou = pamI.getString("souNode");
		if(CHK_EXS_STL( ptd_mpNodes, strSou))
		{
			CFWNode* pSouNd = ptd_mpNodes[strSou];
			if(!pSouNd->chkIsClass(CFWModel::getTypID()))	{ return NULL; }
		}
	}
	CFWNode* pNewNd = __super::Copy(pamI);
	if(pNewNd == NULL)	{ return pNewNd; }
	// Reset name
	if(pamI.has("mdlName"))	{ pNewNd->resetNam(pamI.getString("mdlName")); }
	// Reset space object
	if(pamI.has("spcPos"))	{ pNewNd->setLocation(pamI.getQuate("spcPos")); }
	return pNewNd;
}

CFWNode* CFWMdlMngr::createWldCod(CFWParam pamI)
{
	CFWNode* pNdCod = new CFWNode();
	pNdCod->setAuxTyp(CFWNode::AUX_COD_WLD);
	// @_@: Drawable coordinate should has 0 Z-Depth(Write on the screen)
	pNdCod->AddEffect( CFWEffMngr::EFFECT_Z_DEPTH, CFWParam( "enable", "false"));
	if(pamI.has("mdlName"))
	{
		string strNam = pamI.getString("mdlName");
		strNam += s_cstrWldCod;
		pNdCod->resetNam(strNam);
	}
	string strCodNam = pNdCod->getNam();
	string strAxisLen = "50";
	if(pamI.has("axisLen"))	{ strAxisLen = pamI.getString("axisLen"); }
	if(pamI.has("spcID"))	{ pNdCod->resetSpc(pamI.getDword("spcID")); }
	else if(pamI.has("spcPos"))	{ pNdCod->setLocation(pamI.getQuate("spcPos")); }
	else if(pamI.has("spcLoc"))	{ pNdCod->setCoordSys(pamI.getCoordinate("spcLoc")); }

	CFWParam pamCrte( "mdlName", strCodNam + "_AxisX");
	pamCrte.add( "lnBegPoi", "X0Y0Z0H1");
	// +_+: World coordinate cant generate auxiliary object
	pamCrte.add( "mdlAuxTyp", numToStr<UINT>(CFWNode::AUX_COD_WLD));
	string strAxisX = "X" + strAxisLen + "Y0Z0H1";
	pamCrte.add( "lnEndPoi", strAxisX);
	pamCrte.add( "mtlDif", g_colRed.convertToStr());
	pNdCod->addSubNode(Create(pamCrte));

	pamCrte.upd( "mdlName", strCodNam + "_AxisY");
	string strAxisY = "X0Y" + strAxisLen + "Z0H1";
	pamCrte.upd( "lnEndPoi", strAxisY);
	pamCrte.upd( "mtlDif", g_colGreen.convertToStr());
	pNdCod->addSubNode(Create(pamCrte));

	pamCrte.upd( "mdlName", strCodNam + "_AxisZ");
	string strAxisZ = "X0Y0Z" + strAxisLen + "H1";
	pamCrte.upd( "lnEndPoi", strAxisZ);
	pamCrte.upd( "mtlDif", g_colBlue.convertToStr());
	pNdCod->addSubNode(Create(pamCrte));

	// Insert into nodes' map
	ptd_mpNodes.insert(VtpNode( strCodNam, pNdCod));

	return pNdCod;
}

CFWNode* CFWMdlMngr::createLgtLoc(CFWParam pamI)
{
	string strMdlTmpLgt = "#Model_Template_Light";
	if(!CHK_EXS_STL( ptd_mpNodes, strMdlTmpLgt))
	{
		CFWParam pamCrte( "mdlName", strMdlTmpLgt);
		pamCrte.add( "plnLength", "30");
		pamCrte.add( "plnWidth", "50");
		pamCrte.add( "mdlAuxTyp", numToStr<UINT>(CFWNode::AUX_LGT_LOC));
		pamCrte.add( "mtlDif", g_colWhite.convertToStr());
		pamCrte.add( "difTex", "Light.png");
		CFWNode* pLgtMod = Create(pamCrte);
		pLgtMod->AddEffect(CFWEffMngr::EFFECT_ADV_BOARD);
		pLgtMod->AddEffect(CFWEffMngr::EFFECT_ALPHA);
	}

	// Copy the light model, and reset name
	CFWNode* pNdLgt = __super::Copy(ptd_mpNodes[strMdlTmpLgt]);
	string strLgtNam = (pamI.has("mdlName")) ? \
		pamI.getString("mdlName") : pNdLgt->getNam();
	strLgtNam += s_cstrLgtLoc;
	pNdLgt->resetNam(strLgtNam);

	return pNdLgt;
}

CFWParam CFWMdlMngr::GetGeoCrtePam(CFWParam pamI)
{
	CFWParam pamMshIni;
	if(pamI.has("plnLength") && pamI.has("plnWidth"))
	{
		pamMshIni = CFWMshFcty::GenPlnInitPam(
			pamI.getFloat("plnLength"), pamI.getFloat("plnWidth"),
			(pamI.has("plnLenSec")) ? pamI.getUint("plnLenSec") : 1,
			(pamI.has("plnWidSec")) ? pamI.getUint("plnWidSec") : 1);
		return pamI.mrg(pamMshIni);
	} else
	if(pamI.has("boxLTF") && pamI.has("boxRBB"))
	{
		pamMshIni = CFWMshFcty::GenBoxInitPam(CBoxAABB(
			pamI.getQuate("boxLTF"), pamI.getQuate("boxRBB")),
			pamI.has("wireFrame") ? pamI.getBool("wireFrame") : false);
		return pamI.mrg(pamMshIni);
	} else
	if(pamI.has("lnBegPoi") && pamI.has("lnEndPoi"))
	{
		pamMshIni = CFWMshFcty::GenLineInitPam(
			pamI.getQuate("lnBegPoi"),
			pamI.getQuate("lnEndPoi"));
		return pamI.mrg(pamMshIni);
	} else
	if(pamI.has("tglPoiA") && pamI.has("tglPoiB") && pamI.has("tglPoiC"))
	{
		pamMshIni = CFWMshFcty::GenTriangleInitPam(
			pamI.getQuate("tglPoiA"),
			pamI.getQuate("tglPoiB"),
			pamI.getQuate("tglPoiC"));
		return pamI.mrg(pamMshIni);
	} else
	if(pamI.has("poiLT") && pamI.has("poiRT")
	&& pamI.has("poiLB") && pamI.has("poiRB"))
	{
		pamMshIni = CFWMshFcty::GenRectInitPam(
			pamI.getQuate("poiLT"), pamI.getQuate("poiRT"),
			pamI.getQuate("poiLB"), pamI.getQuate("poiRB"),
			pamI.has("wireFrame") ? pamI.getBool("wireFrame") : false);
		return pamI.mrg(pamMshIni);
	}
	return CFWParam();
}

CFWModel* CFWMdlMngr::createGeoMdl(CFWParam pamI)
{
	// New model and reset its name if it needs to
	CFWModel* pMdlNew = new CFWModel();
	if(pamI.has("mdlName"))
	{
		pMdlNew->resetNam(pamI.getString("mdlName"));
	}

	// Create and initialize the new model's mesh and skin
	fillInMdlByPam( pamI, pMdlNew);

	// Insert into nodes' map
	ptd_mpNodes.insert(VtpNode( pMdlNew->getNam(), pMdlNew));

	return pMdlNew;
}

CFWModel* CFWMdlMngr::createTrnMdl(CFWParam pamI)
{
	// New model and reset its name if it needs to
	CFWTerrain* pMdlNew = new CFWTerrain();
	if(pamI.has("mdlName"))
	{
		pMdlNew->resetNam(pamI.getString("mdlName"));
	}

	// Fill in the initialize parameters to terrain
	pMdlNew->ptd_fLength = pamI.getFloat("plnLength");
	pMdlNew->ptd_fWidth = pamI.getFloat("plnWidth");
	pMdlNew->ptd_uHgtWid = pamI.getUint("hgtMapWid");
	pMdlNew->ptd_uHgtHgt = pamI.getUint("hgtMapHgt");
	pMdlNew->ptd_ayHgt = CFWTerrain::loadHgtMap(
		pamI.getString("hgtMapFile"),
		pMdlNew->ptd_uHgtWid, pMdlNew->ptd_uHgtHgt,
		(pamI.has("hgtScale") ? pamI.getFloat("hgtScale") : 0.5));
	pMdlNew->ptd_prTrnHgt.first = (float)pMdlNew->ptd_uHgtWid / pMdlNew->ptd_fLength;
	pMdlNew->ptd_prTrnHgt.second = (float)pMdlNew->ptd_uHgtHgt / pMdlNew->ptd_fWidth;

	if (pamI.has("trnC1") && pamI.has("trnC2"))
	{
		// Generate auto terrain initialize parameter
		pMdlNew->pvt_fC1 = pamI.getFloat("trnC1");
		pMdlNew->pvt_fC2 = pamI.getFloat("trnC2");
		pamI.mrg(pMdlNew->genAutoTrnInitPam());
	}
	else
	{
		// Generate fixed terrain initialize parameter
		if(pamI.has("plnLenSec") && pamI.has("plnWidSec"))
		{
			pMdlNew->ptd_uLenSec = pamI.getUint("plnLenSec");
			pMdlNew->ptd_uWidSec = pamI.getUint("plnWidSec");
		}
		else
		{
			pMdlNew->ptd_uLenSec = 10;
			pMdlNew->ptd_uWidSec = 10;
		}
		pamI.mrg(pMdlNew->genFixdTrnInitPam());
	}

	// Create and initialize the new model's mesh and skin
	fillInMdlByPam( pamI, pMdlNew);

	// Insert into nodes' map
	ptd_mpNodes.insert(VtpNode( pMdlNew->getNam(), pMdlNew));

	return pMdlNew;
}

void CFWMdlMngr::fillInMdlByPam( CFWParam pamI, CFWModel* pMdlO)
{
	// Create and initialize mesh for model
	CFWMshIntf& rMshIntf = pMdlO->ptd_mshIntf;
	rMshIntf = (CFWMshIntf&)CFWMshFcty::getInstance()->Create(pamI);
	rMshIntf.TransferTo( CFWMshIntf::MSH_STT_INITIALIZED, pamI);
	CFWMshFcty::ClsCrtePam(pamI);

	// Create skin for model(If there's no diffuse material 
	//  and texture, manager wont set skin to model)
	std::map<UINT, CFWParam> mpCrteSkn;
	typedef std::map<UINT, CFWParam>::value_type VtpPam;
	std::vector<string> ayAllKey = pamI.getKeySet();
	for( UINT i = 0; i < ayAllKey.size(); ++i)
	{
		string strKey = ayAllKey[i];
		if(strKey.length() <= 6)	{ continue; }
		string strKeyHd = strKey.substr( 0, 6);
		if(strKeyHd == "mtlDif"
		|| strKeyHd == "mtlAmb"
		|| strKeyHd == "mtlSpc"
		|| strKeyHd == "mtlEms"
		|| strKeyHd == "mtlPwr"
		|| strKeyHd == "difTex"
		|| strKeyHd == "ambTex"
		|| strKeyHd == "spcTex"
		|| strKeyHd == "obcTex"
		|| strKeyHd == "bmpTex"
		|| strKeyHd == "relTex"
		|| strKeyHd == "refTex"
		|| strKeyHd == "dspTex")
		{
			UINT uSubID = strToNum<UINT>(strKey.substr(6));
			// Check whether has such subset
			if(!rMshIntf.HasSubset(uSubID))	{ continue; }
			if(!CHK_EXS_STL( mpCrteSkn, uSubID))
			{
				mpCrteSkn.insert(VtpPam( uSubID, CFWParam()));
			}
			string strVal = pamI.getString(strKey);
			mpCrteSkn[uSubID].add( strKeyHd, strVal);
		}
	}
	typedef std::map<UINT, CFWParam>::iterator ItrCrteSkn;
	CFWSknFcty* pSknFcty = CFWSknFcty::getInstance();
	for( ItrCrteSkn itr = mpCrteSkn.begin(); itr != mpCrteSkn.end(); ++itr)
	{
		CFWSknIntf sknNew = (CFWSknIntf&)pSknFcty->Create(itr->second);
		pMdlO->ptd_mpSknIntf.insert(CFWModel::VtpSknIntf( itr->first, sknNew));
	}

	// Generate default space control
	pamI.add( "outSrdBox", rMshIntf.GeneOutSrdBox().convertToStr());
	pamI.add( "outSrdSph", rMshIntf.GeneOutSrdSph().convertToStr());
	pMdlO->ptd_spcLoc = (CFWSpcIntf&)CFWSpcFcty::getInstance()->Create(pamI);

	// Set auxiliary object type
	if(pamI.has("mdlAuxTyp"))
	{
		pMdlO->setAuxTyp((CFWNode::EFWAuxObj)pamI.getUint("mdlAuxTyp"));
	}

	// Set mesh as include object
	pMdlO->setIncObj(rMshIntf.GetObjID());
}