#pragma once
#include "stdafx.h"
#include "FWMaterial.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
using OPTools::strToNum;

bool SFWMaterial::operator==(const SFWMaterial& mtlI) const
{
	return m_colDiffuse == mtlI.m_colDiffuse
		&& m_colAmbient == mtlI.m_colAmbient
		&& m_colSpecular == mtlI.m_colSpecular
		&& m_colEmissive == mtlI.m_colEmissive
		&& m_fPower == mtlI.m_fPower;
}

bool SFWMaterial::operator!=(const SFWMaterial& mtlI) const
{
	return m_colDiffuse != mtlI.m_colDiffuse
		|| m_colAmbient != mtlI.m_colAmbient
		|| m_colSpecular != mtlI.m_colSpecular
		|| m_colEmissive != mtlI.m_colEmissive
		|| m_fPower != mtlI.m_fPower;
}

SFWMaterial SFWMaterial::getNegMtl() const
{
	SFWMaterial mtlRet;
	mtlRet.m_colDiffuse = m_colDiffuse.getNegCol();
	mtlRet.m_colAmbient = m_colAmbient.getNegCol();
	mtlRet.m_colSpecular = m_colSpecular.getNegCol();
	//mtlRet.m_colEmissive = m_colEmissive.getNegCol();
	return mtlRet;
}

string SFWMaterial::convertToStr() const
{
	string strRet = "";
	strRet += "D[" + m_colDiffuse.convertToStr() + "]";
	strRet += "A[" + m_colAmbient.convertToStr() + "]";
	strRet += "S[" + m_colSpecular.convertToStr() + "]";
	strRet += "E[" + m_colEmissive.convertToStr() + "]";
	strRet += "P" + numToStr<float>(m_fPower);
	return strRet;
}
	
SFWMaterial& SFWMaterial::convertFromStr(string strI)
{
	string::size_type stpOff = 0;
	stpOff = collectColFromStr( strI, stpOff, "D", m_colDiffuse);
	stpOff = collectColFromStr( strI, stpOff, "A", m_colAmbient);
	stpOff = collectColFromStr( strI, stpOff, "S", m_colSpecular);
	stpOff = collectColFromStr( strI, stpOff, "E", m_colEmissive);
	
	stpOff = strI.find( 'P', stpOff);
	if(stpOff != string::npos)	{ m_fPower = strToNum<float>(strI.substr(stpOff)); }

	return *this;
}

bool SFWMaterial::chkStrCanCvt(string strI)
{
	SFWColor colTmp;
	string::size_type stpOff = 0;
	if(collectColFromStr( strI, stpOff, "D", colTmp) == string::npos)	{ return false; }
	if(collectColFromStr( strI, stpOff, "A", colTmp) == string::npos)	{ return false; }
	if(collectColFromStr( strI, stpOff, "S", colTmp) == string::npos)	{ return false; }
	if(collectColFromStr( strI, stpOff, "E", colTmp) == string::npos)	{ return false; }

	stpOff = strI.find( 'P', stpOff);
	if(stpOff == string::npos)	{ return false; }
	else
	{
		return OPTools::chkNum(strI.substr(stpOff));
	}
}

string::size_type SFWMaterial::collectColFromStr( string strI, string::size_type& rFndOff, string strColI, SFWColor& rCol)
{
	string::size_type stpTmp = strI.find( strColI + '[', rFndOff);
	if(stpTmp != string::npos)
	{
		stpTmp += 2;
		rFndOff = stpTmp;
		if(rFndOff >= strI.length())	{ return string::npos; }
		
		rFndOff = strI.find( ']', rFndOff);
		if(rFndOff == string::npos)		{ return string::npos; }

		string strCol = strI.substr(GET_SUB_STR( stpTmp, rFndOff - 1));
		rCol = SFWColor().convertFromStr(strCol);
		if(!rCol.beSetCorrect())		{ return string::npos; }
	}

	return rFndOff;
}