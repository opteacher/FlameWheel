#pragma once
#include "stdafx.h"
#include "FWLgtIntf.h"
#include "FWLight.h"

CFWLgtIntf::CFWLgtIntf( CFWObject* pLgtI )
{
	if(pLgtI->chkIsClass(CFWLight::getTypID()))
	{
		ptd_pBindObj = pLgtI;
		pvt_status = LGT_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = LGT_STT_UNKNOWN;
	}
}

bool CFWLgtIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check The Binding Device Available
	CFWLight* pLgt = (CFWLight*)ptd_pBindObj;
	if(pLgt == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	switch (pvt_status)
	{
	case CFWLgtIntf::LGT_STT_CREATED:
		if(sttToI == LGT_STT_BE_SET)
		{
			if(SUCCEEDED(pLgt->setLgt(pamIO.getInt("lgtID"))))
			{
				pvt_status = LGT_STT_BE_SET;
				return true;
			}
		}
		break;
	case CFWLgtIntf::LGT_STT_BE_SET:
		if(sttToI == LGT_STT_BE_ENB)
		{
			bool bEnable = true;
			if(pamIO.has("enable"))	{ bEnable = pamIO.getBool("enable"); }
			if(SUCCEEDED(pLgt->enable(bEnable)))
			{
				pvt_status = LGT_STT_BE_ENB;
				return true;
			}
		}
		break;
	case CFWLgtIntf::LGT_STT_BE_ENB:
		if(LGT_STT_BE_SET)
		{
			if(SUCCEEDED(pLgt->enable(false)))
			{
				pvt_status = LGT_STT_BE_SET;
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

SFWQuate CFWLgtIntf::GetLgtLoc() const
{
	// Check The Binding Device Available
	CFWLight* pLgt = (CFWLight*)ptd_pBindObj;
	if(pLgt == NULL)	{ return SFWQuate(); }

	return pLgt->getLgtLoc();
}

UINT CFWLgtIntf::GetLgtType() const
{
	// Check The Binding Device Available
	CFWLight* pLgt = (CFWLight*)ptd_pBindObj;
	if(pLgt == NULL)	{ return LGT_TP_UNKNOW; }

	return pLgt->getLgtType();
}