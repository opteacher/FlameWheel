#pragma once
#pragma warning(disable: 4265)
#include "windows.h"
#include "math.h"
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"
#include "Plane.h"
#include "Triangle.h"
#include "RegTgl.h"
#include "Coord.h"
#include "Rect.h"
#include "Line2D.h"
#include "Rigid.h"
#include "Cone.h"
#include "Polyg2D.h"
#include "BoxAABB.h"
#include "Sphere.h"

namespace OPMath
{
	bool isOSSupportsSSE();

	extern const double MAX_VAL;
	extern const double MIN_VAL;
	extern const double FW_PI;

	template<typename T> bool between( T tValChkI, T tValAI, T tValBI, bool bEqualInI)
	{
		T tValMax = max( tValAI, tValBI);
		T tValMin = min( tValAI, tValBI);
		if(bEqualInI)
		{
			return(tValChkI >= tValMin && tValChkI <= tValMax);
		}
		else
		{
			return(tValChkI > tValMin && tValChkI < tValMax);
		}
	}

	template<typename T> int getSign(T tValI)
	{
		return((int)(tValI/abs(tValI)));
	}

	template<typename T> T regulate(T tValI)
	{
		T tValPos = abs(tValI);
		//check whether equal to zero
		if(tValPos < MIN_VAL)
		{
			return(0);
		}
		
		T tValDecl = tValPos - (UINT)tValPos;
		T tValRet = tValI;
		//check whether equal some value
		if(tValDecl < 0.001)
		{
			tValRet = (T)((UINT)tValPos);
		}
		else if(tValDecl + 0.001 >= 1)
		{
			tValRet = (T)((UINT)tValI + 1);
		}

		return((tValI > 0) ? tValRet : -tValRet);
	}

	template<typename T> bool equals( T tValAI, T tValBI)	{ return(abs(tValAI - tValBI) < MIN_VAL); }

	template<typename T> bool isZero(T tValI)	{ return(abs(tValI) < MIN_VAL); }
	
	template<typename T> bool isInt(T tValI)
	{
		T tValReg = ::regulate<T>(tValI);
		UINT uValPos = (UINT)tValI;
		T tValDecl = tValI - uValPos;
		return(::isZero<T>(tValDecl));
	}

	template<typename T> T geneRandVal( T fMinValI, T fMaxValI)
	{
		if(fMinValI == fMaxValI)	{ return(fMinValI); }
		return((fMinValI > fMaxValI) ? \
			(T)((double)rand()/(RAND_MAX + 1)*(fMinValI - fMaxValI) + fMaxValI) : \
			(T)((double)rand()/(RAND_MAX + 1)*(fMaxValI - fMinValI) + fMinValI));
	}

	DWORD geneHashVal(const char* strValI = NULL);

	inline bool geneRandBool()
	{
		return((rand() % 2) ? true : false);
	}
	
	float sqrtSEE(float fValI);
	
	//+_+:input radian angle
	float sinEx(float fAngI);
	
	//+_+:input radian angle
	float cosEx(float fValI);

	float disBetweenPoi( const CPoint& poiAI, const CPoint& poiBI);
};