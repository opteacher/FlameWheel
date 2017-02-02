#pragma once
#include "stdafx.h"
#include "Plane.h"
#include "Math.h"

using namespace OPMath;

CPlane::CPlane( CPoint poiAI, CPoint poiBI, CPoint poiCI): ILnEqua()	{
	if(poiAI == poiBI || poiBI == poiCI || poiCI == poiAI)	{
		throw std::invalid_argument("W:three points equals to each other! can't build plane");
		}
	CVector vecAB( poiAI, poiBI);
	CVector vecAC( poiAI, poiCI);
	vecAB.normalize();	vecAC.normalize();
	if(vecAB.isZeroVec() || vecAC.isZeroVec()
	|| vecAB == vecAC || vecAB.getNegVec() == vecAC)	{
		throw std::invalid_argument("three points equals to each other or in a line!");
		}
		
	CVector vecNor = vecAB.cross(vecAC);
	if(!vecNor.isNorVec())	{
		vecNor.normalize();
		}
	m_vecNor.m_fX = vecNor.m_fX;
	m_vecNor.m_fY = vecNor.m_fY;
	m_vecNor.m_fZ = vecNor.m_fZ;
	m_fD = m_vecNor.dot(CVector(poiAI)).sun();
}

CPlane::CPlane( CPoint poiI, CVector vecNorI): ILnEqua()	{
	if(!vecNorI.isNorVec())	{
		vecNorI.normalize();
	}

	m_vecNor.m_fX = vecNorI.m_fX;
	m_vecNor.m_fY = vecNorI.m_fY;
	m_vecNor.m_fZ = vecNorI.m_fZ;
	m_fD = m_vecNor.dot(CVector(poiI)).sun();
}

float CPlane::getXOfPln( float fYI, float fZI) const	{
	return(m_fD - m_vecNor.m_fY*fYI - m_vecNor.m_fZ*fZI);
}

float CPlane::getYOfPln( float fXI, float fZI) const	{
	return(m_fD - m_vecNor.m_fX*fXI - m_vecNor.m_fZ*fZI);
}

float CPlane::getZOfPln( float fXI, float fYI) const	{
	return(m_fD - m_vecNor.m_fX*fXI - m_vecNor.m_fY*fYI);
}

CVector CPlane::getNorVec() const	{
	return(m_vecNor);
}

float CPlane::getD() const	{
	return(m_fD);
}

int CPlane::mkFunType(CPoint poiI)	{
	float fValChk = calculate( poiI.m_fX, poiI.m_fY, poiI.m_fZ);
	if(::equals( fValChk, m_fD))	{
		m_iTypeFun = EQUAL;
	}
	else if(fValChk < m_fD)	{
		m_iTypeFun = LESS;
	}
	else	{
		m_iTypeFun = BIGGER;
	}
	return(m_iTypeFun);
}

bool CPlane::isFillToFun( float fXI, float fYI, float fZI) const	{
	float fSun = m_vecNor.m_fX*fXI + m_vecNor.m_fY*fYI + m_vecNor.m_fZ*fZI;
	switch(m_iTypeFun)	{
	case LESS:
		if(m_bEnbEquals)	{
			return(m_fD >= fSun);
		}
		else	{
			return(m_fD > fSun);
		}
	case BIGGER:
		if(m_bEnbEquals)	{
			return(m_fD <= fSun);
		}
		else	{
			return(m_fD < fSun);
		}
	}
	return(::equals<float>( m_fD, fSun));
}

float CPlane::calculate( float fXI, float fYI, float fZI) const	{
	return(m_vecNor.m_fX*fXI + m_vecNor.m_fY*fYI + m_vecNor.m_fZ*fZI);
}

