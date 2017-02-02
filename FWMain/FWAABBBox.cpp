#pragma once
#include "stdafx.h"
#include "FWMisc.h"

string CFWAABBBox::convertToStr() const
{
	return "LTF[" + ((SFWQuate&)m_poiLTF).convertToStr() + "]"
		+  "RBB[" + ((SFWQuate&)m_poiRBB).convertToStr() + "]";
}

CFWAABBBox& CFWAABBBox::convertFromStr(string strI)
{
	string::size_type stpOff = 0;
	stpOff = CFWCoord::collectQatFromStr( strI, stpOff, "LTF", m_poiLTF);
	stpOff = CFWCoord::collectQatFromStr( strI, stpOff, "RBB", m_poiRBB);
	return *this;
}

bool CFWAABBBox::chkStrCanCvt(string strI)
{
	SQuate qatTmp;
	string::size_type stpOff = 0;
	if(CFWCoord::collectQatFromStr( strI, stpOff, "LTF", qatTmp) == string::npos)	{ return false; }
	if(CFWCoord::collectQatFromStr( strI, stpOff, "RBB", qatTmp) == string::npos)	{ return false; }
	return true;
}