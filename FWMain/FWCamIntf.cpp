#pragma once
#include "stdafx.h"
#include "FWCamIntf.h"
#include "FWCamera.h"
#include "FWDevIntf.h"
#include "FWMisc.h"
#include "FWFactory.h"
#include "../OPTools/String.h"
using OPTools::numToStr;

CFWCamIntf::CFWCamIntf( CFWObject* pCamI )
{
	if(pCamI->chkIsClass(CFWCamera::getTypID()))
	{
		ptd_pBindObj = pCamI;
		pvt_status = CAM_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = CAM_STT_UNKNOWN;
	}
}

bool CFWCamIntf::TransferTo( DWORD sttToI, CFWParam& pamIO )
{
	// Check The Binding Camera Available
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	switch(pvt_status)
	{
	case CAM_STT_CREATED:
		if(sttToI == CAM_STT_SET_OPR)
		{
			// Change the operation camera of manager
			CFWIntf* pIntf = CFWCamFcty::getInstance()->getOprCam();
			if(pIntf != NULL)
			{
				CFWCamIntf* pFmrCam = (CFWCamIntf*)pIntf;
				pFmrCam->pvt_status = CAM_STT_CREATED;
			}
			CFWCamFcty::getInstance()->setOprCamID(pCam->getObjID());

			// Get current device from device manager
			pIntf = CFWDevFcty::getInstance()->getCurDev();
			if(pIntf != NULL)
			{
				CFWDevIntf* pCurDev = (CFWDevIntf*)pIntf;
				pCurDev->SetProjMat(pCam->getProjMat());
				pCurDev->SetViewMat(pCam->getViewMat());
				pCurDev->ResetWldMat();
			}

			// Set status(if no current device, just set status)
			pvt_status = (EFWCamStt)sttToI;
			return true;
		}
		break;
	case CAM_STT_SET_OPR:
		if(sttToI == CAM_STT_CHANGED)
		{
			// Set status
			pvt_status = (EFWCamStt)sttToI;

			// Call functions according to parameters
			if(pamIO.has("dirLook"))
			{
				pCam->lookAt(pamIO.getQuate("dirLook"));
			}
			else if(pamIO.has("axisMov") && pamIO.has("disMov"))
			{
				// Only free camera can process these parameters
				if(!pCam->chkIsClass(CFWCamFree::getTypID()))	{ return false; }
				CFWCamFree* pCamFree = (CFWCamFree*)pCam;

				switch(pamIO.getChar("axisMov"))
				{
				case 'X': pCamFree->moveByX(pamIO.getFloat("disMov")); break;
				case 'Y': pCamFree->moveByY(pamIO.getFloat("disMov")); break;
				case 'Z': pCamFree->moveByZ(pamIO.getFloat("disMov")); break;
				default: throw std::runtime_error("error parameter: camera can move only by XYZ");
				}
			}
			else if(pamIO.has("axisRot") && pamIO.has("angRot"))
			{
				// Only free camera can process these parameters
				if(!pCam->chkIsClass(CFWCamFree::getTypID()))	{ return false; }
				CFWCamFree* pCamFree = (CFWCamFree*)pCam;

				switch(pamIO.getChar("axisRot"))
				{
				case 'X': pCamFree->rollByX(pamIO.getFloat("angRot")); break;
				case 'Y': pCamFree->rollByY(pamIO.getFloat("angRot")); break;
				case 'Z': pCamFree->rollByZ(pamIO.getFloat("angRot")); break;
				default: throw std::runtime_error("error parameter: camera can roll only by XYZ");
				}
			}
			else
			{
				pvt_status = CAM_STT_SET_OPR;
				return false;
			}
			
			pvt_status = CAM_STT_SET_OPR;
			return true;
		}
		else if(sttToI == CAM_STT_SET_OPR)
		{
			// Get current device from device manager
			CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
			if(pIntf != NULL)
			{
				CFWDevIntf* pCurDev = (CFWDevIntf*)pIntf;
				pCurDev->SetProjMat(pCam->getProjMat());
				pCurDev->SetViewMat(pCam->getViewMat());
				pCurDev->ResetWldMat();
			}
		}
		break;
	case CAM_STT_CHANGED:
		if(sttToI == CAM_STT_SET_OPR)
		{
			// Get current device from device manager
			CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
			if(pIntf != NULL)
			{
				CFWDevIntf* pCurDev = (CFWDevIntf*)pIntf;
				pCurDev->SetViewMat(pCam->getViewMat());
				pCurDev->ResetWldMat();
			}

			// Set status(if no current device, just set status)
			pvt_status = (EFWCamStt)sttToI;
			return true;
		}
	}
	return false;
}

CMatrix CFWCamIntf::GetProjMat()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CMatrix(); }
	return pCam->getProjMat();
}

CMatrix CFWCamIntf::GetViewMat()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CMatrix(); }
	return pCam->getViewMat();
}

CPoint CFWCamIntf::GetEyeLoc()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CPoint(); }
	return pCam->getEyeLoc();
}

CVector CFWCamIntf::GetLookDir()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CVector(); }
	return pCam->getLookDir();
}

CVector CFWCamIntf::GetRightDir()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CVector(); }
	return pCam->getRightDir();
}

CSqCone CFWCamIntf::GetViewCone()
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return CSqCone(); }
	return pCam->getViewCone();
}

bool CFWCamIntf::ChkPoiInsideVw(CPoint poiI) const
{
	CFWCamera* pCam = (CFWCamera*)ptd_pBindObj;
	if(pCam == NULL)	{ return false; }
	return pCam->getViewCone().isInside(poiI);
}