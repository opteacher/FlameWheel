#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWCamera.h"
#include "FWCamIntf.h"
#include "FWDevIntf.h"
#include "FWPool.h"
#include "FWMisc.h"
#include "../OPTools/String.h"
using OPTools::numToStr;

SINGLE_IMPLEMENT(CFWCamFcty)

CFWIntf& CFWCamFcty::Create(CFWParam& pamI)
{
	// Create a new camera according user's required
	CFWCamera* pObjCrte = new CFWCamFree();

	// Set the new camera with parameters
	// Get view's width and height
	if(pamI.has("viewWid") && pamI.has("viewHgt"))
	{
		pObjCrte->ptd_uViewWid = pamI.getUint("viewWid");
		pObjCrte->ptd_uViewHgt = pamI.getUint("viewHgt");
	}
	else
	{
		pObjCrte->ptd_uViewWid = 800;
		pObjCrte->ptd_uViewHgt = 600;
	}
	// Get projection plane
	if(pamI.has("clsProjPln") && pamI.has("farProjPln"))
	{
		pObjCrte->ptd_fClsPln = pamI.getFloat("clsProjPln");
		pObjCrte->ptd_fFarPln = pamI.getFloat("farProjPln");
	}
	else
	{
		pObjCrte->ptd_fClsPln = 1;
		pObjCrte->ptd_fFarPln = 1000;
	}
	// Get direction of looking
	if(pamI.has("dirLook"))
	{
		pObjCrte->ptd_posEye = CVT_POI_D3D(pamI.getQuate("locEye"));
		pObjCrte->ptd_dirLook = CVT_POI_D3D(pamI.getQuate("dirLook"));
		pObjCrte->regulate();
		float fDisView = pObjCrte->ptd_fFarPln - pObjCrte->ptd_fClsPln;
		pObjCrte->ptd_posTar = pObjCrte->ptd_dirLook * fDisView;
	}
	else if(pamI.has("locTgt"))
	{
		pObjCrte->ptd_posEye = CVT_POI_D3D(pamI.getQuate("locEye"));
		pObjCrte->ptd_posTar = CVT_POI_D3D(pamI.getQuate("locTgt"));
		pObjCrte->ptd_dirLook = pObjCrte->ptd_posTar - pObjCrte->ptd_posEye;
		pObjCrte->regulate();
	}
	else
	{
		throw std::runtime_error("dirLook and locTgt must define one of them!");
	}

	// Add new device into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pObjCrte);

	// Set new camera to a camera interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWCamIntf(pObjCrte);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));
	if(pamI.has("camName"))
	{
		string strDevNam = pamI.getString("camName");
		ptd_mpCpdTbl.insert(VtpCpdTbl( strDevNam, dwHashID));
	}

	// Set operation camera if it's needed
	if(pamI.has("setOperation") && pamI.getBool("setOperation"))
	{
		CFWCamIntf* pCam = (CFWCamIntf*)pIntfTmp;
		pCam->TransferTo( CFWCamIntf::CAM_STT_SET_OPR);
	}

	// Return the reference of the camera interface
	return *pIntfTmp;
}