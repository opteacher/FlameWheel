#pragma once
#include "stdafx.h"
#include "FWManager.h"
#include "FWScene.h"
#include "FWFactory.h"
#include "../OPTools/String.h"
using OPTools::cmpStrIgnSize;
using OPTools::strToNum;

SINGLE_IMPLEMENT(CFWScnMngr)

CFWNode* CFWScnMngr::Create(CFWParam pamI)
{
	// Create new scene and reset name if needs
	CFWScene* pScn = new CFWScene();
	if(pamI.has("scnName"))
	{
		pScn->resetNam(pamI.getString("scnName"));
	}

	// Create space control for node
	if(pamI.has("spcPos"))	{ pScn->setLocation(pamI.getQuate("spcPos")); }
	if(pamI.has("spcLoc"))	{ pScn->setCoordSys(pamI.getCoordinate("spcLoc")); }

	// Insert into nodes map
	ptd_mpNodes.insert(VtpNode( pScn->getNam(), pScn));

	// Check whether set as current
	if(pamI.has("setCurrent") && pamI.getBool("setCurrent"))
	{
		this->SwitchScene(pScn->getNam());
	}
	return pScn;
}

CFWNode* CFWScnMngr::CreateByXmlNode(SXmlNode* pRootI)
{
	if(!cmpStrIgnSize( pRootI->m_strNodeTyp, "SCENE"))	{ return NULL; }

	// Create new scene and reset name if needs
	CFWScene* pNewScn = new CFWScene();
	if(!pRootI->m_strNodeNam.empty())
	{
		pNewScn->resetNam(pRootI->m_strNodeNam);
	}

	// Create space control for node
	CFWParam pamScn(pRootI->m_mpAttribute);
	if(pamScn.has("spcPos"))	{ pNewScn->setLocation(pamScn.getQuate("spcPos")); }
	if(pamScn.has("spcLoc"))	{ pNewScn->setCoordSys(pamScn.getCoordinate("spcLoc")); }

	// Insert into nodes map
	ptd_mpNodes.insert(VtpNode( pNewScn->getNam(), pNewScn));

	// Collect sub nodes according XML
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();
	AryXmlNodes aySubset = pRootI->m_aySubNodes;
	for( UINT i = 0; i < aySubset.size(); ++i)
	{
		SXmlNode* pNdXml = aySubset[i];
		// If effect, get effect id and param
		if(cmpStrIgnSize( pNdXml->m_strNodeTyp, "EFFECT"))
		{
			// Get effect ID by it's string name
			UINT uEffID = pEffMgr->CvtStrToEffID(pNdXml->m_strNodeID);
			if(!pEffMgr->ChkEffAva(uEffID))	{ continue; }

			// Collected effect was available, add into new scene
			pNewScn->AddEffect( uEffID, CFWParam(pNdXml->m_mpAttribute));
		}
		else
		{
			pNewScn->addSubNode(createObjForScn(pNdXml));
		}
	}

	// Check whether set as current
	MAP_STR mpAttr = pRootI->m_mpAttribute;
	if(CHK_EXS_STL( mpAttr, "setCurrent") && STR_TO_BOOL(mpAttr["setCurrent"]))
	{
		this->SwitchScene(pNewScn->getNam());
	}

	return pNewScn;
}

#define NEW_OBJ_TO_SPACE(pFactory)	\
	CFWNode* pNode = new CFWNode();\
	CFWIntf& rNew = pFactory->Create(pamTmp);\
	pNode->setIncObj(rNew.GetObjID());

