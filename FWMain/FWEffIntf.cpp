#pragma once
#include "stdafx.h"
#include "FWEffIntf.h"
#include "FWEffect.h"

CFWEffIntf::CFWEffIntf( CFWObject* pEffI )
{
	if(pEffI->chkIsClass(CFWEffect::getTypID()))
	{
		ptd_pBindObj = pEffI;
		pvt_status = EFF_STT_DSABLE;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = EFF_STT_UNKNOWN;
	}
}

bool CFWEffIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check the binding effect available
	CFWEffect* pEff = (CFWEffect*)ptd_pBindObj;
	if(pEff == NULL)	{ return false; }

	// According current status, do callback functions
	switch (pvt_status)
	{
	case CFWEffIntf::EFF_STT_DSABLE:
		if(sttToI == EFF_STT_ENABLE)
		{
			if(SUCCEEDED(pEff->enable(pamIO)))
			{
				pvt_status = EFF_STT_ENABLE;
				return true;
			}
		}
		else if(sttToI == EFF_STT_DSABLE)
		{
			pamIO.add( "effNoChg", "true");
			return true;
		}
		break;
	case CFWEffIntf::EFF_STT_ENABLE:
		if(sttToI == EFF_STT_DSABLE)
		{
			if(SUCCEEDED(pEff->enable( pamIO, false)))
			{
				pvt_status = EFF_STT_DSABLE;
				return true;
			}
		}
		else if(sttToI == EFF_STT_ENABLE)
		{
			pamIO.add( "effNoChg", "true");
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool CFWEffIntf::Enable(CFWParam& pamIO)
{
	if((pamIO.has("enable")) ? pamIO.getBool("enable") : true)
	{
		return this->TransferTo( EFF_STT_ENABLE, pamIO);
	}
	else
	{
		return this->TransferTo( EFF_STT_DSABLE, pamIO);
	}
}

bool CFWEffIntf::Negative(CFWParam& pamIO)
{
	if(pvt_status == EFF_STT_ENABLE)
	{
		return this->TransferTo( EFF_STT_DSABLE, pamIO);
	}
	else
	{
		return this->TransferTo( EFF_STT_ENABLE, pamIO);
	}
}