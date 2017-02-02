#pragma once
#include "windows.h"
#include "FWMacro.h"
#include <string>
using std::string;

struct FWMAIN_API SFWColor
{
	enum EColMode
	{
		EMC_A8R8G8B8,
		EMC_X8R8G8B8,
		EMC_R5G6B5,
		EMC_R8G8B8
	};

	// +_+: define as 0~255
	int m_iRed;
	int m_iGreen;
	int m_iBlue;
	int m_iAlph;

	SFWColor(): m_iRed(-1), m_iGreen(-1), m_iBlue(-1), m_iAlph(255)	{}

	// Use 0~255 range to build
	SFWColor( int iRI, int iGI, int iBI, int iAI = 255): \
		m_iRed(iRI), \
		m_iGreen(iGI), \
		m_iBlue(iBI), \
		m_iAlph(iAI)	{}

	// Use 0~1 range to build
	SFWColor( float fRI, float fGI, float fBI, float fAI = 1.0f): \
		m_iRed((int)(fRI * 255)), \
		m_iGreen((int)(fGI * 255)), \
		m_iBlue((int)(fBI * 255)), \
		m_iAlph((int)(fAI * 255))	{}

	// Use DWORD to build(how to convert to this structure depend on color mode)
	SFWColor( DWORD dwColValI, EColMode uColModI = EMC_A8R8G8B8);

	inline bool operator==(const SFWColor& rColI) const;
	inline bool operator!=(const SFWColor& rColI) const;
	// Compare color, lighter or darker
	inline bool operator<(const SFWColor& rColI) const;

	// Check the color whether be set correct value,
	// not just be created as default construct
	inline bool beSetCorrect() const;

	// Only support A8R8G8B8 mode, the others will be added, when they are needed
	inline DWORD getDword() const;

	// Use 255 to subtract every color parts
	inline SFWColor getNegCol() const;

	// Change color's depth
	SFWColor& chgDepth( float fValI, bool bEffAlphI = false);

	// Generate a random color
	static SFWColor geneRandCol();

	// Convert color to/from string
	// (mode: "R" + RED + "G" + GREEN + "B" + BLUE + "A" + ALPHA)
	string convertToStr() const;
	SFWColor& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
};

FWMAIN_API extern SFWColor g_colWhite;
FWMAIN_API extern SFWColor g_colGray;
FWMAIN_API extern SFWColor g_colBlack;
FWMAIN_API extern SFWColor g_colRed;
FWMAIN_API extern SFWColor g_colGreen;
FWMAIN_API extern SFWColor g_colBlue;
FWMAIN_API extern SFWColor g_colCBlue;
FWMAIN_API extern SFWColor g_colYellow;
FWMAIN_API extern SFWColor g_colCyan;
FWMAIN_API extern SFWColor g_colMagenta;