#pragma once
#include "stdafx.h"
#include "FWManager.h"
#include "FWFactory.h"
#include "FWCamIntf.h"
#include "FWScene.h"
#include "../OPTools/String.h"
using OPTools::numToStr;

LRESULT CALLBACK procMouse( int nCode, WPARAM wParam, LPARAM lParam)
{
	CFWGuiMngr* pGuiIns = CFWGuiMngr::getInstance();
	HHOOK hMosHdl = pGuiIns->getMouseHook();
	string strMosRotSpd = numToStr<float>(pGuiIns->getMouseRotSpeed());
	float fMosMovSpd = pGuiIns->getMouseMovSpeed();

	if(HC_ACTION == nCode)
	{
		// Get mouse's information
		PMOUSEHOOKSTRUCTEX pMosInf = (PMOUSEHOOKSTRUCTEX)lParam;

		// Get current camera
		CFWCamIntf* pOprCam = (CFWCamIntf*)CFWCamFcty::getInstance()->getOprCam();
		if(pOprCam == NULL)
		{
			return ::CallNextHookEx( hMosHdl, nCode, wParam, lParam);
		}

		switch(wParam)
		{
		case WM_MOUSEWHEEL:

			if(pMosInf->mouseData == (WHEEL_DELTA<<16))
			{
				CFWParam pamRot( "axisMov", "Z");
				pamRot.add( "disMov", strMosRotSpd);
				pOprCam->TransferTo( CFWCamIntf::CAM_STT_CHANGED, pamRot);
				pOprCam->TransferTo(CFWCamIntf::CAM_STT_SET_OPR);
			}
			else if(pMosInf->mouseData == (-WHEEL_DELTA<<16))
			{
				CFWParam pamRot( "axisMov", "Z");
				pamRot.add( "disMov", "-" + strMosRotSpd);
				pOprCam->TransferTo( CFWCamIntf::CAM_STT_CHANGED, pamRot);
				pOprCam->TransferTo(CFWCamIntf::CAM_STT_SET_OPR);
			}
			break;
		case WM_MOUSEMOVE:
			// Check mouse M button whether down
			if(::GetAsyncKeyState(VK_MBUTTON) & 0x8000f)
			{
				POINT poiCur = pMosInf->pt;
				POINT poiFmr = pGuiIns->getPoiMosFmrMov();

				float fRowSec = (float)(poiCur.y - poiFmr.y)/fMosMovSpd;
				float fColSec = (float)(poiCur.x - poiFmr.x)/fMosMovSpd;

				CFWParam pamMvX( "axisMov", "X");
				pamMvX.add( "disMov", numToStr<float>(-fColSec));
				pOprCam->TransferTo( CFWCamIntf::CAM_STT_CHANGED, pamMvX);
				CFWParam pamMvY( "axisMov", "Y");
				pamMvY.add( "disMov", numToStr<float>(fRowSec));
				pOprCam->TransferTo( CFWCamIntf::CAM_STT_CHANGED, pamMvY);
				pOprCam->TransferTo(CFWCamIntf::CAM_STT_SET_OPR);
			}
			pGuiIns->setPoiMosFmrMov(pMosInf->pt);
			break;
		default:
			break;
		}
	}

	return ::CallNextHookEx( hMosHdl, nCode, wParam, lParam);
}

SINGLE_IMPLEMENT(CFWGuiMngr)

VTP_INI_MAP( string, DWORD);
BEG_INI_MAP( string, DWORD, 2, g_ayGuiSet)
	VtpKeyVal( "showWorldCoordinateSys",	CFWNode::AUX_COD_WLD),
	VtpKeyVal( "showLightLocation",			CFWNode::AUX_LGT_LOC)
END_INI_MAP( string, DWORD, 2, g_ayGuiSet, g_mpGuiSet)

void CFWGuiMngr::Initialize(CFWParam pamI)
{
	// Enable control IO
	EnableCtlIO((pamI.has("enableControlIO")) ?
		pamI.getBool("enableControlIO") : true);
	
	// Show world coordinate system
	// Show light location object
	ITR_RVW_MAP( string, DWORD);
	for( ItrKeyVal iter = g_mpGuiSet.begin(); iter != g_mpGuiSet.end(); ++iter)
	{
		if(pamI.has(iter->first))
		{
			if(pamI.getBool(iter->first))	{ ptd_dwSttAuxRndr |= iter->second; }
			else							{ ptd_dwSttAuxRndr &= ~iter->second; }
		}
	}
}

void CFWGuiMngr::EnableCtlIO(bool bEnableI)
{
	if(bEnableI && ptd_hMosHook == NULL)
	{
		// Create mouse's hook
		::GetCursorPos(&ptd_poiFmrMov);
		ptd_hMosHook = ::SetWindowsHookEx(
			WH_MOUSE,
			procMouse,
			NULL,
			::GetCurrentThreadId());
		if(!ptd_hMosHook)
		{
			throw std::runtime_error("System error: Disable create mouse hook!");
		}
	}
	else
	{
		if(ptd_hMosHook)
		{
			::UnhookWindowsHookEx(ptd_hMosHook);
			ptd_hMosHook = NULL;
		}
	}
}

float CFWGuiMngr::GetFPS(float fTimeDeltaI)
{
	// One frame ended, number plus 1
	++ptd_dwNumFrm;
	// Passed time plus current frame's time delta
	ptd_fTimePas += fTimeDeltaI;

	if(ptd_fTimePas >= 1.0f)
	{
		ptd_fFPS = (float)ptd_dwNumFrm/ptd_fTimePas;

		// Reset value for next one second
		ptd_dwNumFrm = 0;
		ptd_fTimePas = 0;
	}
	return ptd_fFPS;
}