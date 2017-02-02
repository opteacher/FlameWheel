#pragma once
#include "stdafx.h"
#include "Cone.h"
#include "Math.h"

using namespace OPMath;

CCone::CCone( CPoint poiOrgI, CVector vecDirI, float fRangeI, float fAngRadI): \
	m_poiOrg(poiOrgI), m_vecDir(vecDirI), m_fRange(fRangeI), m_fAngRad(fAngRadI)
{

	if(!m_vecDir.isNorVec())	{ m_vecDir.normalize(); }
	
	CPlane plnTar( m_poiOrg + m_vecDir*m_fRange, m_vecDir.getNegVec());
	plnTar.mkFunType(m_poiOrg);
	m_aPlnSet.clear();
	insertPln(plnTar);
}

bool CCone::isInside(CPoint poiI)
{
	bool bBaseInside = CRigid::isInside(poiI);
	if(!bBaseInside)
	{
		return false;
	}
	else if(m_poiOrg.isZeroPoi()
		&&	m_vecDir.isZeroVec()
		&&  m_fAngRad == 0)
	{
		return(bBaseInside);
	}
	
	CVector vecOrgToPoi( m_poiOrg, poiI);
	float fAngOrgPoi = vecOrgToPoi.angWithVec( m_vecDir, false);
	
	if(fAngOrgPoi > m_fAngRad)
	{ return false; }
	else
	{ return true; }
}

void CSqCone::mkInside(CPoint poiI)
{
	for( UINT i = 0; i < m_aPlnSet.size(); ++i)
	{
		m_aPlnSet[i].mkFunType(poiI);
	}
}