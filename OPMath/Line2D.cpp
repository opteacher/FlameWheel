#pragma once
#include "stdafx.h"
#include "Line2D.h"
#include "Math.h"

using namespace OPMath;

CLine2D::CLine2D( CPoint poiAI, CPoint poiBI)
{
	if(poiAI.m_fZ == 0.0f && poiBI.m_fZ == 0)	{
		if(equals<float>( poiBI.m_fX, poiAI.m_fX))	{
			m_fA = 1.0f;
			m_fB = 0.0f;
			m_fD = regulate<float>(poiBI.m_fX);
		}
		else if(equals<float>( poiBI.m_fY, poiAI.m_fY))	{
			m_fA = 0.0f;
			m_fB = 1.0f;
			m_fD = regulate<float>(poiBI.m_fY);
		}
		else	{
			float fDetX = poiBI.m_fX - poiAI.m_fX;
			float fDetY = poiBI.m_fY - poiAI.m_fY;
			m_fA = -fDetY;
			m_fB = fDetX;
			m_fD = fDetX*poiAI.m_fY - fDetY*poiAI.m_fX;
		}
	}
	else	{
		throw std::invalid_argument("two points create way should be only used to 2D");
	}
}

float CLine2D::getSlope() const
{
	if(0 == m_fB)	{
		throw std::invalid_argument("the slope is zero, error!");
	}
	
	return(-m_fA/m_fB);
}

float CLine2D::getInterc() const
{
	if(0 == m_fB)	{
		throw std::invalid_argument("the slope is zero, error!");
	}
	
	return(m_fD/m_fB);
}

float CLine2D::mkOutY(float fXI)
{
	if(m_fB == 0.0f)	{
		return(m_fD/m_fA);
	}
	
	return((m_fD - m_fA*fXI)/m_fB);
}

int CLine2D::mkFunType( float fXI, float fYI)
{
	float fSunTmp = m_fA*fXI + m_fB*fYI;
	if(fSunTmp > m_fD)	{
		m_iTypeFun = BIGGER;
	}
	else if(fSunTmp == m_fD)	{
		m_iTypeFun = EQUAL;
	}
	else	{
		m_iTypeFun = LESS;
	}
	return(m_iTypeFun);
}

bool CLine2D::isFillToFun( float fXI, float fYI, float fZI) const
{
	float fValSun = m_fA*fXI + m_fB*fYI;
	switch(m_iTypeFun)	{
	case LESS:
		if(m_bEnbEquals)	{
			return(m_fD >= fValSun);
		}
		else	{
			return(m_fD > fValSun);
		}
	case BIGGER:
		if(m_bEnbEquals)	{
			return(m_fD <= fValSun);
		}
		else	{
			return(m_fD < fValSun);
		}
	}
	return(equals<float>( fValSun, m_fD));
}

float CLine2D::calculate( float fXI, float fYI, float fZI) const
{
	return(m_fA*fXI + m_fB*fYI);
}