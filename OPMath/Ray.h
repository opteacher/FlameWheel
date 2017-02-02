#pragma once
#include "Point.h"
#include "Vector.h"
#include "Plane.h"
#include "Triangle.h"


namespace OPMath
{
	class CRay: public CVector
	{
	public:
		CRay()	{}
		CRay( CPoint poiBegI, CPoint poiTarI);
		CRay( CPoint poiBegI, CVector vecDirI);
		CRay(const CRay&);
		CRay(const CVector&);
	protected:
		CPoint m_poiBeg;
	public:
		void setPoiBeg(CPoint poiI)	{ m_poiBeg = poiI; }
		CPoint getPoiBeg() const	{ return m_poiBeg; }
	public:
		float disFromPoi(CPoint poiI);
		CPoint poiProjOnLn(CPoint poiI);
	public:
		bool intersectsTgl( CPoint poiAI, CPoint poiBI, CPoint poiCI);
		CPoint intersectsTgl(CTriangle tglI);
		CPoint intersectsRay(CRay rayI);
		CPoint intersectsPln(CPlane plnI);
	};
};