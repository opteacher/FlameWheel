#pragma once
#include "stdafx.h"
#include "FWEffect.h"
#include "FWDevIntf.h"
#include "FWFactory.h"
#include "FWD3DMisc.h"
#include "d3dx9.h"

INI_TYPID(CFWEffAmbLgt);

SINGLE_IMPLEMENT(CFWEffAmbLgt)

HRESULT CFWEffAmbLgt::enable( CFWParam pamI, bool bEnbI)
{
	// Get current device
	CFWDevIntf* pDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pDev == NULL || pDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pDev->GetImpDev();

	// Set ambient light
	D3DCOLORVALUE colAmb = tunToD3DCol(g_colWhite);
	if(pamI.has("lgtCol"))
	{
		colAmb = tunToD3DCol(pamI.getColor("lgtCol"));
	}
	return pD3DDev->SetRenderState( D3DRS_AMBIENT, COL_TO_DWD(colAmb));
}