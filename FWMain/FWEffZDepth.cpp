#pragma once
#include "stdafx.h"
#include "FWEffect.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWD3DMisc.h"

INI_TYPID(CFWEffZDepth);

SINGLE_IMPLEMENT(CFWEffZDepth)

HRESULT CFWEffZDepth::enable( CFWParam pamI, bool bEnableI)
{
	// Get current device
	CFWDevIntf* pDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pDev == NULL || pDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pDev->GetImpDev();

	// Enable/Disable Z-depth
	if(FAILED(pD3DDev->SetRenderState( D3DRS_ZENABLE, bEnableI)))	{ return E_FAIL; }
	if(bEnableI)
	{
		if(FAILED(pD3DDev->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS)))	{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE)))	{ return E_FAIL; }
	}

	return S_OK;
}