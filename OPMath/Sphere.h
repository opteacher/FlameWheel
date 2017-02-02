#pragma once
#include "Point.h"
#include "Rigid.h"


namespace OPMath
{
	class CSphere: public CRigid
	{
	public:
		CSphere(): m_fRadius(0.0f)	{}
		CSphere( CPoint poiCenI, float fRadiusI): \
			m_poiCenter(poiCenI), m_fRadius(fRadiusI)	{}
	protected:
		CPoint m_poiCenter;
		float m_fRadius;
	public:
		CPoint getPoiCenter()	{ return(m_poiCenter); }
		float getRadius() const	{ return(m_fRadius); }
	public:
		virtual bool isInside(CPoint poiI) const;
		virtual bool isIntersect(CSphere) const;
	};
};