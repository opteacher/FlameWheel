#pragma once

namespace OPMath
{
	struct SQuate
	{
		float m_fX;
		float m_fY;
		float m_fZ;
		float m_fH;

		SQuate(): m_fX(0.0f), m_fY(0.0f), \
			m_fZ(0.0f), m_fH(1.0f)	{}
		SQuate( float fXI, float fYI, float fZI, float fHI):
			m_fX(fXI), m_fY(fYI), m_fZ(fZI), m_fH(fHI)	{}
	};
}