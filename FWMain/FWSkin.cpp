#pragma once
#include "stdafx.h"
#include "FWSkin.h"
#include "FWFactory.h"
#include "FWD3DMisc.h"
#include "FWDevIntf.h"
#include <map>
#include "../OPTools/String.h"

INI_TYPID(CFWSkin);

CFWSkin::CFWSkin()
{
	ptd_mtrl.Diffuse = tunToD3DCol(SFWColor::geneRandCol());
	ptd_mtrl.Ambient = ptd_mtrl.Diffuse;
	ptd_mtrl.Specular = tunToD3DCol(g_colWhite);
	ptd_mtrl.Emissive = tunToD3DCol(g_colBlack);
	ptd_mtrl.Power = 1.0f;
	for( int i = 0; i < MAX_TEX; ++i)	{ ptd_ayTex[i] = NULL; }
	ptd_dwNumTex = 0;
}

HRESULT CFWSkin::actSkin()
{
	// Get current device
	CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL && pIntf->GetStatus() == CFWDevIntf::DEV_STT_PAINTING)	{ return E_FAIL; }
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)((CFWDevIntf*)pIntf)->GetImpDev();

	// Act material and textures
	if(FAILED(pD3DDev->SetMaterial(&ptd_mtrl)))	{ return E_FAIL; }
	for( UINT i = 0; i < MAX_TEX; ++i)
	{
		if(ptd_ayTex[i] == NULL)	{ continue; }
		else if(FAILED(pD3DDev->SetTexture( i, ptd_ayTex[i])))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

void CFWSkin::destroy()
{
	for( UINT i = 0; i < ptd_dwNumTex; ++i)
	{
		SAFE_RELEASE(ptd_ayTex[i]);
	}
}

void CFWSkin::setMtl( const SFWMaterial& rMtrlI )
{
	ptd_mtrl = tunToD3DMtrl(rMtrlI);
}

SFWMaterial CFWSkin::getMtl() const
{
	return tunToFWMtrl(ptd_mtrl);
}

void CFWSkin::setTex( string strImgFileI, UINT uChnlI )
{
	// Get current device
	CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL && pIntf->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)	{ return; }
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)((CFWDevIntf*)pIntf)->GetImpDev();

	// Check channel whether available
	if(uChnlI >= MAX_TEX)	{ return; }

	if(ptd_ayTexFile[uChnlI].empty())	{ ++ptd_dwNumTex; }
	ptd_ayTexFile[uChnlI] = strImgFileI;

	D3DXIMAGE_INFO infoImg;
	WCHAR* wstrFlNam = OPTools::CharToWChar(strImgFileI.c_str());
	if(FAILED(D3DXGetImageInfoFromFile( wstrFlNam, &infoImg)))	{ return; }

	switch(infoImg.ResourceType)
	{
	case D3DRTYPE_TEXTURE:
		{
			IDirect3DTexture9* pTex = NULL;
			if(SUCCEEDED( D3DXCreateTextureFromFile( pD3DDev, wstrFlNam, &pTex)))
			{
				pTex->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&ptd_ayTex[uChnlI]);
				pTex->Release();
			}
		} break;
	case D3DRTYPE_CUBETEXTURE:
		{
			IDirect3DCubeTexture9* pTex;
			if(SUCCEEDED( D3DXCreateCubeTextureFromFile( pD3DDev, wstrFlNam, &pTex)))
			{
				pTex->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&ptd_ayTex[uChnlI]);
				pTex->Release();
			}
		} break;
	case D3DRTYPE_VOLUMETEXTURE:
		{
			IDirect3DVolumeTexture9* pTex;
			if(SUCCEEDED( D3DXCreateVolumeTextureFromFile( pD3DDev, wstrFlNam, &pTex)))
			{
				pTex->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&ptd_ayTex[uChnlI]);
				pTex->Release();
			}
		} break;
	}
}

string CFWSkin::getTex(UINT uChnlI) const
{
	if(uChnlI >= MAX_TEX)	{ return ""; }
	return ptd_ayTexFile[uChnlI];
}

HRESULT CFWSkin::clsFmBlend()
{
	// Get current device
	CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
	if(pIntf == NULL && pIntf->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)	{ return E_FAIL; }
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)((CFWDevIntf*)pIntf)->GetImpDev();

	for( UINT i = 0; i < MAX_TEX; ++i)
	{
		if(ptd_ayTex[i] != NULL)
		{
			if(FAILED(pD3DDev->SetTexture( i, NULL)))	{ return E_FAIL; }
		}
	}

	return S_OK;
}