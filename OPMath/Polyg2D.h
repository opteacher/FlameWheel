#pragma once
#include <vector>
#include "Point.h"


namespace OPMath	{
	class CPolyg2D	{
	protected:
		std::vector<CPoint> m_vecPois;
	public:
		void addPoi(CPoint poiI);
		std::vector<CPoint> getEvePoisInside();
	};
};