#pragma once
#include "Point.h"
#include "Plane.h"

//	*---------------------> X
//	| 1 |  2  | 3
//	| -----------
//	| 8 |  9  | 4
//	| -----------
//	| 7 |  6  | 5
//	|
//	- Y
#define AREA_LT 1
#define AREA_MT 2
#define AREA_RT 3
#define AREA_RM 4
#define AREA_RB 5
#define AREA_MB 6
#define AREA_LB 7
#define AREA_LM 8
#define AREA_M 9

namespace OPMath
{
	
	//+_+: only use in the 2D(X, Y)
	class CRect: public CPlane
	{
	public:
		CRect()	{}
		CRect( CPoint poiLTI, CPoint poiRBI):
		CPlane( poiLTI, poiRBI, CPoint( poiLTI.m_fX, poiRBI.m_fY))
		{
			m_poiLT = poiLTI;
			m_poiRB = poiRBI;
		}
		CRect(const CRect& refRectI): CPlane(refRectI),
			m_poiLT(refRectI.m_poiLT), m_poiRB(refRectI.m_poiRB)	{}
		CRect& operator=(const CRect& refRectI)
		{
			m_poiLT = refRectI.m_poiLT;
			m_poiRB = refRectI.m_poiRB;
			return(*this);
		}
	public:
		CPoint m_poiLT;
		CPoint m_poiRB;
	public:
		float getWidth();
		float getHeight();
	public:
		/**
		 * return a line segment, CRect.m_poiLT and CRect.m_poiRB are the two points of line
		 * @param poiBegI
		 * @param poiEndI
		 * @return
		 */
		CRect intersectsLine( CPoint poiBegI, CPoint poiEndI);
		
		bool isInsideArea( int iIdAreaI, CPoint poiI);
		
		bool equals(const CRect& refRectI);
	protected:
		/**
		 * check line has cross point with the rectangle
		 * +_+:don't check two point both inside condition
		 * @param poiBegI
		 * @param poiEndI
		 * @return
		 */
		bool checkPoiInside( CPoint poiBegI, CPoint poiEndI);
	};
};