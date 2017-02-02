#pragma once
#include "FWMacro.h"
#include "FWIntf.h"
#include "../OPMath/Math.h"
using OPMath::CMatrix;
using std::pair;

class FWMAIN_API CFWDevIntf: public CFWIntf
{
public:
	CFWDevIntf():
		pvt_status(DEV_STT_UNKNOWN)	{}
	CFWDevIntf(CFWObject* pDevI);
public:
	enum EFWDevStt
	{
		DEV_STT_UNKNOWN,
		DEV_STT_CREATED,
		DEV_STT_RESTORING,//restore the device(after change some setting)
		DEV_STT_DESTROYED,
		DEV_STT_INITIALIZED,//do initializing
		DEV_STT_PAINTING,//do begin-end paint
	};

	/*
	*	Transfer map
	*	When the device just be created, the status will set as CREATED,
	*	this status nearly like DESTROYED status, but identified the first 
	*	status that the device has not initialized.
	*	CREATED -> INITIALIZED will call CFWDevice::initialize()
	*
	*	INITIALIZED -> DESTROYED will call CFWDevice::destory()
	*	DESTROYED -> INITIALIZED will call CFWDevice::initialize()
	*
	*	The RESTORING only for device reseting, user can change device
	*	setting anytime. but the result wont show out immediately. to activation
	*	that, restore() should be called during the status transfer to RESTORING
	*	it will take some times, and during that the device will set in the RESTORING.
	*	and when the job has been done, the status will reset to INITIALIZED automatic
	*	INITIALIZED -> DEV_STT_RESTORING will call CFWDevice::restore()
	*
	*	Painting is the important work for device, only during this status, models of
	*	the current scene can be paint. when the PAINTING has been started, it cant stop 
	*	by itself, user should reset status to INITIALIZED by hands, and this wont call
	*	initialize() instead of endPaint()
	*	INITIALIZED -> DEV_STT_PAINTING will call CFWDevice::begPaint()
	*	DEV_STT_PAINTING -> INITIALIZED will call CFWDevice::endPaint()
	*
	*	If user choice error status to transfer, the function will return false
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWDevStt pvt_status;
public:
	virtual HWND GetDispWnd() const;
	virtual CMatrix SetWldMat(CMatrix);
	virtual CMatrix GetWldMat() const;
	virtual void ResetWldMat();
	virtual void SetViewMat(CMatrix);
	virtual void SetProjMat(CMatrix);
	virtual LPVOID GetImpDev() const;
	virtual SFWColor GetBkGrdCol() const;
};