#pragma once
#include "stdafx.h"
#include "FWColor.h"
#include "../OPMath/Math.h"
using OPMath::geneRandVal;
#include "../OPTools/String.h"
using OPTools::numToStr;
using OPTools::strToNum;
using OPTools::chkNum;

FWMAIN_API extern SFWColor g_colWhite = SFWColor( 255, 255, 255);
FWMAIN_API extern SFWColor g_colGray = SFWColor( 100, 100, 100);
FWMAIN_API extern SFWColor g_colBlack = SFWColor( 0, 0, 0);
FWMAIN_API extern SFWColor g_colRed = SFWColor( 255, 0, 0);
FWMAIN_API extern SFWColor g_colGreen = SFWColor( 0, 255, 0);
FWMAIN_API extern SFWColor g_colBlue = SFWColor( 0, 0, 255);
FWMAIN_API extern SFWColor g_colCBlue = SFWColor( 100, 100, 255);
FWMAIN_API extern SFWColor g_colYellow = SFWColor( 255, 255, 0);
FWMAIN_API extern SFWColor g_colCyan = SFWColor( 0, 255, 0);
FWMAIN_API extern SFWColor g_colMagenta = SFWColor( 255, 0, 255);

SFWColor::SFWColor( DWORD dwColValI, EColMode uColModI)
{
	switch(uColModI)
	{
	default:
	case EMC_X8R8G8B8:
	case EMC_A8R8G8B8:
		m_iAlph = (dwColValI>>24)&0xff;
	case EMC_R8G8B8:
		m_iRed = (BYTE)(dwColValI>>16)&0xff;
		m_iGreen = (BYTE)(dwColValI>>8)&0xff;
		m_iBlue = (BYTE)(dwColValI)&0xff;
		break;
	case EMC_R5G6B5:
		m_iRed = (dwColValI>>11)&0xffff;
		m_iGreen = (dwColValI>>5)&0x07e0;
		m_iBlue = (dwColValI)&0x001f;
		break;
	}
}

bool SFWColor::operator==(const SFWColor& rColI) const
{
	return m_iRed == rColI.m_iRed
		&& m_iGreen == rColI.m_iGreen
		&& m_iBlue == rColI.m_iBlue
		&& m_iAlph == rColI.m_iAlph;
}

bool SFWColor::operator!=(const SFWColor& rColI) const
{
	return m_iRed != rColI.m_iRed
		|| m_iGreen != rColI.m_iGreen
		|| m_iBlue != rColI.m_iBlue
		|| m_iAlph != rColI.m_iAlph;
}

bool SFWColor::operator<( const SFWColor& rColI ) const
{
	return m_iRed < rColI.m_iRed
		&& m_iGreen < rColI.m_iGreen
		&& m_iBlue < rColI.m_iBlue;
}

bool SFWColor::beSetCorrect() const
{
	return (m_iRed != -1) \
		&& (m_iGreen != -1) \
		&& (m_iBlue != -1);
}

DWORD SFWColor::getDword() const
{
	if(!beSetCorrect())	{ return 0; }
	return (DWORD(\
		(((DWORD)(m_iAlph)&0xff)<<24)\
		|(((DWORD)(m_iRed)&0xff)<<16)\
		|(((DWORD)(m_iGreen)&0xff)<<8)\
		|((DWORD)(m_iBlue)&0xff)));
}

SFWColor SFWColor::getNegCol() const
{
	if(!beSetCorrect())	{ return SFWColor(); }
	return SFWColor( 255-m_iRed, 255-m_iGreen, 255-m_iBlue, m_iAlph);
}

SFWColor& SFWColor::chgDepth( float fValI, bool bEffAlphI)
{
	m_iRed = (int)(m_iRed*fValI);
	m_iGreen = (int)(m_iGreen*fValI);
	m_iBlue = (int)(m_iBlue*fValI);
	if(bEffAlphI)
	{
		m_iAlph = (int)(m_iAlph*fValI);
	}
	return *this;
}

SFWColor SFWColor::geneRandCol()
{
	SFWColor colRet;
	colRet.m_iAlph = 255;
	colRet.m_iRed = geneRandVal<int>( 0, 255);
	colRet.m_iGreen = geneRandVal<int>( 0, 255);
	colRet.m_iBlue = geneRandVal<int>( 0, 255);
	return colRet;
}

string SFWColor::convertToStr() const
{
	if(!beSetCorrect())	{ return ""; }
	string strRet = "";
	strRet += "R" + numToStr<int>(m_iRed);
	strRet += "G" + numToStr<int>(m_iGreen);
	strRet += "B" + numToStr<int>(m_iBlue);
	strRet += "A" + numToStr<int>(m_iAlph);
	return strRet;
}

SFWColor& SFWColor::convertFromStr( string strI )
{
	// Locate all color parts
	UINT uLocRed = strI.find('R');
	UINT uLocGreen = strI.find( 'G', uLocRed);
	UINT uLocBlue = strI.find( 'B', uLocGreen);
	UINT uLocAlph = strI.find( 'A', uLocBlue);

	// Collect the color parts after flag
	string strRed = strI.substr( uLocRed + 1, uLocGreen - uLocRed - 1);
	if(!chkNum(strRed))	{ return *this; }
	string strGreen = strI.substr( uLocGreen + 1, uLocBlue - uLocGreen - 1);
	if(!chkNum(strGreen))	{ return *this; } 
	string strBlue = strI.substr( uLocBlue + 1, uLocAlph - uLocBlue - 1);
	if(!chkNum(strBlue))	{ return *this; } 
	string strAlph = strI.substr(uLocAlph + 1);
	if(!chkNum(strAlph))	{ return *this; }

	// Convert to this
	m_iRed = strToNum<int>(strRed);
	m_iGreen = strToNum<int>(strGreen);
	m_iBlue = strToNum<int>(strBlue);
	m_iAlph = strToNum<int>(strAlph);

	return *this;
}

bool SFWColor::chkStrCanCvt( string strI )
{
	// Locate all color parts
	UINT uLocRed = strI.find('R');
	if(uLocRed == string::npos)	{ return false; }
	UINT uLocGreen = strI.find( 'G', uLocRed);
	if(uLocGreen == string::npos)	{ return false; }
	UINT uLocBlue = strI.find( 'B', uLocGreen);
	if(uLocBlue == string::npos)	{ return false; }
	UINT uLocAlph = strI.find( 'A', uLocBlue);
	if(uLocAlph == string::npos)	{ return false; }

	// Collect the color parts after flag
	string strRed = strI.substr( uLocRed + 1, uLocGreen - uLocRed - 1);
	if(!chkNum(strRed))	{ return false; }
	string strGreen = strI.substr( uLocGreen + 1, uLocBlue - uLocGreen - 1);
	if(!chkNum(strGreen))	{ return false; } 
	string strBlue = strI.substr( uLocBlue + 1, uLocAlph - uLocBlue - 1);
	if(!chkNum(strBlue))	{ return false; } 
	string strAlph = strI.substr(uLocAlph + 1);
	if(!chkNum(strAlph))	{ return false; }

	return true;
}
