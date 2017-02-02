#pragma once
#include "stdafx.h"
#include "FWSpace.h"

INI_TYPID(CFWSpace);

CFWParam CFWSpace::genCrteInfo() const
{
	return CFWParam( "spcLoc", ((CFWCoord&)ptd_codLoc).convertToStr());
}

void CFWSpace::movTo(CPoint posI)
{
	ptd_codLoc.m_poiCenter = posI;
}

void CFWSpace::rotBy( float fAglI, CRay axsRoI)
{
	ptd_codLoc.transfmByMat(CMatrix::rotate3D( fAglI, axsRoI));
}