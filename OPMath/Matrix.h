#pragma once
#include "windows.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"


namespace OPMath
{
	#define DIM_3D 4
	#define DIM_2D 3

	class CMatrix
	{
	public:
		CMatrix(void);
		CMatrix(UINT uDimI);
		CMatrix(const float[DIM_3D][DIM_3D]);
		CMatrix(const CMatrix&);
		CMatrix& operator=(const CMatrix&);
	public:
		CMatrix operator+(const CMatrix&);
		CMatrix operator-(const CMatrix&);
		CMatrix operator*(const float&);
		CMatrix operator-();
	public:
		bool operator==(const CMatrix&);
		bool operator!=(const CMatrix&);
	public:
		CMatrix operator+=(const CMatrix&);
		CMatrix operator-=(const CMatrix&);
		CMatrix dotLeft(const CMatrix&) const;
		CMatrix dotLeftToSelf(const CMatrix&);
		CMatrix dotRight(const CMatrix&) const;
		CMatrix dotRightToSelf(const CMatrix&);
		CPoint multiPoiRight(CPoint) const;
		CPoint multiPoiLeft(CPoint) const;
		CVector multiVecLeft(CVector) const;
		CVector multiVecRight(CVector) const;
	public:
		float m_fVal[DIM_3D][DIM_3D];
		UINT m_uNumDim;
	public:
		float getDetVal();
		CMatrix getDecip();
		CMatrix getAdjMat();
		CMatrix tunIndMat();
		CMatrix getNegMat();
		CMatrix getTrspMat();
	public:
		bool isIndMat();
	public:
		//accept angle, +_+: not radian
		static CMatrix rotate2D(float);
		//accept angle, +_+: not radian
		static CMatrix rotate2D( float, CPoint);
		static CMatrix transfer2D( float, float);
		static CMatrix scale2D( float, float);
		static CMatrix scale2D( float, float, CPoint);
	public:
		static CMatrix transfer3D( float, float, float);
		static CMatrix rotate3D( float, CVector, bool = false/* whether use quate */);
		static CMatrix rotate3D( float, CRay, bool = false/* whether use quate */);
		//matrix rotate from begin to end
		static CMatrix rotate3D( CVector, CVector);
		static CMatrix scale3D( float, float, float, CPoint = CPoint());
	};
};