#pragma once
#include "stdafx.h"
#include "FWEngine.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWManager.h"
#include "../OPMath/Math.h"
using OPMath::CPoint;

SINGLE_IMPLEMENT(CFWEngine)

HRESULT CFWEngine::initialize(CFWParam pamI)
{
	// Get window's handle. If not given,
	// take the active window as display window
	string strCfgFile = pamI.getString("cfgFile");
	HWND hWnd = ::GetActiveWindow();
	if(pamI.has("wndHdl"))
	{
		hWnd = (HWND)pamI.getDword("wndHdl");
	}

	// Create required device by information from file
	AryXmlNodes ayCrteDev = LoadParamFromXML( strCfgFile, "DEVICE");
	if(ayCrteDev.size() == 1)
	{
		IFWFactory* pDevFactory = CFWDevFcty::getInstance();
		CFWParam pamInitDev(ayCrteDev.front()->m_mpAttribute);
		pamInitDev.add( "wndHdl", OPTools::numToStr<DWORD>((DWORD)hWnd));
		CFWDevIntf& rDevice = (CFWDevIntf&)pDevFactory->Create(pamInitDev);
		if(FAILED(rDevice.TransferTo(CFWDevIntf::DEV_STT_INITIALIZED)))
		{
			return E_FAIL;
		}
	}

	// Set GUI config by information from file
	CFWParam pamSetGUI;
	AryXmlNodes aySetGUI = LoadParamFromXML( strCfgFile, "GUI");
	for( UINT i = 0; i < aySetGUI.size(); ++i)
	{
		pamSetGUI.mrg(aySetGUI[i]->m_mpAttribute);
	}
	CFWGuiMngr::getInstance()->Initialize(pamSetGUI);

	// Create scenes by information from file
	AryXmlNodes ayCrteScn = LoadParamFromXML( strCfgFile, "SCENE");
	for( UINT i = 0; i < ayCrteScn.size(); ++i)
	{
		CFWScnMngr::getInstance()->CreateByXmlNode(ayCrteScn[i]);
	}
	return S_OK;
}

void CFWEngine::display(float fDeltaI)
{
	CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL)	{ return; }
	CFWDevIntf* pCurDev = (CFWDevIntf*)pIntf;
	if(!pCurDev->TransferTo(CFWDevIntf::DEV_STT_PAINTING))	{ return; }

	// Do current scene and environment render
	CFWGuiMngr* pMgrGUI = CFWGuiMngr::getInstance();
	CFWNode* pScn = CFWScnMngr::getInstance()->getCurScn();
	if(pScn != NULL)
	{
		DWORD dwRndrStt = pMgrGUI->GetAuxRndrStt();
		dwRndrStt |= CFWNode::EFWAuxObj::MAIN_OBJ;
		if(FAILED(pScn->render( CMatrix(), dwRndrStt)))	{ return; }
	}

	// Show FPS on the left top of screen
	string strFPS = numToStr<float>( pMgrGUI->GetFPS(fDeltaI), 2);
	CFWParam pam( "showTxt", strFPS);
	RECT rectScn;
	::GetWindowRect( pCurDev->GetDispWnd(), &rectScn);
	CFWAABBBox rectOutSrd(
		CPoint( rectScn.left + 20, rectScn.top + 20),
		CPoint( rectScn.right, rectScn.bottom));
	pam.add( "outSrdRect", rectOutSrd.convertToStr());
	pam.add( "dockInRect", numToStr<UINT>(CFWTxtIntf::LFT_TOP));
	CFWIntf* pDftTxt = CFWTxtFcty::getInstance()->getDftTxt();
	pDftTxt->TransferTo( CFWTxtIntf::TXT_STT_PAINTING, pam);

	pCurDev->TransferTo(CFWDevIntf::DEV_STT_INITIALIZED);
}

void CFWEngine::clearUp()
{

}
