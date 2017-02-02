#pragma once
#include "stdafx.h"
#include "FWManager.h"
#include "FWEffect.h"

SINGLE_IMPLEMENT(CFWEffMngr)

CFWEffMngr::CFWEffMngr()
{
	ptd_ayEffect[EFFECT_AMBIENT_LGT]	= CFWEffIntf(CFWEffAmbLgt::getInstance());
	ptd_ayEffect[EFFECT_SHOW_EDGE]		= CFWEffIntf(CFWEffShwEdg::getInstance());
	ptd_ayEffect[EFFECT_ADV_BOARD]		= CFWEffIntf(CFWEffAdvBd::getInstance());
	ptd_ayEffect[EFFECT_ALPHA]			= CFWEffIntf(CFWEffAlpha::getInstance());
	ptd_ayEffect[EFFECT_Z_DEPTH]		= CFWEffIntf(CFWEffZDepth::getInstance());
}

CFWEffIntf& CFWEffMngr::GetEffect(UINT uEffIdI)
{
	if(uEffIdI < EFFECT_UNKNOWN)
	{
		return ptd_ayEffect[uEffIdI];
	}
	return ptd_ayEffect[EFFECT_UNKNOWN];
}

bool CFWEffMngr::ChkEffAva(UINT uEffIdI) const
{
	return uEffIdI < EFFECT_UNKNOWN && ptd_ayEffect[uEffIdI].BeBound();
}

typedef std::map<string, UINT>::value_type VtpEffID;

BEG_INI_MAP( string, UINT, CFWEffMngr::EFFECT_UNKNOWN, g_ayEffTmp)

	VtpEffID( "AMBIENT_LGT",	CFWEffMngr::EFFECT_AMBIENT_LGT),
	VtpEffID( "SHOW_EDGE",		CFWEffMngr::EFFECT_SHOW_EDGE),
	VtpEffID( "ADV_BOARD",		CFWEffMngr::EFFECT_ADV_BOARD),
	VtpEffID( "ALPHA",			CFWEffMngr::EFFECT_ALPHA),
	VtpEffID( "Z_DEPTH",		CFWEffMngr::EFFECT_Z_DEPTH)

END_INI_MAP( string, UINT, CFWEffMngr::EFFECT_UNKNOWN, g_ayEffTmp, g_mpSptdEff);

UINT CFWEffMngr::CvtStrToEffID(string strEffI) const
{
	if(CHK_EXS_STL( g_mpSptdEff, strEffI))
	{
		return g_mpSptdEff[strEffI];
	}
	else	{ return EFFECT_UNKNOWN; }
}