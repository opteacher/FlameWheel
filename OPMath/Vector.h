#pragma once
#include "Point.h"

namespace OPMath
{
	class CVector: public CPoint
	{
	public:
		CVector();
		CVector( float, float, float = 0.0f, float = 1.0f);
		CVector( const CPoint& poiBegI, const CPoint& poiEndI);
		CVector(const SQuate&);
		CVector(const CVector&);
		CVector(const CPoint&);
	public:
		CVector operator=(const CVector&);
		CVector operator+(const CVector&);
		CVector operator-(const CVector&);
		CVector operator*(float);
		CVector operator/(float);
		CVector operator-();
		CVector dot(const CVector&) const;//dot multi
		CVector cross(const CVector&) const;//cross multi
		bool equal(const CVector vecI);
		float sun() const;//x+y+z
	protected:
		float m_fLength;//len of the vector
	public:
		CVector getNegVec() const;//get negative vector
		bool isZeroVec() const;//check zero vector
		bool isNorVec();//check normal vector
		CVector getIndVec() const;//get normalized vector(will not change self)
		float normalize();//make length to 1
		//get vector length(param identify whether recalculate the length)
		float getVecLen(bool = false);
		void clear();//make vector to zero
	public:
		//the second param identify whether return radian, default return radian
		float angWithVec( CVector, bool = true);
		float cosWithVec( const CVector&);
		bool isParallel(const CVector&) const;
		bool isVertical(const CVector&) const;
	public:
		bool isSameDir_VerticalIn(CVector vecI);
		bool isSameDir_VerticalOut(CVector vecI);
	};
};