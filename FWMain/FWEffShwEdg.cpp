#pragma once
#include "stdafx.h"
#include "FWEffect.h"
#include "FWDevIntf.h"
#include "FWSknIntf.h"
#include "FWFactory.h"
#include "FWD3DMisc.h"

INI_TYPID(CFWEffShwEdg);

SINGLE_IMPLEMENT(CFWEffShwEdg)

HRESULT CFWEffShwEdg::enable( CFWParam pamI, bool bEnbI)
{
	// Get current device
	CFWDevIntf* pDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pDev == NULL || pDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pDev->GetImpDev();

	// Enable/Disable effect
	if(bEnbI)
	{
		CFWSknFcty* pSknFcty = CFWSknFcty::getInstance();

		D3DMATERIAL9 mtrlEdg = tunToD3DMtrl(pDev->GetBkGrdCol());
		if(pamI.has("mtlEdg"))
		{
			mtrlEdg = tunToD3DMtrl(pamI.getMaterial("mtlEdg"));
		}
		else if(pSknFcty->getActSknID() != ERR_OBJ_ID)
		{
			CFWSknIntf* pSkn = (CFWSknIntf*)pSknFcty->getActSkn();
			if(pSkn != NULL && pSkn->BeBound())
			{
				mtrlEdg = tunToD3DMtrl(pSkn->GetMtrl().getNegMtl());
			}
		}

		if(FAILED(pD3DDev->GetMaterial(&pvt_mtlFmr)))							{ return E_FAIL; }
		if(FAILED(pD3DDev->SetMaterial(&mtrlEdg)))								{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME)))	{ return E_FAIL; }
	}
	else
	{
		if(FAILED(pD3DDev->SetMaterial(&pvt_mtlFmr)))							{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID)))		{ return E_FAIL; }
	}

	return S_OK;
}