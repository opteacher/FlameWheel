#pragma once
#include <vector>
#include "windows.h"
#include "Point.h"
#include "Plane.h"


namespace OPMath
{
	#define PLN_XY 0
	#define PLN_YZ 1
	#define PLN_XZ 2
	
	class CTriangle: public CPlane
	{
	protected:
		CPoint m_poiA;
		CPoint m_poiB;
		CPoint m_poiC;
		UINT m_uProjablePln;//identify which plane can be projected
	public:
		CTriangle( CPoint poiAI, CPoint poiBI, CPoint poiCI, UINT uProjablePlnI = 3);
	public:
		bool isPoiInside(CPoint poiI) const;
		std::vector<CPoint> getEvePoisInside() const;
		CPoint getPoiCen() const;
	};
};