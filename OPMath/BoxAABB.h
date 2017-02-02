#pragma once
#include "windows.h"
#include "Point.h"
#include "Vector.h"
#include "Rigid.h"
#include "Matrix.h"


namespace OPMath
{
	class CBoxAABB: public CRigid
	{
	public:
		CBoxAABB(void);
		CBoxAABB(const CBoxAABB&);
		CBoxAABB( CPoint, CPoint);
	protected:
		CPoint m_poiLTF;//left top front
		CPoint m_poiRBB;//right bottom back
	public:
		//+_+:update points, only transfer matrix and scale
		void updatePois(CMatrix);
	public:
		inline CPoint getPoiCentre();

		CPoint getPoiLTF() const	{ return m_poiLTF; }
		CPoint getPoiRTF() const;
		CPoint getPoiRTB() const;
		CPoint getPoiLTB() const;

		CPoint getPoiRBB() const	{ return m_poiRBB; }
		CPoint getPoiLBB() const;
		CPoint getPoiLBF() const;
		CPoint getPoiRBF() const;

		inline float getWidth();
		inline float getLength();
		inline float getHeight();
	public:
		bool operator == (const CBoxAABB&);
	public:
		CPoint genePoi(UINT = 1);
		virtual bool isInside(CPoint poiI) const;
		virtual bool isIntersect(CBoxAABB) const;
	};
};