#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Math.h"

using namespace OPMath;

CVector::CVector()
{
	clear();
}

CVector::CVector( float fXI, float fYI, float fZI, float fHI): \
	CPoint( fXI, fYI, fZI, fHI), m_fLength(0.0f)
{
	getVecLen();
}

CVector::CVector( const CPoint& poiBegI, const CPoint& poiEndI)
{
	m_fX = poiEndI.m_fX - poiBegI.m_fX;
	m_fY = poiEndI.m_fY - poiBegI.m_fY;
	m_fZ = poiEndI.m_fZ - poiBegI.m_fZ;
	m_fH = 1;
	m_fLength = 0.0f;
	getVecLen();
}

CVector::CVector(const SQuate& refQutI)
{
	m_fX = refQutI.m_fX;
	m_fY = refQutI.m_fY;
	m_fZ = refQutI.m_fZ;
	m_fH = refQutI.m_fH;
	m_fLength = 0.0f;
	getVecLen();
}

CVector::CVector(const CVector& vecI)
{
	m_fX = vecI.m_fX;
	m_fY = vecI.m_fY;
	m_fZ = vecI.m_fZ;
	m_fH = vecI.m_fH;
	m_fLength = 0.0f;
	getVecLen();
}

CVector::CVector(const CPoint& poiI)
{
	m_fX = poiI.m_fX;
	m_fY = poiI.m_fY;
	m_fZ = poiI.m_fZ;
	m_fH = poiI.m_fH;
	m_fLength = 0.0f;
	getVecLen();
}

CVector CVector::operator=(const CVector& vecI)
{
	m_fX = vecI.m_fX;
	m_fY = vecI.m_fY;
	m_fZ = vecI.m_fZ;
	m_fH = vecI.m_fH;
	m_fLength = 0.0f;
	getVecLen();
	return(*this);
}

CVector CVector::operator+(const CVector& vecI)
{
	CVector vecRet;

	vecRet.m_fX	= m_fX + vecI.m_fX;
	vecRet.m_fY	= m_fY + vecI.m_fY;
	vecRet.m_fZ	= m_fZ + vecI.m_fZ;
	//vecRet.m_fH = m_fH + vecI.m_fH;

	vecRet.m_fLength = vecRet.getVecLen();

	return(vecRet);
}

CVector CVector::operator-(const CVector& vecI)
{
	CVector vecRet;

	vecRet.m_fX	= m_fX - vecI.m_fX;
	vecRet.m_fY	= m_fY - vecI.m_fY;
	vecRet.m_fZ	= m_fZ - vecI.m_fZ;
	//vecRet.m_fH = m_fH - vecI.m_fH;

	vecRet.m_fLength = vecRet.getVecLen();

	return(vecRet);
}

CVector CVector::operator*(float fValI)
{
	CVector vecRet;

	vecRet.m_fX	= m_fX * fValI;
	vecRet.m_fY	= m_fY * fValI;
	vecRet.m_fZ	= m_fZ * fValI;
	//H would not involue in the calculate
	//vecRet.m_fH	= m_fH * fValI;
	vecRet.m_fLength = m_fLength * abs(fValI);

	return(vecRet);
}

bool OPMath::CVector::equal(const CVector vecI)
{
	return(abs(1 - this->dot(vecI).sun()) < MIN_VAL);
}

float OPMath::CVector::cosWithVec( const CVector& vecI)
{
	if(::isZero(m_fLength))	{
		return(0.0f);
	}

	return((m_fX*vecI.m_fX + m_fY*vecI.m_fY + m_fZ*vecI.m_fZ) / ( m_fLength*vecI.m_fLength));
}

bool CVector::isSameDir_VerticalIn( CVector vecI )
{
	float fDot = dot(vecI).sun();
	if(0 == fDot)	{ return true; }
	return(0 < ::getSign<float>(fDot));
}

bool CVector::isSameDir_VerticalOut( CVector vecI )
{
	float fDot = dot(vecI).sun();
	if(0 == fDot)	{ return false; }
	return(0 < ::getSign<float>(fDot));
}

CVector CVector::getNegVec()	const
{
	return(CVector( -m_fX, -m_fY, -m_fZ));
}

