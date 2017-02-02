#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWDevice.h"
#include "FWDevIntf.h"
#include "FWPool.h"
#include "FWD3DMisc.h"
#include "../OPTools/String.h"

SINGLE_IMPLEMENT(CFWDevFcty)

CFWIntf& CFWDevFcty::Create(CFWParam& pamI)
{
	// Create a new device according user's required
	CFWDevice* pObjCrte = new CFWDevice();

	// Set the new device with parameters
	pObjCrte->ptd_tpDev = (D3DDEVTYPE)switchDevTypFromPam(pamI.getString("devType"));
	pObjCrte->ptd_colBackGd = tunToD3DCol(pamI.getColor("backGdCol"));
	pObjCrte->ptd_pmPresent.BackBufferWidth = pamI.getUint("width");
	pObjCrte->ptd_pmPresent.BackBufferHeight = pamI.getUint("height");
	pObjCrte->ptd_pmPresent.BackBufferFormat = (D3DFORMAT)switchVwFmtFromPam(pamI.getString("backFmt"));
	pObjCrte->ptd_pmPresent.BackBufferCount = pamI.getUint("backCount");
	pObjCrte->ptd_pmPresent.MultiSampleQuality = pamI.getDword("multiSpl");
	pObjCrte->ptd_pmPresent.MultiSampleType = (D3DMULTISAMPLE_TYPE)pObjCrte->ptd_pmPresent.MultiSampleQuality;
	pObjCrte->ptd_pmPresent.SwapEffect = (D3DSWAPEFFECT)switchSwapEffcFromPam(pamI.getString("swapEffc"));
	pObjCrte->ptd_pmPresent.Windowed = pamI.getBool("beWindow");
	pObjCrte->ptd_pmPresent.EnableAutoDepthStencil = pamI.getBool("enableAutoDepth");
	pObjCrte->ptd_pmPresent.AutoDepthStencilFormat = (D3DFORMAT)switchVwFmtFromPam(pamI.getString("autoDepthFmt"));
	pObjCrte->ptd_pmPresent.Flags = pamI.getDword("flags");
	pObjCrte->ptd_pmPresent.FullScreen_RefreshRateInHz = pamI.getUint("fullScnRefsRate");
	pObjCrte->ptd_pmPresent.PresentationInterval = switchPresIntervalFromPam(pamI.getString("presInterval"));
	pObjCrte->ptd_pmPresent.hDeviceWindow = (HWND)pamI.getDword("wndHdl");

	// Add new device into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pObjCrte);

	// Set new device to a device interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWDevIntf(pObjCrte);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));
	if(pamI.has("devName"))
	{
		string strDevNam = pamI.getString("devName");
		ptd_mpCpdTbl.insert(VtpCpdTbl( strDevNam, dwHashID));
	}

	// Set as current device if it's needed
	if(pamI.has("setCurrent") && pamI.getBool("setCurrent"))
	{
		ptd_dwCurDev = dwHashID;
	}

	// Return the reference of the device interface
	return *pIntfTmp;
}

DWORD CFWDevFcty::switchDevTypFromPam(string strPamI)
{
	return (OPTools::cmpStrIgnSize( strPamI, "hal")) ? \
		D3DDEVTYPE::D3DDEVTYPE_HAL : \
		D3DDEVTYPE::D3DDEVTYPE_REF;
}


