#pragma once
#include "windows.h"
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"


#ifndef AXIS_X
#define AXIS_X 0
#endif
#ifndef AXIS_Y
#define AXIS_Y 1
#endif
#ifndef AXIS_Z
#define AXIS_Z 2
#endif

namespace OPMath
{
	class CCoord	{
	public:
		CCoord();
		CCoord(CPoint);
		CCoord(const CCoord&);
	public:
		std::vector<CVector> m_vecAxis;
		CPoint m_poiCenter;
	protected:
		bool m_bLeftH;
	public:
		void createCoord( CPoint, CVector);
		void createCoord(CPoint);
	public:
		CMatrix transform(CCoord);
		CMatrix transformFromWld();
		CMatrix transformToWld() const;
		void transfmByMat(CMatrix);
	public:
		void regulate();
		void tunLeftHand();
		bool isLeftHand() const;
		UINT getDim();
	public:
		CMatrix transfer( CPoint, bool = true);
		CMatrix transfer( UINT, float, bool = true);
		CMatrix rotate( UINT, float, bool = true);
		CMatrix scale( UINT, float, bool = true);
	public:
		bool operator == (const CCoord&);
	};
};