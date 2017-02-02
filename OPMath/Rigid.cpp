#pragma once
#include "stdafx.h"
#include "Rigid.h"

using namespace OPMath;
using namespace std;

void CRigid::clearUp()
{
	m_aPlnSet.clear();
}

void CRigid::insertPln(CPlane plnI)
{
	if(!isPlnExist(plnI))	{
		m_aPlnSet.push_back(plnI);
	}
}

bool CRigid::isPlnExist(const CPlane& refPlnI)
{
	for( UINT i = 0; i < m_aPlnSet.size(); ++i)	{
		if(m_aPlnSet[i].equals(refPlnI))	{
			return true;
		}
	}
	return false;
}

void CRigid::buildSpace(map<DWORD, CPoint> mpIndPoiI)
{
	for( map<DWORD, CPoint>::iterator iter = mpIndPoiI.begin(); mpIndPoiI.end() != iter; ++iter)	{
		m_aPlnSet[iter->first].mkFunType(iter->second);
	}
}

bool CRigid::isInside(CPoint poiI)
{
	for( UINT i = 0; i < m_aPlnSet.size(); ++i)	{
		if(!m_aPlnSet[i].isFillToFun( poiI.m_fX, poiI.m_fY, poiI.m_fZ))	{
			return false;
		}
	}

	return true;
}