#pragma once
#include "Point.h"
#include "LnEqua.h"


namespace OPMath
{
	class CLine2D: public ILnEqua
	{
	protected:
		float m_fA;
		float m_fB;
		float m_fD;
	public:
		CLine2D( CPoint poiAI, CPoint poiBI);
	public:
		float getSlope() const;
		float getInterc() const;
	public:
		float mkOutY(float fXI);
		int mkFunType( float fXI, float fYI);
		bool isFillToFun( float fXI, float fYI, float fZI) const;
		float calculate( float fXI, float fYI, float fZI) const;
	};
};