CPoint CPlane::calcPoiOfThreePln( CPlane plnAI, CPlane plnBI, CPlane plnCI)	{
	if(plnAI.m_vecNor.isZeroVec()
	|| plnBI.m_vecNor.isZeroVec()
	|| plnCI.m_vecNor.isZeroVec())	{
		throw std::invalid_argument("E:error plane as the param");
	}
	if(plnAI.m_fD == 0
	&& plnBI.m_fD == 0
	&& plnCI.m_fD == 0)	{
		//return world center
		return(CPoint());
	}
	
	CMatrix matTmp(DIM_2D);
	matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fY;	matTmp.m_fVal[0][2] = plnAI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnBI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnCI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnCI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnCI.m_vecNor.m_fZ;
	float fValDn = matTmp.getDetVal();
	
	CPoint poiRet;
	if(0 == fValDn)	{
		matTmp = CMatrix(2);
		if(plnAI.m_vecNor.m_fX == 0
		&& plnBI.m_vecNor.m_fX == 0
		&& plnCI.m_vecNor.m_fX == 0)	{
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fY;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fZ;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fY;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fZ;
			fValDn = matTmp.getDetVal();
			
			if(0 == fValDn)	{
				throw std::runtime_error("W:no point of the three planes");
			}
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_fD;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fZ;
			matTmp.m_fVal[1][0] = plnBI.m_fD;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fZ;
			poiRet.m_fY = matTmp.getDetVal()/fValDn;
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fY;	matTmp.m_fVal[0][1] = plnAI.m_fD;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fY;	matTmp.m_fVal[1][1] = plnBI.m_fD;
			poiRet.m_fZ = matTmp.getDetVal()/fValDn;
			
			return(poiRet);
		}
		else if(plnAI.m_vecNor.m_fY == 0
			 && plnBI.m_vecNor.m_fY == 0
			 && plnCI.m_vecNor.m_fY == 0)	{
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fZ;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fZ;
			fValDn = matTmp.getDetVal();
			
			if(0 == fValDn)	{
				throw std::runtime_error("W:no point of the three planes");
			}
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_fD;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fZ;
			matTmp.m_fVal[1][0] = plnBI.m_fD;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fZ;
			poiRet.m_fX = matTmp.getDetVal()/fValDn;
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_fD;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_fD;
			poiRet.m_fZ = matTmp.getDetVal()/fValDn;
			
			return(poiRet);
		}
		else if(plnAI.m_vecNor.m_fZ == 0
			 && plnBI.m_vecNor.m_fZ == 0
			 && plnCI.m_vecNor.m_fZ == 0)	{
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fY;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fY;
			fValDn = matTmp.getDetVal();
			
			if(0 == fValDn)	{
				throw std::runtime_error("W:no point of the three planes");
			}
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_fD;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fY;
			matTmp.m_fVal[1][0] = plnBI.m_fD;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fY;
			poiRet.m_fX = matTmp.getDetVal()/fValDn;
			
			matTmp.tunIndMat();
			matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_fD;
			matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_fD;
			poiRet.m_fY = matTmp.getDetVal()/fValDn;
			
			return(poiRet);
		}
		else	{
			throw std::runtime_error("E:unknow error, can't calculate cross point");
		}
	}
	
	matTmp.tunIndMat();
	matTmp.m_fVal[0][0] = plnAI.m_fD;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fY;	matTmp.m_fVal[0][2] = plnAI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnBI.m_fD;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnBI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnCI.m_fD;	matTmp.m_fVal[1][1] = plnCI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnCI.m_vecNor.m_fZ;
	poiRet.m_fX = matTmp.getDetVal()/fValDn;

	matTmp.tunIndMat();
	matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_fD;	matTmp.m_fVal[0][2] = plnAI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_fD;	matTmp.m_fVal[1][2] = plnBI.m_vecNor.m_fZ;
	matTmp.m_fVal[1][0] = plnCI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnCI.m_fD;	matTmp.m_fVal[1][2] = plnCI.m_vecNor.m_fZ;
	poiRet.m_fY = matTmp.getDetVal()/fValDn;

	matTmp.tunIndMat();
	matTmp.m_fVal[0][0] = plnAI.m_vecNor.m_fX;	matTmp.m_fVal[0][1] = plnAI.m_vecNor.m_fY;	matTmp.m_fVal[0][2] = plnAI.m_fD;
	matTmp.m_fVal[1][0] = plnBI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnBI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnBI.m_fD;
	matTmp.m_fVal[1][0] = plnCI.m_vecNor.m_fX;	matTmp.m_fVal[1][1] = plnCI.m_vecNor.m_fY;	matTmp.m_fVal[1][2] = plnCI.m_fD;
	poiRet.m_fZ = matTmp.getDetVal()/fValDn;
	
	return(poiRet);
}

CPoint CPlane::getProjPoi(CPoint poiI) const	{
	CVector vecToPln = m_vecNor;
	if(this->calculate( poiI.m_fX, poiI.m_fY, poiI.m_fZ) > m_fD)	{
		vecToPln = m_vecNor.getNegVec();
	}
	if(!vecToPln.isNorVec())	{
		vecToPln.normalize();
	}
	
	float fI = (m_fD - m_vecNor.dot(CVector(poiI)).sun()) / m_vecNor.dot(vecToPln).sun();
	CPoint poiRet = poiI + vecToPln*fI;
	
	return(poiRet);
}

bool CPlane::equals(const CPlane& refPlnI) const	{
	return(m_vecNor == refPlnI.m_vecNor
		&& m_fD == refPlnI.m_fD
		&& m_iTypeFun == refPlnI.m_iTypeFun
		&& m_bEnbEquals == refPlnI.m_bEnbEquals);
}

int CPlane::tunOtherSide()	{
	if(BIGGER == m_iTypeFun
	|| LESS == m_iTypeFun)	{
		m_iTypeFun *= -1;
	}
	
	return(m_iTypeFun);
}