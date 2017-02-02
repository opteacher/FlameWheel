#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWLight.h"
#include "FWLgtIntf.h"
#include "FWD3DMisc.h"
#include "FWPool.h"

SINGLE_IMPLEMENT(CFWLgtFcty)

typedef std::map<string, UINT>::value_type VtpLgt;

BEG_INI_MAP( string, UINT, CFWLgtIntf::LGT_TP_UNKNOW, g_ayLgtTmp)
	
	VtpLgt( "POINT",	CFWLgtIntf::LGT_TP_POINT),
	VtpLgt( "SPOT",		CFWLgtIntf::LGT_TP_SPOT),
	VtpLgt( "DIRECT",	CFWLgtIntf::LGT_TP_DIRECT)

END_INI_MAP( string, UINT, CFWLgtIntf::LGT_TP_UNKNOW, g_ayLgtTmp, g_mpLgtTyp);

DWORD g_mpD3DLgtTyp[] = 
{
	D3DLIGHTTYPE::D3DLIGHT_POINT,//LGT_TP_POINT
	D3DLIGHTTYPE::D3DLIGHT_SPOT,//LGT_TP_SPOT
	D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL//LGT_TP_DIRECT
};

CFWIntf& CFWLgtFcty::Create(CFWParam& pamI)
{
	CFWLight* pNewLgt = new CFWLight();

	// Fill in by parameter
	if(pamI.has("lgtTyp"))
	{
		string strLgtTyp = pamI.getString("lgtTyp");
		if(CHK_EXS_STL( g_mpLgtTyp, strLgtTyp))
		{
			UINT uLgtTyp = g_mpLgtTyp[strLgtTyp];
			if(uLgtTyp < CFWLgtIntf::LGT_TP_UNKNOW)
			{
				DWORD dwD3DTyp = g_mpD3DLgtTyp[uLgtTyp];
				pNewLgt->ptd_lgtD3D.Type = (D3DLIGHTTYPE)dwD3DTyp;
			}
		}
	}
	if(pamI.has("lgtDif"))	{ pNewLgt->ptd_lgtD3D.Diffuse = tunToD3DCol(pamI.getColor("lgtDif")); }
	if(pamI.has("lgtAmb"))	{ pNewLgt->ptd_lgtD3D.Ambient = tunToD3DCol(pamI.getColor("lgtAmb")); }
	if(pamI.has("lgtSpc"))	{ pNewLgt->ptd_lgtD3D.Specular = tunToD3DCol(pamI.getColor("lgtSpc")); }
	if(pamI.has("lgtPos"))	{ pNewLgt->ptd_lgtD3D.Position = CVT_POI_D3D(pamI.getQuate("lgtPos")); }
	if(pamI.has("lgtDir"))	{ pNewLgt->ptd_lgtD3D.Direction = CVT_POI_D3D(pamI.getQuate("lgtDir")); }
	if(pamI.has("lgtRng"))	{ pNewLgt->ptd_lgtD3D.Range = pamI.getFloat("lgtRng"); }
	if(pamI.has("lgtAtt0"))	{ pNewLgt->ptd_lgtD3D.Attenuation0 = pamI.getFloat("lgtAtt0"); }
	if(pamI.has("lgtAtt1"))	{ pNewLgt->ptd_lgtD3D.Attenuation0 = pamI.getFloat("lgtAtt1"); }
	if(pamI.has("lgtAtt2"))	{ pNewLgt->ptd_lgtD3D.Attenuation0 = pamI.getFloat("lgtAtt2"); }

	// Add new light into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pNewLgt);

	// Set new device to a device interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWLgtIntf(pNewLgt);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));
	if(pamI.has("lgtName"))
	{
		string strDevNam = pamI.getString("lgtName");
		ptd_mpCpdTbl.insert(VtpCpdTbl( strDevNam, dwHashID));
	}

	// Set light used ID and enable or not according parameter
	if(pamI.has("lgtID"))
	{
		pIntfTmp->TransferTo( CFWLgtIntf::LGT_STT_BE_SET, pamI);
	}
	if(pamI.has("enable"))
	{
		pIntfTmp->TransferTo( CFWLgtIntf::LGT_STT_BE_ENB, pamI);
	}

	// Return the reference of the device interface
	return *pIntfTmp;
}
