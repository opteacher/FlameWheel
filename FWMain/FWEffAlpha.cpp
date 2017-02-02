#pragma once
#include "stdafx.h"
#include "FWEffect.h"
#include "FWDevIntf.h"
#include "FWFactory.h"

INI_TYPID(CFWEffAlpha);

SINGLE_IMPLEMENT(CFWEffAlpha)

HRESULT CFWEffAlpha::enable( CFWParam pamI, bool bEnableI)
{
	// Get current device
	CFWDevIntf* pDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pDev == NULL || pDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pDev->GetImpDev();

	// Set whether enable alpha blend
	if(FAILED(pD3DDev->SetRenderState( D3DRS_ZWRITEENABLE, !bEnableI)))			{ return E_FAIL; }
	if(FAILED(pD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, bEnableI)))		{ return E_FAIL; }
	if(bEnableI)
	{
		if(FAILED(pD3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))		{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))	{ return E_FAIL; }
	}

	// Alpha effect for texture
	//if(FAILED(pD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE)))	{ return E_FAIL; }
	//if(FAILED(pD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE)))	{ return E_FAIL; }
	//if(FAILED(pD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE)))	{ return E_FAIL; }

	return S_OK;
}