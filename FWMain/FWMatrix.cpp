#pragma once
#include "stdafx.h"
#include "FWMisc.h"
#include "../OPTools/File.h"

string CFWMatrix::convertToStr() const
{
	SFWQuate qatLine1( m_fVal[0][0], m_fVal[0][1], m_fVal[0][2], m_fVal[0][3]);
	SFWQuate qatLine2( m_fVal[1][0], m_fVal[1][1], m_fVal[1][2], m_fVal[1][3]);
	SFWQuate qatLine3( m_fVal[2][0], m_fVal[2][1], m_fVal[2][2], m_fVal[2][3]);
	SFWQuate qatLine4( m_fVal[3][0], m_fVal[3][1], m_fVal[3][2], m_fVal[3][3]);

	return	qatLine1.convertToStr().append(
			qatLine2.convertToStr().append(
			qatLine3.convertToStr().append(
			qatLine4.convertToStr())));
}

CFWMatrix& CFWMatrix::convertFromStr( string strI )
{
	UINT uLocLn1 = strI.find("X");
	UINT uLocLn2 = strI.find( "X", uLocLn1 + 1);
	UINT uLocLn3 = strI.find( "X", uLocLn2 + 1);
	UINT uLocLn4 = strI.find( "X", uLocLn3 + 1);

	string strLn1 = strI.substr(GET_SUB_STR( uLocLn1, uLocLn2));
	string strLn2 = strI.substr(GET_SUB_STR( uLocLn2, uLocLn3));
	string strLn3 = strI.substr(GET_SUB_STR( uLocLn3, uLocLn4));
	string strLn4 = strI.substr(uLocLn4);

	SFWQuate qatLn1 = SFWQuate().convertFromStr(strLn1);
	SFWQuate qatLn2 = SFWQuate().convertFromStr(strLn2);
	SFWQuate qatLn3 = SFWQuate().convertFromStr(strLn3);
	SFWQuate qatLn4 = SFWQuate().convertFromStr(strLn4);

	m_fVal[0][0] = qatLn1.m_fX;	m_fVal[0][1] = qatLn1.m_fY;	m_fVal[0][2] = qatLn1.m_fZ;	m_fVal[0][3] = qatLn1.m_fH;
	m_fVal[1][0] = qatLn2.m_fX;	m_fVal[1][1] = qatLn2.m_fY;	m_fVal[1][2] = qatLn2.m_fZ;	m_fVal[1][3] = qatLn2.m_fH;
	m_fVal[2][0] = qatLn3.m_fX;	m_fVal[2][1] = qatLn3.m_fY;	m_fVal[2][2] = qatLn3.m_fZ;	m_fVal[2][3] = qatLn3.m_fH;
	m_fVal[3][0] = qatLn4.m_fX;	m_fVal[3][1] = qatLn4.m_fY;	m_fVal[3][2] = qatLn4.m_fZ;	m_fVal[3][3] = qatLn4.m_fH;
	m_uNumDim = DIM_3D;

	return *this;
}

bool CFWMatrix::chkStrCanCvt( string strI )
{
	UINT uLocLn1 = strI.find("X");
	UINT uLocLn2 = strI.find( "X", uLocLn1);
	UINT uLocLn3 = strI.find( "X", uLocLn2);
	UINT uLocLn4 = strI.find( "X", uLocLn3);

	if(!SFWQuate::chkStrCanCvt(strI.substr( uLocLn1, uLocLn2 - uLocLn1)))	{ return false; }
	if(!SFWQuate::chkStrCanCvt(strI.substr( uLocLn2, uLocLn3 - uLocLn2)))	{ return false; }
	if(!SFWQuate::chkStrCanCvt(strI.substr( uLocLn3, uLocLn4 - uLocLn3)))	{ return false; }
	if(!SFWQuate::chkStrCanCvt(strI.substr(uLocLn4)))	{ return false; }

	return true;
}
