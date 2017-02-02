#pragma once
#include "LnEqua.h"
#include "Vector.h"
#include "Point.h"

namespace OPMath
{
	class CPlane: public ILnEqua
	{
	public:
		CPlane(): m_vecNor( 0.0f, 1.0f, 0.0f), m_fD(0)	{}
		CPlane( float fAI, float fBI, float fCI, float fDI): m_fD(fDI)
		{
			m_vecNor.m_fX = fAI;
			m_vecNor.m_fY = fBI;
			m_vecNor.m_fZ = fCI;
			m_vecNor.normalize();
		}
		//m_vecNor.m_fX*x + m_vecNor.m_fY*y + m_vecNor.m_fZ*z = m_fD
		CPlane( CPoint poiAI, CPoint poiBI, CPoint poiCI);
		CPlane( CPoint poiI, CVector vecNorI);
		CPlane(const CPlane& refPlnI): ILnEqua(refPlnI),
			m_vecNor(refPlnI.m_vecNor), m_fD(refPlnI.m_fD)	{}
		CPlane& operator=(const CPlane& refPlnI)
		{
			m_vecNor = refPlnI.m_vecNor;
			m_fD = refPlnI.m_fD;
			
			return(*this);
		}
	protected:
		CVector m_vecNor;
		float m_fD;
	public:
		float getXOfPln( float fYI, float fZI) const;
		float getYOfPln( float fXI, float fZI) const;
		float getZOfPln( float fXI, float fYI) const;
		CVector getNorVec() const;
		float getD() const;
	public:
		int mkFunType(CPoint poiI);
		bool isFillToFun( float fXI, float fYI, float fZI) const;
		float calculate( float fXI, float fYI, float fZI) const;
		
		/**
		 * make out the cross point of the three planes
		 * @param plnAI
		 * @param plnBI
		 * @param plnCI
		 * @return
		 * @throws Exception
		 */
		static CPoint calcPoiOfThreePln( CPlane plnAI, CPlane plnBI, CPlane plnCI);
		
		//get the projection point on the plane
		CPoint getProjPoi(CPoint poiI) const;
	public:
		bool equals(const CPlane& refPlnI) const;
		int tunOtherSide();
	};
};