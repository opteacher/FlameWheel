#pragma once
#include "stdafx.h"
#include "FWMisc.h"

string CFWCoord::convertToStr() const
{
	string strRet = "";
	strRet += "C[" + ((SFWQuate&)m_poiCenter).convertToStr() + "]";
	strRet += "X[" + ((SFWQuate&)m_vecAxis[AXIS_X]).convertToStr() + "]";
	strRet += "Y[" + ((SFWQuate&)m_vecAxis[AXIS_Y]).convertToStr() + "]";
	strRet += "Z[" + ((SFWQuate&)m_vecAxis[AXIS_Z]).convertToStr() + "]";
	return strRet;
}

CFWCoord& CFWCoord::convertFromStr( string strI )
{
	string::size_type stpOff = 0;
	stpOff = collectQatFromStr( strI, stpOff, "C", m_poiCenter);
	stpOff = collectQatFromStr( strI, stpOff, "X", m_vecAxis[AXIS_X]);
	stpOff = collectQatFromStr( strI, stpOff, "Y", m_vecAxis[AXIS_Y]);
	stpOff = collectQatFromStr( strI, stpOff, "Z", m_vecAxis[AXIS_Z]);
	return *this;
}

bool CFWCoord::chkStrCanCvt( string strI )
{
	SQuate qatTmp;
	string::size_type stpOff = 0;
	if(collectQatFromStr( strI, stpOff, "C", qatTmp) == string::npos)	{ return false; }
	if(collectQatFromStr( strI, stpOff, "X", qatTmp) == string::npos)	{ return false; }
	if(collectQatFromStr( strI, stpOff, "Y", qatTmp) == string::npos)	{ return false; }
	if(collectQatFromStr( strI, stpOff, "Z", qatTmp) == string::npos)	{ return false; }

	return true;
}

std::string::size_type CFWCoord::collectQatFromStr(
		string strI,
		string::size_type& rFndOff,
		string strQatI,
		SQuate& rQatO)
{
	string::size_type stpTmp = strI.find( strQatI + '[', rFndOff);
	if(stpTmp != string::npos)
	{
		stpTmp += 2;
		rFndOff = stpTmp;
		if(rFndOff >= strI.length())	{ return string::npos; }
		
		rFndOff = strI.find( ']', rFndOff);
		if(rFndOff == string::npos)		{ return string::npos; }

		string strQat = strI.substr(GET_SUB_STR( stpTmp, rFndOff - 1));
		if(!SFWQuate::chkStrCanCvt(strQat))	{ return string::npos; }
		rQatO = SFWQuate().convertFromStr(strQat);
	}

	return rFndOff;
}