typedef std::map<string, D3DFORMAT>::value_type VtpVwFmt;
VtpVwFmt s_ayD3DFmt[] = 
{
	VtpVwFmt("UNKNOWN",            D3DFORMAT::D3DFMT_UNKNOWN),
	VtpVwFmt("R8G8B8",             D3DFORMAT::D3DFMT_R8G8B8),
	VtpVwFmt("A8R8G8B8",           D3DFORMAT::D3DFMT_A8R8G8B8),
	VtpVwFmt("X8R8G8B8",           D3DFORMAT::D3DFMT_X8R8G8B8),
	VtpVwFmt("R5G6B5",             D3DFORMAT::D3DFMT_R5G6B5),
	VtpVwFmt("X1R5G5B5",           D3DFORMAT::D3DFMT_X1R5G5B5),
	VtpVwFmt("A1R5G5B5",           D3DFORMAT::D3DFMT_A1R5G5B5),
	VtpVwFmt("A4R4G4B4",           D3DFORMAT::D3DFMT_A4R4G4B4),
	VtpVwFmt("R3G3B2",             D3DFORMAT::D3DFMT_R3G3B2),
	VtpVwFmt("A8",                 D3DFORMAT::D3DFMT_A8),
	VtpVwFmt("A8R3G3B2",           D3DFORMAT::D3DFMT_A8R3G3B2),
	VtpVwFmt("X4R4G4B4",           D3DFORMAT::D3DFMT_X4R4G4B4),
	VtpVwFmt("A2B10G10R10",        D3DFORMAT::D3DFMT_A2B10G10R10),
	VtpVwFmt("A8B8G8R8",           D3DFORMAT::D3DFMT_A8B8G8R8),
	VtpVwFmt("X8B8G8R8",           D3DFORMAT::D3DFMT_X8B8G8R8),
	VtpVwFmt("G16R16",             D3DFORMAT::D3DFMT_G16R16),
	VtpVwFmt("A2R10G10B10",        D3DFORMAT::D3DFMT_A2R10G10B10),
	VtpVwFmt("A16B16G16R16",       D3DFORMAT::D3DFMT_A16B16G16R16),
	VtpVwFmt("A8P8",               D3DFORMAT::D3DFMT_A8P8),
	VtpVwFmt("P8",                 D3DFORMAT::D3DFMT_P8),
	VtpVwFmt("L8",                 D3DFORMAT::D3DFMT_L8),
	VtpVwFmt("A8L8",               D3DFORMAT::D3DFMT_A8L8),
	VtpVwFmt("A4L4",               D3DFORMAT::D3DFMT_A4L4),
	VtpVwFmt("V8U8",               D3DFORMAT::D3DFMT_V8U8),
	VtpVwFmt("L6V5U5",             D3DFORMAT::D3DFMT_L6V5U5),
	VtpVwFmt("X8L8V8U8",           D3DFORMAT::D3DFMT_X8L8V8U8),
	VtpVwFmt("Q8W8V8U8",           D3DFORMAT::D3DFMT_Q8W8V8U8),
	VtpVwFmt("V16U16",             D3DFORMAT::D3DFMT_V16U16),
	VtpVwFmt("A2W10V10U10",        D3DFORMAT::D3DFMT_A2W10V10U10),
	VtpVwFmt("UYVY",               D3DFORMAT::D3DFMT_UYVY),
	VtpVwFmt("R8G8_B8G8",          D3DFORMAT::D3DFMT_R8G8_B8G8),
	VtpVwFmt("YUY2",               D3DFORMAT::D3DFMT_YUY2),
	VtpVwFmt("G8R8_G8B8",          D3DFORMAT::D3DFMT_G8R8_G8B8),
	VtpVwFmt("DXT1",               D3DFORMAT::D3DFMT_DXT1),
	VtpVwFmt("DXT2",               D3DFORMAT::D3DFMT_DXT2),
	VtpVwFmt("DXT3",               D3DFORMAT::D3DFMT_DXT3),
	VtpVwFmt("DXT4",               D3DFORMAT::D3DFMT_DXT4),
	VtpVwFmt("DXT5",               D3DFORMAT::D3DFMT_DXT5),
	VtpVwFmt("D16_LOCKABLE",       D3DFORMAT::D3DFMT_D16_LOCKABLE),
	VtpVwFmt("D32",                D3DFORMAT::D3DFMT_D32),
	VtpVwFmt("D15S1",              D3DFORMAT::D3DFMT_D15S1),
	VtpVwFmt("D24S8",              D3DFORMAT::D3DFMT_D24S8),
	VtpVwFmt("D24X8",              D3DFORMAT::D3DFMT_D24X8),
	VtpVwFmt("D24X4S4",            D3DFORMAT::D3DFMT_D24X4S4),
	VtpVwFmt("D16",                D3DFORMAT::D3DFMT_D16),
	VtpVwFmt("D32F_LOCKABLE",      D3DFORMAT::D3DFMT_D32F_LOCKABLE),
	VtpVwFmt("D24FS8",             D3DFORMAT::D3DFMT_D24FS8),
	VtpVwFmt("D32_LOCKABLE",       D3DFORMAT::D3DFMT_D32_LOCKABLE),
	VtpVwFmt("S8_LOCKABLE",        D3DFORMAT::D3DFMT_S8_LOCKABLE),
	VtpVwFmt("L16",                D3DFORMAT::D3DFMT_L16),
	VtpVwFmt("VERTEXDATA",         D3DFORMAT::D3DFMT_VERTEXDATA),
	VtpVwFmt("INDEX16",            D3DFORMAT::D3DFMT_INDEX16),
	VtpVwFmt("INDEX32",            D3DFORMAT::D3DFMT_INDEX32),
	VtpVwFmt("Q16W16V16U16",       D3DFORMAT::D3DFMT_Q16W16V16U16),
	VtpVwFmt("MULTI2_ARGB8",       D3DFORMAT::D3DFMT_MULTI2_ARGB8),
	VtpVwFmt("R16F",               D3DFORMAT::D3DFMT_R16F),
	VtpVwFmt("G16R16F",            D3DFORMAT::D3DFMT_G16R16F),
	VtpVwFmt("A16B16G16R16F",      D3DFORMAT::D3DFMT_A16B16G16R16F),
	VtpVwFmt("R32F",               D3DFORMAT::D3DFMT_R32F),
	VtpVwFmt("G32R32F",            D3DFORMAT::D3DFMT_G32R32F),
	VtpVwFmt("A32B32G32R32F",      D3DFORMAT::D3DFMT_A32B32G32R32F),
	VtpVwFmt("CxV8U8",             D3DFORMAT::D3DFMT_CxV8U8),
	VtpVwFmt("A1",                 D3DFORMAT::D3DFMT_A1),
	VtpVwFmt("A2B10G10R10_XR_BIAS",D3DFORMAT::D3DFMT_A2B10G10R10_XR_BIAS),
	VtpVwFmt("BINARYBUFFER",       D3DFORMAT::D3DFMT_BINARYBUFFER),
	VtpVwFmt("FORCE_DWORD",        D3DFORMAT::D3DFMT_FORCE_DWORD)
};
const std::map<string, D3DFORMAT> s_mpD3DFmt( s_ayD3DFmt, s_ayD3DFmt + 66);

