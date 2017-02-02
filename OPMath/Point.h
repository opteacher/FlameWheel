#pragma once
#include "Quate.h"


namespace OPMath
{
	class CVector;
	class CPoint: public SQuate
	{
	public:
		CPoint();
		CPoint( float, float, float = 0.0f, float = 1.0f);
		CPoint(const SQuate&);
		CPoint(const CPoint&);
		CPoint(const CVector&);
		CPoint& operator=(const CPoint&);
	public:
		CPoint operator+(const CPoint&) const;
		CPoint operator+=(const CVector&);
		CPoint operator-(const CPoint&) const;
		CPoint operator*(float) const;
		CPoint operator/(float) const;
	public:
		bool operator==(const CPoint&) const;
		bool operator!=(const CPoint&) const;
		bool operator<(const CPoint&) const;
		bool operator>(const CPoint&) const;
	public:
		void regulate();
		bool isZeroPoi() const;
	};
};