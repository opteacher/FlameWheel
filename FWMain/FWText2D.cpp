#pragma once
#include "stdafx.h"
#include "FWText.h"
#include "FWDevIntf.h"
#include "FWTxtIntf.h"
#include "FWFactory.h"
#include "FWD3DMisc.h"
#include "../OPTools/String.h"
using OPTools::CharToWChar;

INI_TYPID(CFWText);
INI_TYPID(CFWText2D);

HRESULT CFWText2D::initialize(CFWParam pamI)
{
	// Get current device for display window and background color
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pCurDev == NULL || pCurDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pCurDev->GetImpDev();

	// Get font name for parameter and create font
	string strFont = pamI.has("fontName") ? pamI.getString("fontName") : "Arial";
	return D3DXCreateFont( pD3DDev, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		CharToWChar(strFont.c_str()), &ptd_pFont);
}

HRESULT CFWText2D::destroy()
{
	SAFE_RELEASE(ptd_pFont);
	return S_OK;
}

VTP_INI_MAP( UINT, UINT);
BEG_INI_MAP( UINT, UINT, 9, g_ayLocInRect)
	VtpKeyVal( CFWTxtIntf::LFT_TOP, DT_LEFT | DT_TOP),
	VtpKeyVal( CFWTxtIntf::TOP_CEN, DT_TOP | DT_CENTER),
	VtpKeyVal( CFWTxtIntf::RGT_TOP, DT_RIGHT | DT_TOP),
	VtpKeyVal( CFWTxtIntf::LFT_CEN, DT_LEFT | DT_CENTER),
	VtpKeyVal( CFWTxtIntf::CENTER, DT_CENTER),
	VtpKeyVal( CFWTxtIntf::RGT_CEN, DT_RIGHT | DT_CENTER),
	VtpKeyVal( CFWTxtIntf::LFT_BTM, DT_LEFT | DT_BOTTOM),
	VtpKeyVal( CFWTxtIntf::BTM_CEN, DT_BOTTOM | DT_CENTER),
	VtpKeyVal( CFWTxtIntf::RGT_BTM, DT_RIGHT | DT_BOTTOM)
END_INI_MAP( UINT, UINT, 9, g_ayLocInRect, g_mpLocInRect);

HRESULT CFWText2D::draw(CFWParam pamI)
{
	if(ptd_pFont == NULL)	{ return E_FAIL; }
	// Get display text from parameter
	string strShow = pamI.getString("showTxt");

	// Get current device for display window and background color
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(pCurDev == NULL || pCurDev->GetStatus() < CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}

	// Get out surround rectangle. If it has not been given, engine will use screen rectangle
	RECT rectShow;
	if(pamI.has("outSrdRect"))
	{
		CFWAABBBox boxPam = pamI.getAABBBox("outSrdRect");
		CPoint poiLT = boxPam.getPoiLTF();
		CPoint poiRB = boxPam.getPoiRBB();
		rectShow.left = min( poiLT.m_fX, poiRB.m_fX);
		rectShow.right = max( poiLT.m_fX, poiRB.m_fX);
		rectShow.top = min( poiLT.m_fY, poiRB.m_fY);
		rectShow.bottom = max( poiLT.m_fY, poiRB.m_fY);
	}
	else
	{
		::GetWindowRect( pCurDev->GetDispWnd(), &rectShow);
	}

	// Generate display mode from parameter
	DWORD dwFmt = DT_SINGLELINE;
	if(pamI.has("dockInRect"))
	{
		dwFmt |= pamI.getDword("dockInRect");
	}
	if(pamI.has("beCut") && !pamI.getBool("beCut"))
	{
		dwFmt |= DT_CALCRECT | DT_NOCLIP;
	}
	if(pamI.has("ignWrap") && !pamI.getBool("ignWrap"))
	{
		if((dwFmt & DT_BOTTOM) == 0)
		{
			dwFmt &= ~DT_SINGLELINE;
		}
	}

	// Get text's color from parameter, or use the negative color of background color
	SFWColor colTxt = pamI.has("textCol") ? pamI.getColor("textCol") : pCurDev->GetBkGrdCol().getNegCol();

	// Do draw text
	ptd_pFont->DrawTextA(
		NULL,
		strShow.c_str(),
		strShow.length() + 1,
		&rectShow,
		dwFmt,
		COL_TO_DWD(tunToD3DCol(colTxt)));
	return S_OK;
}