DWORD CFWDevFcty::switchVwFmtFromPam(string strPamI)
{
	std::map<string, D3DFORMAT>::const_iterator iterFnd = s_mpD3DFmt.find(strPamI);
	return (iterFnd != s_mpD3DFmt.end()) ? iterFnd->second : D3DFORMAT::D3DFMT_A8R8G8B8;
}

typedef std::map<string, D3DSWAPEFFECT>::value_type VtpSwpEff;
VtpSwpEff s_aySwpEff[] =
{
	VtpSwpEff("DISCARD",     D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD),
	VtpSwpEff("FLIP",        D3DSWAPEFFECT::D3DSWAPEFFECT_FLIP),
	VtpSwpEff("COPY",        D3DSWAPEFFECT::D3DSWAPEFFECT_COPY),
	VtpSwpEff("OVERLAY",     D3DSWAPEFFECT::D3DSWAPEFFECT_OVERLAY),
	VtpSwpEff("FLIPEX",      D3DSWAPEFFECT::D3DSWAPEFFECT_FLIPEX),
	VtpSwpEff("FORCE_DWORD", D3DSWAPEFFECT::D3DSWAPEFFECT_FORCE_DWORD)
};
const std::map<string, D3DSWAPEFFECT> s_mpSwpEff( s_aySwpEff, s_aySwpEff + 6);

DWORD CFWDevFcty::switchSwapEffcFromPam(string strPamI)
{
	std::map<string, D3DSWAPEFFECT>::const_iterator iterFnd = s_mpSwpEff.find(strPamI);
	return (iterFnd != s_mpSwpEff.end()) ? iterFnd->second : D3DSWAPEFFECT_DISCARD;
}

typedef std::map<string, UINT>::value_type VtpPresInt;
VtpPresInt s_ayPresInt[] =
{
	VtpPresInt("DEFAULT",   D3DPRESENT_INTERVAL_DEFAULT),
	VtpPresInt("ONE",       D3DPRESENT_INTERVAL_ONE),
	VtpPresInt("TWO",       D3DPRESENT_INTERVAL_TWO),
	VtpPresInt("THREE",     D3DPRESENT_INTERVAL_THREE),
	VtpPresInt("FOUR",      D3DPRESENT_INTERVAL_FOUR),
	VtpPresInt("IMMEDIATE", D3DPRESENT_INTERVAL_IMMEDIATE)
};
const std::map<string, UINT> s_mpPresInt( s_ayPresInt, s_ayPresInt + 6);

UINT CFWDevFcty::switchPresIntervalFromPam(string strPamI)
{
	std::map<string, UINT>::const_iterator iterFnd = s_mpPresInt.find(strPamI);
	return (iterFnd != s_mpPresInt.end()) ? iterFnd->second : D3DPRESENT_INTERVAL_IMMEDIATE;
}
