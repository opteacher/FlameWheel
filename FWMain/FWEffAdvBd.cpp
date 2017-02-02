#pragma once
#include "stdafx.h"
#include "FWEffect.h"
#include "FWCamIntf.h"
#include "FWDevIntf.h"
#include "FWFactory.h"

using namespace OPMath;

INI_TYPID(CFWEffAdvBd);

SINGLE_IMPLEMENT(CFWEffAdvBd)

HRESULT CFWEffAdvBd::enable( CFWParam pamI, bool bEnableI)
{
	// Get operation camera
	if(CFWCamFcty::getInstance()->getOprCamID() == ERR_OBJ_ID)	{ return E_FAIL; }
	CFWCamIntf* pOprCam = (CFWCamIntf*)CFWCamFcty::getInstance()->getOprCam();

	// Get current device
	if(CFWDevFcty::getInstance()->getCurDevID() == ERR_OBJ_ID)	{ return E_FAIL; }
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();

	// Advertisement board's disable just reset world transform
	if(!bEnableI)	{ pCurDev->ResetWldMat(); return S_OK; }

	// Get current world transform matrix
	CMatrix matTsf = pCurDev->GetWldMat();

	// Get look direction
	CVector dirLook = pOprCam->GetLookDir().getNegVec().getIndVec();
	if(!dirLook.equal(pvt_dirLook))
	{
		pvt_dirLook = dirLook;

		// Turn object to camera roll by Axis Y
		CVector dirLkY = pvt_dirLook;	dirLkY.m_fY = 0;
		//+_+: default object's face direction
		pvt_matToCam = CMatrix::rotate3D( CVector( 0, 0, 1), dirLkY);

		// Calculate the angle between direction of look and Axis Y
		float fAngRot = CVector( 0, 1, 0).angWithVec( pvt_dirLook, false);
		// Turn object to camera roll by camera's right(Axis X)
		pvt_matToCam.dotLeftToSelf(CMatrix::rotate3D( fAngRot, CVector( 1, 0, 0)));
	}
	
	// Reset to world matrix
	pCurDev->SetWldMat(matTsf.dotLeft(pvt_matToCam));

	return S_OK;
}