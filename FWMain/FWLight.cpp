#pragma once
#include "stdafx.h"
#include "FWLight.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWLgtIntf.h"
#include "FWD3DMisc.h"
#include "../OPMath/Math.h"

INI_TYPID(CFWLight);

CFWLight::CFWLight():
	ptd_iUsedID(-1),
	ptd_bEnable(false)
{
	ptd_lgtD3D.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	ptd_lgtD3D.Diffuse = tunToD3DCol(g_colWhite);
	ptd_lgtD3D.Ambient = tunToD3DCol(g_colWhite);
	ptd_lgtD3D.Specular = tunToD3DCol(g_colWhite);
	ptd_lgtD3D.Position = D3DXVECTOR3( 0, 0, 0);
}

HRESULT CFWLight::setLgt(int iLgtIdI)
{
	if(iLgtIdI < 0 || iLgtIdI >= MAX_LIGHT)	{ return E_FAIL; }
	
	CFWDevIntf* pIntf = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL || pIntf->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pIntf->GetImpDev();

	return pD3DDev->SetLight( iLgtIdI, &ptd_lgtD3D);
}

HRESULT CFWLight::enable(bool bEnbI)
{
	if(ptd_iUsedID < 0 || ptd_iUsedID >= MAX_LIGHT)	{ return E_FAIL; }

	CFWDevIntf* pIntf = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL || pIntf->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pIntf->GetImpDev();

	if(ptd_bEnable != bEnbI)
	{
		if(FAILED(pD3DDev->SetRenderState( D3DRS_LIGHTING, bEnbI)))			{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_NORMALIZENORMALS, bEnbI)))	{ return E_FAIL; }
		if(FAILED(pD3DDev->SetRenderState( D3DRS_SPECULARENABLE, bEnbI)))	{ return E_FAIL; }
		if(FAILED(pD3DDev->LightEnable( ptd_iUsedID, bEnbI)))				{ return E_FAIL; }
		ptd_bEnable = bEnbI;
	}

	return S_OK;
}

SFWQuate CFWLight::getLgtLoc() const
{
	if(ptd_lgtD3D.Type == D3DLIGHTTYPE::D3DLIGHT_POINT
	|| ptd_lgtD3D.Type == D3DLIGHTTYPE::D3DLIGHT_SPOT)
	{
		return CVT_POI_FW(ptd_lgtD3D.Position);
	}
	return SFWQuate();
}

typedef std::map<D3DLIGHTTYPE, CFWLgtIntf::EFELgtTyp>::value_type VtpLgtTyp;

BEG_INI_MAP( D3DLIGHTTYPE, CFWLgtIntf::EFELgtTyp, CFWLgtIntf::LGT_TP_UNKNOW, g_ayLgtTyp)
	VtpLgtTyp( D3DLIGHT_POINT, CFWLgtIntf::LGT_TP_POINT),
	VtpLgtTyp( D3DLIGHT_SPOT, CFWLgtIntf::LGT_TP_SPOT),
	VtpLgtTyp( D3DLIGHT_DIRECTIONAL, CFWLgtIntf::LGT_TP_DIRECT)
END_INI_MAP( D3DLIGHTTYPE, CFWLgtIntf::EFELgtTyp, CFWLgtIntf::LGT_TP_UNKNOW, g_ayLgtTyp, g_mpLgtTyp)

UINT CFWLight::getLgtType() const
{
	return g_mpLgtTyp[ptd_lgtD3D.Type];
}