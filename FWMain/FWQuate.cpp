#pragma once
#include "stdafx.h"
#include "FWMisc.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
using OPTools::strToNum;
using OPTools::chkNum;

string SFWQuate::convertToStr() const
{
	string strRet = "";
	strRet += "X" + numToStr<float>(m_fX);
	strRet += "Y" + numToStr<float>(m_fY);
	strRet += "Z" + numToStr<float>(m_fZ);
	strRet += "H" + numToStr<float>(m_fH);
	return strRet;
}

SFWQuate& SFWQuate::convertFromStr( string strI )
{
	// Locate XYZH parts
	UINT uLocX = strI.find('X');
	UINT uLocY = strI.find( 'Y', uLocX);
	UINT uLocZ = strI.find( 'Z', uLocY);
	UINT uLocH = strI.find( 'H', uLocZ);

	// Collect XYZH parts after flag
	string strX = strI.substr( uLocX + 1, uLocY - uLocX - 1);
	if(!chkNum(strX))	{ return *this; }
	string strY = strI.substr( uLocY + 1, uLocZ - uLocY - 1);
	if(!chkNum(strY))	{ return *this; } 
	string strZ = strI.substr( uLocZ + 1, uLocH - uLocZ - 1);
	if(!chkNum(strZ))	{ return *this; } 
	string strH = strI.substr(uLocH + 1);
	if(!chkNum(strH))	{ return *this; }

	// Convert to this
	m_fX = strToNum<float>(strX);
	m_fY = strToNum<float>(strY);
	m_fZ = strToNum<float>(strZ);
	m_fH = strToNum<float>(strH);

	return *this;
}

bool SFWQuate::chkStrCanCvt( string strI )
{
	// Locate XYZH parts
	UINT uLocX = strI.find('X');
	if(uLocX == string::npos)	{ return false; }
	UINT uLocY = strI.find( 'Y', uLocX);
	if(uLocY == string::npos)	{ return false; }
	UINT uLocZ = strI.find( 'Z', uLocY);
	if(uLocZ == string::npos)	{ return false; }
	UINT uLocH = strI.find( 'H', uLocZ);
	if(uLocH == string::npos)	{ return false; }

	// Collect the color parts after flag
	string strX = strI.substr( uLocX + 1, uLocY - uLocX - 1);
	if(!chkNum(strX))	{ return false; }
	string strY = strI.substr( uLocY + 1, uLocZ - uLocY - 1);
	if(!chkNum(strY))	{ return false; } 
	string strZ = strI.substr( uLocZ + 1, uLocH - uLocZ - 1);
	if(!chkNum(strZ))	{ return false; } 
	string strH = strI.substr(uLocH + 1);
	if(!chkNum(strH))	{ return false; }

	return true;
}
