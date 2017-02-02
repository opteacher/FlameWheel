#pragma once
#include "stdafx.h"
#include "Coord.h"
#include "Math.h"

using std::vector;
using namespace OPMath;

CCoord::CCoord()
{
	m_poiCenter	= CPoint( 0.0f, 0.0f, 0.0f);

	m_vecAxis.clear();
	m_vecAxis.push_back(CVector( 1.0f, 0.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 1.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 0.0f, 1.0f));

	m_bLeftH = false;
}

CCoord::CCoord(CPoint poiCenI)
{
	m_poiCenter	= poiCenI;

	m_vecAxis.clear();
	m_vecAxis.push_back(CVector( 1.0f, 0.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 1.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 0.0f, 1.0f));

	m_bLeftH = false;
}

CCoord::CCoord(const CCoord& coordi)
{
	m_bLeftH = coordi.m_bLeftH;
	m_vecAxis = coordi.m_vecAxis;
	m_poiCenter = coordi.m_poiCenter;
}

CMatrix CCoord::transform(CCoord codTarI)
{
	//1.check the dimensions
	UINT uNumDim = m_vecAxis.size();
	if(uNumDim != codTarI.m_vecAxis.size())	{
		throw std::runtime_error("error dimensions, the number of dimensions");
	}
	//2.get the transfer matrix
	CMatrix matTrans = CMatrix::transfer3D(
			codTarI.m_poiCenter.m_fX - m_poiCenter.m_fX,
			codTarI.m_poiCenter.m_fY - m_poiCenter.m_fY,
			codTarI.m_poiCenter.m_fZ - m_poiCenter.m_fZ);

	//3.get the rotate matrix
	CMatrix matRot(uNumDim + 1);
	for( int i = uNumDim - 1; i > 0 ; --i)   {
		CVector axisThs(m_vecAxis[i]);
		CVector axisTar(codTarI.m_vecAxis[i]);
		if(axisThs == axisTar)	{ continue; }
		CVector vecRot = axisTar.cross(axisThs);
		float fAngRot = (float)axisTar.angWithVec( axisThs, false);
		CMatrix matTmp = CMatrix::rotate3D( fAngRot, vecRot);
		matRot = matRot.dotRight(matTmp);
	}

	//4.check axis X is the same. if not, @_@
	
	return(matTrans.dotRight(matRot));
}

CMatrix CCoord::transformFromWld()
{
	return(transform(CCoord()));
}

CMatrix CCoord::transformToWld() const
{
	return CCoord().transform(*this);
}

void CCoord::transfmByMat(CMatrix matI)
{
	m_poiCenter	= matI.multiPoiLeft(m_poiCenter);
	for( vector<CVector>::size_type i = 0; i < m_vecAxis.size(); ++i)	{
		m_vecAxis[i] = matI.multiVecLeft(m_vecAxis[i]);
		m_vecAxis[i].normalize();
	}
}

void CCoord::createCoord( CPoint poiCtI, CVector vecLookAtI)
{
	m_poiCenter	= poiCtI;
	vecLookAtI.normalize();
	//look to the top
	CVector vecUp;
	if(vecLookAtI.m_fY == 1.0f
	&& vecLookAtI.m_fX == 0.0f
	&& vecLookAtI.m_fZ == 0.0f)	{
		vecUp.m_fZ	= 1.0f;
	}
	else	{
		vecUp.m_fY	= 1.0f;
	}

	CVector vecX = vecLookAtI.cross(vecUp);
	m_vecAxis.clear();
	m_vecAxis.push_back(vecX);
	m_vecAxis.push_back(vecX.cross(vecLookAtI));
	m_vecAxis.push_back(vecLookAtI);
	
	m_bLeftH = true;
}

void CCoord::createCoord(CPoint poiCtI)
{
	m_poiCenter	= poiCtI;
	m_vecAxis.clear();
	m_vecAxis.push_back(CVector( 1.0f, 0.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 1.0f, 0.0f));
	m_vecAxis.push_back(CVector( 0.0f, 0.0f, 1.0f));
}

void CCoord::regulate()
{
	CVector vecYTmp( 0.0f, 1.0f, 0.0f);
	if(1 == abs(m_vecAxis[AXIS_Z].m_fY))	{
		vecYTmp = CVector( 1.0f, 0.0f, 0.0f);
	}
	m_vecAxis[AXIS_X] = vecYTmp.cross(m_vecAxis[AXIS_Z]);
	m_vecAxis[AXIS_Y] = m_vecAxis[AXIS_Z].cross(m_vecAxis[AXIS_X]);
	m_vecAxis[AXIS_Y].normalize();
	m_vecAxis[AXIS_X] = m_vecAxis[AXIS_Y].cross(m_vecAxis[AXIS_Z]);
	m_vecAxis[AXIS_X].normalize();
}

void CCoord::tunLeftHand()	{ m_bLeftH = true; }

bool CCoord::isLeftHand() const	{ return(m_bLeftH); }

UINT CCoord::getDim()	{ return(m_vecAxis.size()); }

CMatrix CCoord::transfer( CPoint poiTarI, bool bAffI)
{
	CPoint poiTmp	= poiTarI - m_poiCenter;

	CMatrix matRet;
	matRet.transfer3D( poiTmp.m_fX, poiTmp.m_fY, poiTmp.m_fZ);

	if(bAffI)	{
		m_poiCenter = matRet.multiPoiLeft(m_poiCenter);
	}

	return(matRet);
}

CMatrix CCoord::transfer( UINT uIndAxis, float fValI, bool bAffI)
{
	m_vecAxis[uIndAxis].normalize();
	CVector vecTmp = m_vecAxis[uIndAxis] * fValI;
	CMatrix matRet = CMatrix::transfer3D( vecTmp.m_fX, vecTmp.m_fY, vecTmp.m_fZ);

	if(bAffI)	{
		m_poiCenter	= matRet.multiPoiLeft(m_poiCenter);
	}

	return(matRet);
}

CMatrix CCoord::rotate( UINT uIndAxis, float fValI, bool bAffI)
{
	CMatrix matRet;
	UINT uDim = getDim();
	float fAng	= ((float)FW_PI/180.0f) * fValI;//turn to radian
	if(2 == uDim)	{
		matRet = CMatrix::rotate3D( fAng, m_poiCenter);
	}
	else if(3 == uDim)	{
		matRet = CMatrix::rotate3D( fAng, m_vecAxis[uIndAxis]);
	}

	if(bAffI)	{
		for( UINT uIndex = 0; uIndex < uDim; ++uIndex)	{
			m_vecAxis[uIndex] = matRet.multiVecLeft(m_vecAxis[uIndex]);
		}
	}

	return(matRet);
}

CMatrix CCoord::scale( UINT uIndAxis, float fVali, bool bAffi)
{
	m_vecAxis[uIndAxis].normalize();
	CVector vecTmp = m_vecAxis[uIndAxis] * fVali;
	CMatrix matRet = CMatrix::scale3D( vecTmp.m_fX, vecTmp.m_fY, vecTmp.m_fZ);

	return(matRet);
}

bool CCoord::operator == (const CCoord& regCoordI)
{
	for( UINT i = 0; i < m_vecAxis.size(); ++i)
		if(m_vecAxis[i] != regCoordI.m_vecAxis[i])
			return false;
	return( (m_poiCenter == regCoordI.m_poiCenter) && \
		(m_bLeftH == regCoordI.m_bLeftH));
}