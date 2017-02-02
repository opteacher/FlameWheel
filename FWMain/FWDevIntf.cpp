#pragma once
#include "stdafx.h"
#include "FWDevIntf.h"
#include "FWDevice.h"
#include "FWPool.h"

CFWDevIntf::CFWDevIntf( CFWObject* pDevI )
{
	if(pDevI->chkIsClass(CFWDevice::getTypID()))
	{
		ptd_pBindObj = pDevI;
		pvt_status = DEV_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = DEV_STT_UNKNOWN;
	}
}

bool CFWDevIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check The Binding Device Available
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	bool bTrsfSucceed = false;
	switch(pvt_status)
	{
	case DEV_STT_CREATED:
		if(sttToI == DEV_STT_INITIALIZED)
		{
			pvt_status = DEV_STT_INITIALIZED;
			return SUCCEEDED(pDev->initialize());
		}
		break;
	case DEV_STT_INITIALIZED:
		switch(sttToI)
		{
		case DEV_STT_DESTROYED:
			pDev->destory();
			pvt_status = DEV_STT_DESTROYED;
			bTrsfSucceed = true;
			break;
		case DEV_STT_PAINTING:
			bTrsfSucceed = SUCCEEDED(pDev->begPaint());
			pvt_status = DEV_STT_PAINTING;
			break;
		case DEV_STT_RESTORING:
			pvt_status = DEV_STT_RESTORING;
			bTrsfSucceed = SUCCEEDED(pDev->restore());
			pvt_status = DEV_STT_INITIALIZED;
			break;
		}
		return bTrsfSucceed;
	case DEV_STT_PAINTING:
		if(sttToI == DEV_STT_INITIALIZED)
		{
			pvt_status = DEV_STT_INITIALIZED;
			return SUCCEEDED(pDev->endPaint());
		}
		break;
	case DEV_STT_RESTORING:
		//During the restore status, cant do nothing transfer
		break;
	case DEV_STT_DESTROYED:
		if(sttToI == DEV_STT_INITIALIZED)
		{
			pvt_status = DEV_STT_INITIALIZED;
			return SUCCEEDED(pDev->initialize());
		}
		break;
	}
	return false;
}

HWND CFWDevIntf::GetDispWnd() const
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return NULL; }
	return pDev->getDispWnd();
}

CMatrix CFWDevIntf::SetWldMat(CMatrix matI)
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return CMatrix(); }
	return pDev->setWldMat(matI);
}

CMatrix CFWDevIntf::GetWldMat() const
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return CMatrix(); }
	return pDev->getWldMat();
}

void CFWDevIntf::ResetWldMat()
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return; }
	pDev->setWldMat(OPMath::CMatrix());
}

void CFWDevIntf::SetViewMat(CMatrix matI)
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return; }
	pDev->setViewMat(matI);
}

void CFWDevIntf::SetProjMat(CMatrix matI)
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return; }
	pDev->setProjMat(matI);
}

LPVOID CFWDevIntf::GetImpDev() const
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return NULL; }
	return pDev->getImpDev();
}

SFWColor CFWDevIntf::GetBkGrdCol() const
{
	CFWDevice* pDev = (CFWDevice*)ptd_pBindObj;
	if(pDev == NULL)	{ return SFWColor(); }
	return pDev->getBkGrdCol();
}