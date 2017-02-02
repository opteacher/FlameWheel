#pragma once
#include "stdafx.h"
#include "Point.h"
#include "Vector.h"
#include "Math.h"

using OPMath::CPoint;
using OPMath::CVector;

CPoint::CPoint()
{
	m_fX	= 0.0f;
	m_fY	= 0.0f;
	m_fZ	= 0.0f;
	m_fH	= 1.0f;
}

CPoint::CPoint( float fxi, float fyi, float fzi, float fhi)
{
	m_fX	= fxi;
	m_fY	= fyi;
	m_fZ	= fzi;
	m_fH	= fhi;
}

CPoint::CPoint(const SQuate& refQutI)
{
	m_fX	= refQutI.m_fX;
	m_fY	= refQutI.m_fY;
	m_fZ	= refQutI.m_fZ;
	m_fH	= refQutI.m_fH;
}

CPoint::CPoint(const CPoint& refPoiI)
{
	m_fX	= refPoiI.m_fX;
	m_fY	= refPoiI.m_fY;
	m_fZ	= refPoiI.m_fZ;
	m_fH	= refPoiI.m_fH;
}

CPoint::CPoint( const CVector& refVecI)
{
	m_fX	= refVecI.m_fX;
	m_fY	= refVecI.m_fY;
	m_fZ	= refVecI.m_fZ;
	m_fH	= refVecI.m_fH;
}

CPoint& CPoint::operator=(const CPoint& refPoiI)
{
	m_fX	= refPoiI.m_fX;
	m_fY	= refPoiI.m_fY;
	m_fZ	= refPoiI.m_fZ;
	m_fH	= refPoiI.m_fH;
	return(*this);
}

CPoint CPoint::operator+(const CPoint& poii) const
{
	CPoint poiRet;

	poiRet.m_fX	= m_fX + poii.m_fX;
	poiRet.m_fY	= m_fY + poii.m_fY;
	poiRet.m_fZ	= m_fZ + poii.m_fZ;
	//poiRet.m_fH	= m_fH + poii.m_fH;

	return(poiRet);
}

CPoint CPoint::operator+=(const CVector& rVecI)
{
	*this = *this + rVecI;
	return *this;
}

CPoint CPoint::operator-(const CPoint& poii) const
{
	CPoint poiRet;

	poiRet.m_fX	= m_fX - poii.m_fX;
	poiRet.m_fY	= m_fY - poii.m_fY;
	poiRet.m_fZ	= m_fZ - poii.m_fZ;
	//poiRet.m_fH	= m_fH - poii.m_fH;

	return(poiRet);
}

CPoint CPoint::operator*(float fVali) const
{
	CPoint poiRet;

	poiRet.m_fX	= m_fX * fVali;
	poiRet.m_fY	= m_fY * fVali;
	poiRet.m_fZ	= m_fZ * fVali;
	//H will involue into the calculation
	poiRet.m_fH	= m_fH * fVali;

	return(poiRet);
}

CPoint CPoint::operator/(float fVali) const
{
	CPoint poiRet;

	poiRet.m_fX	= m_fX / fVali;
	poiRet.m_fY	= m_fY / fVali;
	poiRet.m_fZ	= m_fZ / fVali;
	//H will involue into the calculation
	poiRet.m_fH	= m_fH / fVali;

	return(poiRet);
}

bool CPoint::operator==(const CPoint& poii) const
{
	return(
		abs(m_fX - poii.m_fX) < MIN_VAL &&
		abs(m_fY - poii.m_fY) < MIN_VAL &&
		abs(m_fZ - poii.m_fZ) < MIN_VAL);
}

bool CPoint::operator!=(const CPoint& poii) const
{
	return(
		abs(m_fX - poii.m_fX) >= MIN_VAL &&
		abs(m_fY - poii.m_fY) >= MIN_VAL &&
		abs(m_fZ - poii.m_fZ) >= MIN_VAL);
}

bool CPoint::operator<(const CPoint& poii) const
{
	return((
		m_fX < poii.m_fX &&
		m_fY < poii.m_fY &&
		m_fZ < poii.m_fZ));
}

bool CPoint::operator>(const CPoint& poii) const
{
	return((
		m_fX > poii.m_fX &&
		m_fY > poii.m_fY &&
		m_fZ > poii.m_fZ));
}

void CPoint::regulate()
{
	m_fX = OPMath::regulate<float>(m_fX);
	m_fY = OPMath::regulate<float>(m_fY);
	m_fZ = OPMath::regulate<float>(m_fZ);
}

bool CPoint::isZeroPoi() const
{
	return(abs(m_fX) < MIN_VAL
		&& abs(m_fY) < MIN_VAL
		&& abs(m_fZ) < MIN_VAL);
}