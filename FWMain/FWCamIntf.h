#pragma once
#pragma warning(disable:4800)
#include "FWMacro.h"
#include "FWIntf.h"
#include "FWObject.h"
#include "../OPMath/Math.h"
using namespace OPMath;

class FWMAIN_API CFWCamIntf: public CFWIntf
{
public:
	CFWCamIntf():
		pvt_status(CAM_STT_UNKNOWN)	{}
	CFWCamIntf(CFWObject* pCamI);
public:
	enum EFWCamStt
	{
		CAM_STT_UNKNOWN,
		CAM_STT_CREATED,
		CAM_STT_SET_OPR,
		CAM_STT_CHANGED
	};
	/*
	*	When the camera was just be created, status will set as CAM_STT_CREATED.
	*	The camera's effect wont show unless user set status to CAM_STT_SET_OPR.
	*	In this process, the camera will get the current device from engine, and
	*	call it's SetViewMat(CMatrix) interface to act the camera's effect, 
	*	the camera will also be set as operation camera for manager to store it's ID.
	*	If user want to change the direction of camera or move, rotate camera by any axis.
	*	this camera will transfer to status CAM_STT_CHANGED. That means you operation has 
	*	changed the camera location already, but the effect of camera wont be showed.
	*	In order to use the new camera display the world. you should transfer camera 
	*	status to CAM_STT_SET_OPR by hands. By that, camera will call the SetViewMat of 
	*	the current device for updating.
	*	Transfer Map
	*	CAM_STT_CREATED -> CAM_STT_SET_OPR will call current device's SetViewMat() and engine's SetOprCam()
	*	CAM_STT_SET_OPR -> CAM_STT_CHANGED will call LookAt() or Move/RotateByXX
	*		Requires parameters:	[1]"dirLook" = CVector;(cause to call lookTo())
	*								<!-- Required bound camera was free camera -->
	*								[2]"axisMov" = X/Y/Z, "disMov" = float;(cause to call moveByX/Y/Z())
	*								[3]"axisRot" = X/Y/Z, "angRot" = float;(cause to call rotateByX/Y/Z())
	*	CAM_STT_CHANGED -> CAM_STT_SET_OPR will call current device's SetViewMat()
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWCamStt pvt_status;
public:
	virtual CMatrix GetProjMat();
	virtual CMatrix GetViewMat();
	virtual CPoint GetEyeLoc();
	virtual CVector GetLookDir();
	virtual CVector GetRightDir();
	virtual CSqCone GetViewCone();
	virtual bool ChkPoiInsideVw(CPoint) const;
};