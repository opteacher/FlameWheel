#pragma once
#include "stdafx.h"
#include "FWDevice.h"
#include "FWFactory.h"
#include "FWCamIntf.h"
#include "FWD3DMisc.h"
#include "dxErr.h"

INI_TYPID(CFWDevice);

HRESULT CFWDevice::initialize()
{
	// Step 1: Create the IDirect3D9 object.
	if(!ptd_lpAdapter)
	{
		ptd_lpAdapter = ::Direct3DCreate9(D3D_SDK_VERSION);
		if(!ptd_lpAdapter)
		{
			::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
			return E_FAIL;
		}
	}

	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	ptd_lpAdapter->GetDeviceCaps(D3DADAPTER_DEFAULT, ptd_tpDev, &caps);
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//hard
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//soft
	}

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
	if(!ptd_pmPresent.hDeviceWindow)
	{
		ptd_pmPresent.hDeviceWindow = ::GetActiveWindow();
	}

	// Step 4: Create the device.
	HRESULT hr = ptd_lpAdapter->CreateDevice(
		D3DADAPTER_DEFAULT,
		ptd_tpDev,
		ptd_pmPresent.hDeviceWindow,
		vp,
		&ptd_pmPresent,
		&ptd_lpDevice);
	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		ptd_pmPresent.AutoDepthStencilFormat = D3DFMT_D16;

		hr = ptd_lpAdapter->CreateDevice(
			D3DADAPTER_DEFAULT,
			ptd_tpDev,
			ptd_pmPresent.hDeviceWindow,
			vp,
			&ptd_pmPresent,
			&ptd_lpDevice);

		if( FAILED(hr) )
		{
			ptd_lpAdapter->Release(); // done with ptd_lpAdapter object
			::MessageBox(0, L"createObj() - FAILED", 0, 0);
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CFWDevice::restore()
{
	// Check device state
	HRESULT hr = ptd_lpDevice->TestCooperativeLevel();

	// Device can be reset now
	if (SUCCEEDED(hr) || hr == D3DERR_DEVICENOTRESET)
	{
		// Release resource allocated as D3DPOOL_DEFAULT

		// Reset device
		HRESULT hr = ptd_lpDevice->Reset(&ptd_pmPresent);
		if (SUCCEEDED(hr))
		{
			CFWIntf* pIntf = CFWCamFcty::getInstance()->getOprCam();
			if(pIntf != NULL)
			{
				((CFWCamIntf*)pIntf)->TransferTo(CFWCamIntf::CAM_STT_SET_OPR);
			}
		}
		else
		{
			// Reset device failed, show error box
			const WCHAR* errorString = DXGetErrorString(hr);
			DXTRACE_ERR_MSGBOX(errorString, hr);
		}
	}
	// Device is still in lost state, wait
	else if (hr == D3DERR_DEVICELOST)	{ Sleep(25); }
	else
	{
		// Other error, Show error box
		const WCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX( errorString, hr);
	}
	return hr;
}

void CFWDevice::destory()
{
	SAFE_RELEASE(ptd_lpAdapter);
	SAFE_RELEASE(ptd_lpSwapChain);
	SAFE_RELEASE(ptd_lpDevice);
}

HRESULT CFWDevice::begPaint()
{
	if(ptd_lpDevice)
	{
		HRESULT hr	= S_OK;
		//refresh the screen
		hr &= ptd_lpDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COL_TO_DWD(ptd_colBackGd), 1.0f, 0);
		//begin scene
		hr &= ptd_lpDevice->BeginScene();

		return hr;
	}
	return E_FAIL;
}

HRESULT CFWDevice::endPaint()
{
	if(ptd_lpDevice)
	{
		HRESULT hr	= S_OK;
		//end scene
		hr	&= ptd_lpDevice->EndScene();
		//refresh
		hr	&= ptd_lpDevice->Present(0, 0, 0, 0);
		//failed end the paint, restore the device
		if(FAILED(hr))	{ restore(); }

		return hr;
	}
	return E_FAIL;
}

CMatrix CFWDevice::setWldMat(CMatrix matI)
{
	//check initialization and real pointer
	if(!ptd_lpDevice)	{ return CMatrix(); }

	//convert to D3D matrix
	D3DXMATRIXA16 matTrsf((float*)matI.m_fVal);
	D3DXMATRIXA16 matOrig;
		
	//do transform
	ptd_lpDevice->GetTransform( D3DTS_WORLD, &matOrig);
	ptd_lpDevice->SetTransform( D3DTS_WORLD, &matTrsf);

	return CMatrix(matOrig.m);
}

CMatrix CFWDevice::getWldMat() const
{
	//check initialization and real pointer
	if(!ptd_lpDevice)	{ return CMatrix(); }

	//convert to D3D matrix
	D3DXMATRIXA16 matCurTsf;
		
	//do transform
	ptd_lpDevice->GetTransform( D3DTS_WORLD, &matCurTsf);

	return CMatrix(matCurTsf.m);
}

void CFWDevice::setViewMat(CMatrix matI)
{
	//check initialization and real pointer
	if(!ptd_lpDevice)	{ return; }

	//convert to D3D matrix
	D3DXMATRIX matView((FLOAT*)matI.m_fVal);
		
	//do transform
	ptd_lpDevice->SetTransform( D3DTS_VIEW, &matView);
}

void CFWDevice::setProjMat(CMatrix matI)
{
	//check initialization and real pointer
	if(!ptd_lpDevice)	{ return; }

	//convert to D3D matrix
	D3DXMATRIX matProj((FLOAT*)matI.m_fVal);

	//do transform
	ptd_lpDevice->SetTransform( D3DTS_PROJECTION, &matProj);
}

SFWColor CFWDevice::getBkGrdCol() const
{
	return SFWColor(
		ptd_colBackGd.r,
		ptd_colBackGd.g,
		ptd_colBackGd.b,
		ptd_colBackGd.a);
}