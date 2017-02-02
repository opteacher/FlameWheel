#pragma once
#include "stdafx.h"
#include "FWMesh.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWMshIntf.h"

INI_TYPID(CFWPrim);

HRESULT CFWPrim::initialize(CFWParam pamI)
{
	// Get current device
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pCurDev == NULL || pCurDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pCurDev->GetImpDev();

	// Create vertex buffer
	if(FAILED(pD3DDev->CreateVertexBuffer(
		ptd_dwNumVtx * ptd_dwPerVtx,
		0,//Vertex buffer's attribute
		ptd_dwFVF,
		D3DPOOL_DEFAULT,
		&ptd_lpVtxBuf,
		NULL)))	{ return E_FAIL; }

	// Collect vertexes' data from parameter
	SFWVertex* ayPois = (SFWVertex*)pamI.getDword("aryPois");

	LPBYTE pVtxBuf = NULL;
	if(FAILED(ptd_lpVtxBuf->Lock( 0, ptd_dwNumVtx, (LPVOID*)&pVtxBuf, 0)))	{ return E_FAIL; }
	const DWORD dwSzVtx = sizeof(SFWVertex);
	const DWORD dwSzCol = sizeof(DWORD);
	DWORD dwVtxCol = COL_TO_DWD(ptd_colVtx);
	for( DWORD i = 0; i < ptd_dwNumVtx; ++i)
	{
		// Vertexes' location information input into memory
		memcpy( pVtxBuf, &ayPois[i], dwSzVtx); pVtxBuf += dwSzVtx;

		// Check vertexes' color existed
		if(ptd_dwFVF & D3DFVF_DIFFUSE)
		{
			memcpy( pVtxBuf, &dwVtxCol, dwSzCol); pVtxBuf += dwSzCol;
		}
	}
	if(FAILED(ptd_lpVtxBuf->Unlock()))	{ return E_FAIL; }

	return S_OK;
}

HRESULT CFWPrim::draw(UINT uSubIdI)
{
	// Get current device
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pCurDev == NULL || pCurDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pCurDev->GetImpDev();

	// Draw primitive
	if(FAILED(pD3DDev->SetStreamSource( 0, ptd_lpVtxBuf, 0, ptd_dwPerVtx)))	{ return E_FAIL; }
	DWORD dwFmrFVF;
	if(FAILED(pD3DDev->GetFVF(&dwFmrFVF)))	{ return E_FAIL; }
	if(FAILED(pD3DDev->SetFVF(ptd_dwFVF)))	{ return E_FAIL; }
	switch (ptd_uPrimTyp)
	{
	case CFWMshIntf::PRIM_TP_POINT:
		if(FAILED(pD3DDev->DrawPrimitive( D3DPT_POINTLIST, 0, ptd_dwNumVtx)))		{ return E_FAIL; }
		break;
	case CFWMshIntf::PRIM_TP_LINE:
		if(FAILED(pD3DDev->DrawPrimitive( D3DPT_LINESTRIP, 0, 1)))					{ return E_FAIL; }
		break;
	case CFWMshIntf::PRIM_TP_LN_LST:
		if(FAILED(pD3DDev->DrawPrimitive( D3DPT_LINELIST, 0, (UINT)(ptd_dwNumVtx * 0.5f))))	{ return E_FAIL; }
		break;
	case CFWMshIntf::PRIN_TP_TRIANGLE:
		if(FAILED(pD3DDev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, ptd_dwNumVtx/3)))	{ return E_FAIL; }
		break;
	}
	if(FAILED(pD3DDev->SetFVF(dwFmrFVF)))	{ return E_FAIL; }

	return S_OK;
}