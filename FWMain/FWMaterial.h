#pragma once
#include "FWMacro.h"
#include "FWColor.h"

struct FWMAIN_API SFWMaterial
{
	SFWColor m_colDiffuse;	//Diffuse color
	SFWColor m_colAmbient;	//Ambient color
	SFWColor m_colSpecular;	//Specular color
	SFWColor m_colEmissive;	//Emissive color
	float m_fPower;			//Power

	SFWMaterial(): 
		m_colDiffuse(SFWColor().geneRandCol()),
		m_colAmbient(m_colDiffuse),
		m_colSpecular(m_colDiffuse),
		m_colEmissive(g_colBlack),
		m_fPower(5.0f)	{}

	SFWMaterial(SFWColor colDif):
		m_colDiffuse(colDif),
		m_colAmbient(colDif),
		m_colSpecular(colDif),
		m_colEmissive(g_colBlack),
		m_fPower(5.0f)	{}
		
	SFWMaterial(
		SFWColor colDifI,
		SFWColor colAmbI,
		SFWColor colSpcI,
		SFWColor colEmsI,
		float fPowerI):
	m_colDiffuse(colDifI),
	m_colAmbient(colAmbI),
	m_colSpecular(colSpcI),
	m_colEmissive(colEmsI),
	m_fPower(fPowerI)	{}

	inline bool operator==(const SFWMaterial&) const;
	inline bool operator!=(const SFWMaterial&) const;
	
	inline SFWMaterial getNegMtl() const;

	// Convert color to/from string
	// (mode:	"D[R" + RED + "G" + GREEN + "B" + BLUE + "A" + ALPHA + "]" + 
	//			"A[R" + RED + "G" + GREEN + "B" + BLUE + "A" + ALPHA + "]" +
	//			"S[R" + RED + "G" + GREEN + "B" + BLUE + "A" + ALPHA + "]" +
	//			"E[R" + RED + "G" + GREEN + "B" + BLUE + "A" + ALPHA + "]" + "P" + POWER)
	string convertToStr() const;
	SFWMaterial& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
private:
	static std::string::size_type collectColFromStr(
		string strI,//String source
		string::size_type& rFndOff,//From where find color
		string strColI,//Color name
		SFWColor& rCol);//Input color
};