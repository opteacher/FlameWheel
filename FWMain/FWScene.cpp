#pragma once
#include "stdafx.h"
#include "FWScene.h"
#include "FWManager.h"

INI_TYPID(CFWScene);

HRESULT CFWScene::preRender()
{
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();
	CFWGuiMngr* pGuiMgr = CFWGuiMngr::getInstance();
	for( IterPam iter = ptd_mpEffect.begin(); iter != ptd_mpEffect.end(); ++iter)
	{
		CFWEffIntf& rEff = pEffMgr->GetEffect(iter->first);
		bool bEnble = iter->second.has("enable") ? iter->second.getBool("enable") : true;
		switch (iter->first)
		{
		case CFWEffMngr::EFFECT_SHOW_EDGE:
			if(bEnble)	{ pGuiMgr->EnableAuxStt(CFWNode::AUX_GRID); }
			else		{ pGuiMgr->DsableAuxStt(~CFWNode::AUX_GRID); }
			break;
		default:
			if(!rEff.Enable(iter->second))	{ return E_FAIL; }
		}
	}
	return S_OK;
}

HRESULT CFWScene::sufRender()
{
	CFWEffMngr* pEffMgr = CFWEffMngr::getInstance();
	CFWGuiMngr* pGuiMgr = CFWGuiMngr::getInstance();
	for( IterPam iter = ptd_mpEffect.begin(); iter != ptd_mpEffect.end(); ++iter)
	{
		CFWEffIntf& rEff = pEffMgr->GetEffect(iter->first);
		bool bEnble = iter->second.has("enable") ? iter->second.getBool("enable") : true;
		switch (iter->first)
		{
		case CFWEffMngr::EFFECT_SHOW_EDGE:
			if(bEnble)	{ pGuiMgr->DsableAuxStt(~CFWNode::AUX_GRID); }
			else		{ pGuiMgr->EnableAuxStt(CFWNode::AUX_GRID); }
			break;
		default:
			if(!rEff.Negative(iter->second))	{ return E_FAIL; }
		}
	}
	return S_OK;
}