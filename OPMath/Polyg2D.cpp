#pragma once
#include "stdafx.h"
#include "Polyg2D.h"
#include "Math.h"
#include "Line2D.h"

using namespace OPMath;
using namespace std;

void CPolyg2D::addPoi(CPoint poiI)	{
	m_vecPois.push_back(CPoint(poiI));
}

vector<CPoint> CPolyg2D::getEvePoisInside()	{
	//1.data check
	if(0 == m_vecPois.size())	{
		throw std::runtime_error("W:the polygon has no shape!");
	}
	if(1 == m_vecPois.size())	{
		return(m_vecPois);
	}
	vector<CPoint> vecRet;
	if(2 == m_vecPois.size())	{
		CPoint poiBeg = m_vecPois[0];
		CPoint poiEnd = m_vecPois[1];
		CVector vecDir( poiBeg, poiEnd);
		vecDir.normalize();
		
		CPoint poiTmp(poiBeg);
		while(((::getSign<float>(vecDir.m_fX) == 1.0f && poiTmp.m_fX < poiEnd.m_fX)
			|| (::getSign<float>(vecDir.m_fX) == -1.0f && poiTmp.m_fX > poiEnd.m_fX))
			&&((::getSign<float>(vecDir.m_fY) == 1.0f && poiTmp.m_fY < poiEnd.m_fY)
			|| (::getSign<float>(vecDir.m_fY) == -1.0f && poiTmp.m_fY > poiEnd.m_fY)))	{
			
			vecRet.push_back(CPoint(poiTmp));
			poiTmp.m_fX += vecDir.m_fX;
			poiTmp.m_fY += vecDir.m_fY;
		}
		
		return(vecRet);
	}
	//2.build line function group
	vector<CLine2D> vecLnFun;
	CPoint poiOther = m_vecPois[2];
	for( DWORD i = 0; i < m_vecPois.size() - 1; ++i)	{
		CPoint poiBeg = m_vecPois[i];
		CPoint poiEnd = m_vecPois[i + 1];
		
		CLine2D lnCur( poiBeg, poiEnd);
		lnCur.mkFunType( poiOther.m_fX, poiOther.m_fY);
		
		vecLnFun.push_back(CLine2D(lnCur));
	}
	
	//3.designate range
	float fXMin = m_vecPois[0].m_fX;
	float fYMin = m_vecPois[0].m_fY;
	float fXMax = fXMin;
	float fYMax = fYMin;
	
	for( DWORD i = 0; i < m_vecPois.size(); ++i)	{
		CPoint poiCur = m_vecPois[i];
		if(poiCur.m_fX < fXMin)	{
			fXMin = poiCur.m_fX;
		}
		else if(poiCur.m_fX > fXMax)	{
			fXMax = poiCur.m_fX;
		}
		else if(poiCur.m_fY < fYMin)	{
			fYMin = poiCur.m_fY;
		}
		else if(poiCur.m_fY > fYMax)	{
			fYMax = poiCur.m_fY;
		}
	}
	
	//4.collection
	CPoint poiTmp;
	for( poiTmp.m_fX = fXMin; poiTmp.m_fX < fXMax; ++poiTmp.m_fX)	{
		for( poiTmp.m_fY = fYMin; poiTmp.m_fY < fYMax; ++poiTmp.m_fY)	{
			
			bool bInside = true;
			for( UINT i = 0; i < vecLnFun.size(); ++i)	{
				if(!vecLnFun[i].isFillToFun( poiTmp.m_fX, poiTmp.m_fY, poiTmp.m_fZ))	{
					bInside = false;
					break;
				}
			}
			
			if(bInside)	{
				vecRet.push_back(CPoint(poiTmp));
			}
		}
	}
	
	return(vecRet);
}