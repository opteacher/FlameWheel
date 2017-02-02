#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWText.h"
#include "FWTxtIntf.h"
#include "FWPool.h"

SINGLE_IMPLEMENT(CFWTxtFcty)

CFWIntf& CFWTxtFcty::Create(CFWParam& pamI)
{
	// Create a new device according user's required
	CFWText* pTxtCrte = NULL;
	if(pamI.has("3DText") && pamI.getBool("3DText"))
	{
		//@_@
	}
	else	{ pTxtCrte = new CFWText2D(); }

	// Add new device into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pTxtCrte);

	// Set new mesh to a mesh interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWTxtIntf(pTxtCrte);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));

	return *pIntfTmp;
}

CFWIntf* CFWTxtFcty::getDftTxt()
{
	if(ptd_dwDftTxt == ERR_OBJ_ID)
	{
		// Create default text
		CFWIntf& txtDft = Create(CFWParam());
		if(!txtDft.BeBound())	{ return NULL; }
		if(!txtDft.TransferTo(CFWTxtIntf::TXT_STT_INITIALIZED))
		{
			return NULL;
		}
		ptd_dwDftTxt = txtDft.GetObjID();
	}
	return GetIntfByID(ptd_dwDftTxt);
}