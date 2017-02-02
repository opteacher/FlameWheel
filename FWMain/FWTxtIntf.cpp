#pragma once
#include "stdafx.h"
#include "FWTxtIntf.h"
#include "FWText.h"

CFWTxtIntf::CFWTxtIntf(CFWObject* pTxtI)
{
	if(pTxtI->chkIsClass(CFWText::getTypID()))
	{
		ptd_pBindObj = pTxtI;
		pvt_status = TXT_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = TXT_STT_UNKNOWN;
	}
}

bool CFWTxtIntf::TransferTo( DWORD dwSttI, CFWParam& pamI)
{
	// Check The Binding Device Available
	CFWText* pTxt = (CFWText*)ptd_pBindObj;
	if(pTxt == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	switch (pvt_status)
	{
	case TXT_STT_CREATED:
		if(dwSttI == TXT_STT_INITIALIZED)
		{
			return SUCCEEDED(pTxt->initialize(pamI));
		}
	case TXT_STT_INITIALIZED:
		if(dwSttI == TXT_STT_PAINTING)
		{
			return SUCCEEDED(pTxt->draw(pamI));
		}
	}
	return false;
}