#pragma once
#include "Point.h"
#include "Vector.h"
#include "Rigid.h"

namespace OPMath
{
	class CCone: public CRigid
	{
	protected:
		CPoint m_poiOrg;
		float m_fRange;
		CVector m_vecDir;
		float m_fAngRad;
	public:
		CCone(): m_fRange(0.0f), m_fAngRad(0.0f)	{}
		CCone(const CCone& refConI): CRigid(refConI), \
			m_poiOrg(refConI.m_poiOrg), m_vecDir(refConI.m_vecDir), \
			m_fRange(refConI.m_fRange), m_fAngRad(refConI.m_fAngRad)	{}
		CCone( CPoint poiOrgI, CVector vecDirI, float fRangeI, float fAngRadI);
	public:
		bool isInside(CPoint poiI);
	};

	class CSqCone: public CRigid
	{
	public:
		enum
		{
			PLN_CLS,	PLN_FAR,
			PLN_LFT,	PLN_RGT,
			PLN_TOP,	PLN_BTM
		};
	public:
		virtual void mkInside(CPoint poiI);
	};
};