#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWSkin.h"
#include "FWSknIntf.h"
#include "FWPool.h"

SINGLE_IMPLEMENT(CFWSknFcty)

CFWIntf& CFWSknFcty::Create(CFWParam& pamI)
{
	// Collect information from parameter
	SFWMaterial mtlFmPam;
	if(pamI.has("mtlDif"))	{ mtlFmPam = SFWMaterial(pamI.getColor("mtlDif")); }
	if(pamI.has("mtlAmb"))	{ mtlFmPam.m_colAmbient = pamI.getColor("mtlAmb"); }
	if(pamI.has("mtlSpc"))	{ mtlFmPam.m_colSpecular = pamI.getColor("mtlSpc"); }
	if(pamI.has("mtlEms"))	{ mtlFmPam.m_colEmissive = pamI.getColor("mtlEms"); }
	if(pamI.has("mtlPwr"))	{ mtlFmPam.m_fPower = pamI.getFloat("mtlPwr"); }

	std::map<UINT, string> mpTexFmPam;
	typedef std::map<UINT, string>::value_type VtpTexFl;
	if(pamI.has("difTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_DI, pamI.getString("difTex"))); }
	if(pamI.has("ambTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_AM, pamI.getString("ambTex"))); }
	if(pamI.has("spcTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_SP, pamI.getString("spcTex"))); }
	if(pamI.has("obcTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_OP, pamI.getString("obcTex"))); }
	if(pamI.has("bmpTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_DP, pamI.getString("bmpTex"))); }
	if(pamI.has("relTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_RL, pamI.getString("relTex"))); }
	if(pamI.has("refTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_RR, pamI.getString("refTex"))); }
	if(pamI.has("dspTex"))	{ mpTexFmPam.insert(VtpTexFl( CFWSknIntf::TEX_DP, pamI.getString("dspTex"))); }

	// Check required skin whether existed
	CFWIntf* pSknRet = ChkExs( mtlFmPam, mpTexFmPam);
	if(pSknRet != NULL)	{ return *pSknRet; }

	// Create a new skin according user's required
	CFWSkin* pCrteObj = new CFWSkin();

	// Get material from parameter, and set into new skin
	
	pCrteObj->setMtl(mtlFmPam);

	// Get each textures' file path
	typedef std::map<UINT, string>::iterator ItrTexFl;
	for( ItrTexFl iter = mpTexFmPam.begin(); iter != mpTexFmPam.end(); ++iter)
	{
		pCrteObj->setTex( iter->second, iter->first);
	}

	// Add new skin into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pCrteObj);

	// Set new skin to a skin interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWSknIntf(pCrteObj);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));

	// Return the reference of the skin interface
	return *pIntfTmp;
}

CFWIntf* CFWSknFcty::ChkExs( const SFWMaterial& mtlI, std::map<UINT, string> mpTexI) const
{
	for( CiterIntf iter = ptd_mpIntfs.begin(); iter != ptd_mpIntfs.end(); ++iter)
	{
		CFWSknIntf* pSkn = (CFWSknIntf*)iter->second;
		if(pSkn->GetMtrl() != mtlI)	{ continue; }

		bool bTexFit = true;
		typedef std::map<UINT, string>::iterator ItrTexFl;
		for( ItrTexFl i = mpTexI.begin(); i != mpTexI.end(); ++i)
		{
			if(i->second != pSkn->GetTexFile(i->first))
			{
				bTexFit = false;
				break;
			}
		}
		if(bTexFit)	{ return pSkn; }
	}
	return NULL;
}

CFWParam CFWSknFcty::MrgPam( CFWParam& pamBasIO, CFWParam pamSubI, bool bCvrBasI)
{
	std::vector<string> aySubKey = pamSubI.getKeySet();
	for( UINT i = 0; i < aySubKey.size(); ++i)
	{
		string strKey = aySubKey[i];
		string strKeyHd = strKey.substr( 0, 6);
		if(strKeyHd == "mtlDif"
		|| strKeyHd == "mtlAmb"
		|| strKeyHd == "mtlSpc"
		|| strKeyHd == "mtlEms"
		|| strKeyHd == "mtlPwr"
		|| strKeyHd == "difTex"
		|| strKeyHd == "ambTex"
		|| strKeyHd == "spcTex"
		|| strKeyHd == "obcTex"
		|| strKeyHd == "bmpTex"
		|| strKeyHd == "relTex"
		|| strKeyHd == "refTex"
		|| strKeyHd == "dspTex")
		{
			if(pamBasIO.has(strKey) && bCvrBasI)
			{
				pamBasIO.upd( strKey, pamSubI.getString(strKey));
			}
			else
			{
				pamBasIO.add( strKey, pamSubI.getString(strKey));
			}
		}
	}
	return pamBasIO;
}