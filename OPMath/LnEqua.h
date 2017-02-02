#pragma once


namespace OPMath
{
	#define LESS -1
	#define EQUAL 0
	#define BIGGER 1
	
	class ILnEqua
	{
	public:
		ILnEqua(): m_iTypeFun(EQUAL), m_bEnbEquals(true)	{}
		ILnEqua(const ILnEqua& refLnEquI):
			m_iTypeFun(refLnEquI.m_iTypeFun),
			m_bEnbEquals(refLnEquI.m_bEnbEquals)	{}
		ILnEqua& operator=(const ILnEqua& refLnEquI)
		{
			m_iTypeFun = refLnEquI.m_iTypeFun;
			m_bEnbEquals = refLnEquI.m_bEnbEquals;
			
			return(*this);
		}
	protected:
		int m_iTypeFun;
		bool m_bEnbEquals;
	public:
		void setFunType(int iFunTpI);
		void enableEquals(bool bEnableI);
		virtual float calculate( float fXI, float fYI, float fZI) const = 0;
		virtual bool isFillToFun( float fXI, float fYI, float fZI) const = 0;
	};
};