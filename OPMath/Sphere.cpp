#pragma once
#include "stdafx.h"
#include "Sphere.h"

using namespace OPMath;

bool CSphere::isInside( CPoint poiI ) const
{
	float fFunRes = \
		pow( poiI.m_fX - m_poiCenter.m_fX, 2) \
	+	pow( poiI.m_fY - m_poiCenter.m_fY, 2) \
	+	pow( poiI.m_fZ - m_poiCenter.m_fZ, 2);
	return(fFunRes <= pow( m_fRadius, 2));
}

bool CSphere::isIntersect(CSphere sphI) const
{
	return CVector( sphI.m_poiCenter, m_poiCenter).getVecLen() < (sphI.m_fRadius + m_fRadius);
}