#pragma once
#include "Point.h"
#include "Triangle.h"

namespace OPMath
{
	class CRegTgl: public CTriangle
	{
	public:
		CRegTgl( CPoint poiTopI, CPoint poiLftI, CPoint poiRgtI): 
			CTriangle( poiTopI, poiLftI, poiRgtI)	{}
	public:
		CPoint getPoiCen()	{ return((m_poiB + m_poiC)/2); }
	};
};