CVector CVector::operator/(float fValI)
{
	CVector vecRet;
	if(0 == fValI)	{
		throw std::runtime_error("divisor can't be zero!");
	}

	vecRet.m_fX	= m_fX / fValI;
	vecRet.m_fY	= m_fY / fValI;
	vecRet.m_fZ	= m_fZ / fValI;
	//H would not involue in the calculate
	//vecRet.m_fH	= m_fH / fValI;
	vecRet.m_fLength = m_fLength / abs(fValI);

	return(vecRet);
}

CVector CVector::operator-()
{
	m_fX = -m_fX;
	m_fY = -m_fY;
	m_fZ = -m_fZ;
	return(*this);
}

CVector CVector::dot(const CVector& vecI) const
{
	CVector vecRet;
	if(!isOSSupportsSSE())	{
		vecRet.m_fX	= m_fX * vecI.m_fX;
		vecRet.m_fY	= m_fY * vecI.m_fY;
		vecRet.m_fZ	= m_fZ * vecI.m_fZ;

		vecRet.m_fH	= m_fH * vecI.m_fH;
	}
	else	{
		CPoint* pVecRet = new CPoint(*this);
		CPoint* pVecTmp = new CPoint(vecI);
		__asm {
			mov esi, pVecRet;
			mov edi, pVecTmp;
			movups xmm0, [esi];
			movups xmm1, [edi];
			mulps xmm0, xmm1;
			mov esi, pVecRet;
			movups [esi], xmm0;
		}
		vecRet = CVector(*pVecRet);
		vecRet.getVecLen(true);
		SAFE_DELETE(pVecRet);
		SAFE_DELETE(pVecTmp);
	}

	return(vecRet);
}

CVector CVector::cross(const CVector& vecI) const
{
	CVector vecRet;

	if(!isOSSupportsSSE())	{
		vecRet.m_fX	= m_fY * vecI.m_fZ - m_fZ * vecI.m_fY;
		vecRet.m_fY	= m_fZ * vecI.m_fX - m_fX * vecI.m_fZ;
		vecRet.m_fZ	= m_fX * vecI.m_fY - m_fY * vecI.m_fX;

		vecRet.m_fH	= m_fH * vecI.m_fH;
	}
	else	{
		CPoint* pVecRet = new CPoint(*this);
		CPoint* pVecTmp = new CPoint(vecI);
		__asm {
			mov esi, pVecRet;
			mov edi, pVecTmp;
			movups xmm0, [esi];
			movups xmm1, [edi];
			movaps xmm2, xmm0;
			movaps xmm3, xmm1;
			shufps xmm0, xmm0, 0xc9;
			shufps xmm1, xmm1, 0xd2;
			mulps xmm0, xmm1;
			shufps xmm2, xmm2, 0xd2;
			shufps xmm3, xmm3, 0xc9;
			mulps xmm2, xmm3;
			subps xmm0, xmm2;
			mov esi, pVecRet;
			movups [esi], xmm0;
		}
		vecRet = CVector(*pVecRet);
		vecRet.getVecLen(true);
		SAFE_DELETE(pVecRet);
		SAFE_DELETE(pVecTmp);
	}

	return(CVector(vecRet));
}

float CVector::sun() const
{
	return(m_fX + m_fY + m_fZ);
}

bool CVector::isZeroVec() const
{
	return(this->isZeroPoi());
}

bool CVector::isNorVec()
{
	return(equals<float>( 1.0f, m_fLength));
}

float CVector::normalize()
{
	/*if(equals<float>( 1.0f, m_fLength))	{
		return;
	}
	if(isZeroVec())	{
		return;
	}
	if(!isOSSupportsSSE())	{
		if(::isZero<float>(m_fLength))	{
			m_fLength = getVecLen();
			if(::isZero(m_fLength))	{
				throw SFWInfoExc(EXC_DEF_HLP( EXC_ZERO, "zero vector can't do normalize"));
			}
			*this = *this / m_fLength;
			m_fLength = 1.0f;
		}
	}
	else	{
		m_fH = 0;
		CPoint* pPoiThis = new CPoint(*this);
		__asm	{
			mov esi, pPoiThis;
			movups xmm0, [esi];
			movaps xmm2, xmm0;
			mulps xmm0, xmm0;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 4Eh;
			addps xmm0, xmm1;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 11h;
			addps xmm0, xmm1;

			rsqrtps xmm0, xmm0; reciprocal square root;
			mulps xmm2, xmm0; multiply;
			movups [esi], xmm2;
		}
		m_fH = 1.0;
		*this = CVector(*pPoiThis);
		m_fLength = 1.0f;
		SAFE_DELETE(pPoiThis);
	}*/
	float fLenRet = m_fLength;
	*this = this->getIndVec();
	return fLenRet;
}