CFWNode* CFWScnMngr::CreateByPamAry(AryIniScn ayIniI)
{
	CFWMdlMngr* pMdlMgr = CFWMdlMngr::getInstance();
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();

	// Create every nodes as subset
	CFWScene* pScn = new CFWScene();
	for( UINT i = 0; i < ayIniI.size(); ++i)
	{
		CFWParam pamTmp = ayIniI[i];
		if(pamTmp.has("scnName"))
		{
			// Create space control for node
			if(pamTmp.has("spcPos"))	{ pScn->setLocation(pamTmp.getQuate("spcPos")); }
			if(pamTmp.has("spcLoc"))	{ pScn->setCoordSys(pamTmp.getCoordinate("spcLoc")); }

			// Insert into nodes map
			ptd_mpNodes.insert(VtpNode( pScn->getNam(), pScn));

			// Check whether set as current
			if(pamTmp.has("setCurrent") && pamTmp.getBool("setCurrent"))
			{
				this->SwitchScene(pScn->getNam());
			}
		} else
		if(pamTmp.has("mdlName"))
		{
			if(!pamTmp.has("aryPois"))
			{
				pScn->addSubNode(pMdlMgr->Create(pamTmp));
			}
			else
			{
				pScn->addSubNode(pMdlMgr->createGeoMdl(pamTmp));
			}
		} else
		if(pamTmp.has("camName"))
		{
			NEW_OBJ_TO_SPACE(CFWCamFcty::getInstance());
			pScn->addSubNode(pNode);
		} else
		if(pamTmp.has("lgtName"))
		{
			NEW_OBJ_TO_SPACE(CFWLgtFcty::getInstance());
			pScn->addSubNode(pNode);
		} else
		if(pamTmp.has("effID"))
		{
			UINT uEffID = pEffMgr->CvtStrToEffID(pamTmp.getString("effID"));
			if(!pEffMgr->ChkEffAva(uEffID))	{ continue; }
			pScn->AddEffect( uEffID, pamTmp);
		}
	}
	return pScn;
}

CFWNode* CFWScnMngr::createObjForScn(SXmlNode* pSubI)
{
	CFWMdlMngr* pMdlMgr = CFWMdlMngr::getInstance();
	CFWParam pamTmp(pSubI->m_mpAttribute);
	if(cmpStrIgnSize( pSubI->m_strNodeTyp, "MODEL"))
	{
		std::vector<SXmlNode*> aySubs = pSubI->m_aySubNodes;
		CFWNode* pNdRet = aySubs.empty() ? pMdlMgr->Create(pamTmp) : NULL;
		for( UINT i = 0; i < aySubs.size(); ++i)
		{
			SXmlNode* pXmlNd = aySubs[i];
			if(cmpStrIgnSize( pXmlNd->m_strNodeTyp, "SUBSET"))
			{
				UINT uSubID = i;
				if(!pXmlNd->m_strNodeID.empty())
				{
					uSubID = strToNum<UINT>(pXmlNd->m_strNodeID);
				}
				CFWParam pamIniMsh(CFWMdlMngr::GetGeoCrtePam(pXmlNd->m_mpAttribute));
				if(!pamTmp.has("aryPois"))	{ pamTmp.mrg(pamIniMsh); }
				else
				{
					CFWMshFcty::MrgPam( pamTmp, pamIniMsh, uSubID);
					CFWSknFcty::MrgPam( pamTmp, pamIniMsh);
				}
			}
			else
			{
				pNdRet->addSubNode(createObjForScn(pXmlNd));
			}
		}
		if(pNdRet == NULL)
		{
			pNdRet = pMdlMgr->createGeoMdl(pamTmp);
		}
		return pNdRet;
	}
	if(cmpStrIgnSize( pSubI->m_strNodeTyp, "CAMERA"))
	{
		NEW_OBJ_TO_SPACE(CFWCamFcty::getInstance()) return pNode;
	}
	if(cmpStrIgnSize( pSubI->m_strNodeTyp, "LIGHT"))
	{
		NEW_OBJ_TO_SPACE(CFWLgtFcty::getInstance()) return pNode;
	}

	return NULL;
}

bool CFWScnMngr::SwitchScene(string strNamI)
{
	// Check existed and type
	if(!CHK_EXS_STL( ptd_mpNodes, strNamI))	{ return false; }
	CFWScene* pScn = (CFWScene*)ptd_mpNodes[strNamI];
	if(!pScn->chkIsClass(CFWScene::getTypID()))	{ return false; }

	// Do switch
	if(FAILED(pScn->preRender()))	{ return false; }
	CiterNode iterFnd = ptd_mpNodes.find(ptd_strCurScn);
	if(iterFnd != ptd_mpNodes.end())
	{
		if(FAILED(((CFWScene*)iterFnd->second)->sufRender()))
		{
			return false;
		}
	}
	ptd_strCurScn = strNamI;

	return true;
}

bool CFWScnMngr::SwitchScene(CFWNode* pScnI)
{
	return SwitchScene(pScnI->getNam());
}