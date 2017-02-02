#pragma once
#include "stdafx.h"
#include "BoxAABB.h"
#include "Math.h"

using namespace OPMath;

CBoxAABB::CBoxAABB(void)
{
}

CBoxAABB::CBoxAABB(const CBoxAABB& regBoxI)
{
	m_poiLTF = regBoxI.m_poiLTF;
	m_poiRBB = regBoxI.m_poiRBB;
}

CBoxAABB::CBoxAABB( CPoint poiLTFI, CPoint poiRBBI):
	m_poiLTF(poiLTFI), m_poiRBB(poiRBBI)	{}

void CBoxAABB::updatePois(CMatrix matWldI)
{
	m_poiLTF = matWldI.multiPoiLeft(m_poiLTF);
	m_poiRBB = matWldI.multiPoiLeft(m_poiRBB);
}

CPoint CBoxAABB::getPoiCentre()
{
	return(CPoint(m_poiLTF + m_poiRBB) / 2);
}

CPoint CBoxAABB::getPoiRTF() const
{
	return(CPoint( m_poiRBB.m_fX, m_poiLTF.m_fY, m_poiLTF.m_fZ));
}

CPoint CBoxAABB::getPoiRTB() const
{
	return(CPoint( m_poiRBB.m_fX, m_poiLTF.m_fY, m_poiRBB.m_fZ));
}

CPoint CBoxAABB::getPoiLTB() const
{
	return(CPoint( m_poiLTF.m_fX, m_poiLTF.m_fY, m_poiRBB.m_fZ));
}

CPoint CBoxAABB::getPoiLBB() const
{
	return(CPoint( m_poiLTF.m_fX, m_poiRBB.m_fY, m_poiRBB.m_fZ));
}

CPoint CBoxAABB::getPoiLBF() const
{
	return(CPoint( m_poiLTF.m_fX, m_poiRBB.m_fY, m_poiLTF.m_fZ));
}

CPoint CBoxAABB::getPoiRBF() const
{
	return(CPoint( m_poiRBB.m_fX, m_poiRBB.m_fY, m_poiLTF.m_fZ));
}

bool CBoxAABB::operator == (const CBoxAABB& regBoxI)
{
	return( (m_poiLTF == regBoxI.m_poiLTF) && (m_poiRBB == regBoxI.m_poiRBB));
}

float CBoxAABB::getWidth()
{
	return(abs(m_poiLTF.m_fX - m_poiRBB.m_fX));
}

float CBoxAABB::getLength()
{
	return(abs(m_poiLTF.m_fZ - m_poiRBB.m_fZ));
}

float CBoxAABB::getHeight()
{
	return(abs(m_poiLTF.m_fY - m_poiRBB.m_fY));
}

CPoint CBoxAABB::genePoi(UINT uTypeI)
{
	switch(uTypeI)	{
	case OUTSIDE:	{
		return(CPoint( \
			((::geneRandBool()) ? \
				::geneRandVal<float>( (float)MIN_VAL, m_poiLTF.m_fX): \
				::geneRandVal<float>( m_poiRBB.m_fX, (float)MAX_VAL)), \
			((::geneRandBool()) ? \
				::geneRandVal<float>( (float)MIN_VAL, m_poiLTF.m_fY): \
				::geneRandVal<float>( m_poiRBB.m_fY, (float)MAX_VAL)), \
			((::geneRandBool()) ? \
				::geneRandVal<float>( (float)MIN_VAL, m_poiLTF.m_fZ): \
				::geneRandVal<float>( m_poiRBB.m_fZ, (float)MAX_VAL))) );
					}
	}

	return(CPoint());
}

bool CBoxAABB::isInside(CPoint poiI) const
{
	return between<float>( poiI.m_fX, m_poiLTF.m_fX, m_poiRBB.m_fX, true)
		&& between<float>( poiI.m_fY, m_poiLTF.m_fY, m_poiRBB.m_fY, true)
		&& between<float>( poiI.m_fZ, m_poiLTF.m_fZ, m_poiRBB.m_fZ, true);
}

bool CBoxAABB::isIntersect(CBoxAABB boxI) const
{
	return between<float>( boxI.m_poiLTF.m_fX, m_poiLTF.m_fX, m_poiRBB.m_fX, true)
		|| between<float>( boxI.m_poiRBB.m_fX, m_poiLTF.m_fX, m_poiRBB.m_fX, true)
		|| between<float>( boxI.m_poiLTF.m_fY, m_poiLTF.m_fY, m_poiRBB.m_fY, true)
		|| between<float>( boxI.m_poiRBB.m_fY, m_poiLTF.m_fY, m_poiRBB.m_fY, true)
		|| between<float>( boxI.m_poiLTF.m_fZ, m_poiLTF.m_fZ, m_poiRBB.m_fZ, true)
		|| between<float>( boxI.m_poiRBB.m_fZ, m_poiLTF.m_fZ, m_poiRBB.m_fZ, true);
}