CVector CVector::getIndVec() const
{
	CVector vecRet(*this);
	if(equals<float>( 1.0f, vecRet.m_fLength))	{
		return(vecRet);
	}
	if(vecRet.isZeroVec())	{
		return(vecRet);
	}
	if(!isOSSupportsSSE())	{
		if(::isZero<float>(vecRet.m_fLength))	{
			vecRet.m_fLength = vecRet.getVecLen();
			if(::isZero(m_fLength))	{
				throw std::runtime_error("zero vector can't do normalize");
			}
			vecRet = vecRet / m_fLength;
			vecRet.m_fLength = 1.0f;
		}
	}
	else	{
		vecRet.m_fH = 0;
		CPoint* pPoiThis = &vecRet;
		__asm	{
			mov esi, pPoiThis;
			movups xmm0, [esi];
			movaps xmm2, xmm0;
			mulps xmm0, xmm0;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 4Eh;
			addps xmm0, xmm1;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 11h;
			addps xmm0, xmm1;

			rsqrtps xmm0, xmm0; reciprocal square root;
			mulps xmm2, xmm0; multiply;
			movups [esi], xmm2;
		}
		vecRet.m_fH = 1.0;
		vecRet.m_fLength = 1.0f;
	}
	return(vecRet);
}

float CVector::getVecLen(bool bRecalculateI)
{
	if(!::isZero<float>(m_fLength) && !bRecalculateI)	{
		return(m_fLength);
	}

	if (!isOSSupportsSSE()) {
		m_fLength= (float)sqrt(m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ);
	}
	else {
		float* pf = new float(0);
		m_fH = 0.0f;
		CPoint* pPoiTmp = new CPoint(*this);
		__asm {
			mov ecx, pf ; point to result register;
			mov esi, pPoiTmp ; vector U;
			movups xmm0, [esi] ; vector U in XMM0;
			mulps xmm0, xmm0 ; multiply;
			movaps xmm1, xmm0 ; copy result;
			shufps xmm1, xmm1, 4Eh ; shuffle: f1,f0,f3,f2;
			addps xmm0, xmm1;
			movaps xmm1, xmm0 ; copy result;
			shufps xmm1, xmm1, 11h;
			addps xmm0, xmm1;
			sqrtss xmm0, xmm0 ; square root;
			movss [ecx], xmm0 ; move result to ECX which is f;
		}
		m_fLength = *pf;
		SAFE_DELETE(pPoiTmp);
		SAFE_DELETE(pf);
		m_fH = 1.0f;
	}

	return((::isZero(m_fLength)) ? 1.0f : m_fLength);
}

void CVector::clear()
{
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fH = 1.0f;
	m_fLength = 0.0f;
}

float CVector::angWithVec( CVector vecI, bool bRadI)
{
	//check length, zero vector has zero angle with every vector
	if( !m_fLength || !vecI.m_fLength)	{
		return(0.0f);
	}

	float fAngCos = ( m_fX*vecI.m_fX + m_fY*vecI.m_fY + m_fZ*vecI.m_fZ) / ( getVecLen()*vecI.getVecLen());

	float fAngRad = acosf(fAngCos);
	float fAngRet = (float)((fAngRad * 180) / FW_PI);

	return((bRadI) ? fAngRad : fAngRet);
}

bool CVector::isParallel(const CVector& vecI) const
{
	CVector vecTmpThis = this->getIndVec();
	CVector vecTmp = vecI.getIndVec();
	if(vecTmpThis == vecTmp)	{
		return true;
	}
	else if(vecTmpThis == vecI.getNegVec())	{
		return true;
	}
	return false;
}
	
bool CVector::isVertical(const CVector& vecI) const
{
	if(::isZero<float>(abs(this->dot(vecI).sun())))	{
		return true;
	}
	else	{
		return false;
	}
}