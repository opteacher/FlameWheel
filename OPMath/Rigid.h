#pragma once
#include <map>
#include <vector>
#include "windows.h"
#include "Plane.h"
#include "Point.h"


namespace OPMath
{
	class CRigid
	{
	public:
		enum { OUTSIDE = -1, ON, INSIDE };
	protected:
		std::vector<CPlane> m_aPlnSet;
	public:
		void clearUp();
		void insertPln(CPlane plnI);
		bool isPlnExist(const CPlane& refPlnI);
		void buildSpace(std::map<DWORD, CPoint> mpIndPoiI);
		virtual bool isInside(CPoint poiI);
	};
};