#pragma once
#include "stdafx.h"
#include "FWSknIntf.h"
#include "FWSkin.h"
#include "FWFactory.h"

CFWSknIntf::CFWSknIntf( CFWObject* pSknI )
{
	if(pSknI->chkIsClass(CFWSkin::getTypID()))
	{
		ptd_pBindObj = pSknI;
		pvt_status = SKN_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = SKN_STT_UNKNOWN;
	}
}

bool CFWSknIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	CFWSknFcty* pFcty = CFWSknFcty::getInstance();
	switch(pvt_status)
	{
	case SKN_STT_CREATED:
		if(sttToI == SKN_STT_ACTIVED)
		{
			if(FAILED(pSkn->actSkin()))	{ return false; }
			pvt_status = (EFWSknStt)sttToI;

			// Transfer former active skin to created status
			CFWSknIntf* pFmrSkn = (CFWSknIntf*)pFcty->getActSkn();
			if(pFmrSkn != NULL)
			{
				pFmrSkn->pvt_status = SKN_STT_CREATED;
				if(FAILED(pFmrSkn->ClsFmBlend()))	{ return false; }
			}

			pFcty->setActSknID(pSkn->getObjID());
			return true;
		}
		else if(sttToI == SKN_STT_DESTROYED)
		{
			pSkn->destroy();
			pvt_status = (EFWSknStt)sttToI;
			return true;
		}
		break;
	case SKN_STT_ACTIVED:
		if(sttToI == SKN_STT_CREATED)
		{
			pFcty->setActSknID(ERR_OBJ_ID);
			if(FAILED(pSkn->clsFmBlend()))	{ return false; }
			pvt_status = (EFWSknStt)sttToI;
			return true;
		}
		else if(sttToI == SKN_STT_DESTROYED)
		{
			pSkn->destroy();
			pvt_status = (EFWSknStt)sttToI;
			return true;
		}
	}
	return false;
}

HRESULT CFWSknIntf::ClsFmBlend()
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return E_FAIL; }

	return pSkn->clsFmBlend();
}

SFWMaterial CFWSknIntf::GetMtrl() const
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return SFWMaterial(); }

	return pSkn->getMtl();
}

SFWMaterial CFWSknIntf::SetMtrl(SFWMaterial mtlI)
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return SFWMaterial(); }

	SFWMaterial mtlRet = pSkn->getMtl();
	pSkn->setMtl(mtlI);
	return mtlRet;
}

string CFWSknIntf::SetTex( UINT uCnlI, string strFileI)
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return ""; }

	string strRet = pSkn->getTex(uCnlI);
	pSkn->setTex( strFileI, uCnlI);
	return strRet;
}

string CFWSknIntf::GetTexFile(UINT uCnlI) const
{
	// Check The Binding Skin Available
	CFWSkin* pSkn = (CFWSkin*)ptd_pBindObj;
	if(pSkn == NULL)	{ return ""; }

	return pSkn->getTex(uCnlI);
}
