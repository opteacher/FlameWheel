#pragma once
#include "FWModel.h"
#include "../OPMath/Math.h"
using namespace OPMath;

class FWMAIN_API CFWTerrain: public CFWModel
{
public: friend class CFWMdlMngr;
public:
	CFWTerrain(): CFWModel("Terrain" + numToStr<DWORD>(s_stUsedNams.size())),
		ptd_fLength(0), ptd_fWidth(0), ptd_uLenSec(0), ptd_uWidSec(0),
		ptd_uHgtWid(0), ptd_uHgtHgt(0), pvt_fC1(0), pvt_fC2(0),
		pvt_pTglLT(NULL), pvt_pTglRB(NULL)	{}
protected:
	float ptd_fLength;
	float ptd_fWidth;
	UINT ptd_uLenSec;
	UINT ptd_uWidSec;
	UINT ptd_uHgtWid;
	UINT ptd_uHgtHgt;
	std::vector<float> ptd_ayHgt;
	// Terrain's LW to height's WH rate
	// first: L->W; second: W->H;
	std::pair<float, float> ptd_prTrnHgt;
protected:
	struct SFWTglInf
	{
		typedef std::pair<SFWTglInf*, UINT> PrAdj;
		enum EFWAdjEdg
		{
			ADJ_NULL,
			ADJ_LEFT,
			ADJ_RIGHT,
			ADJ_BOTTOM
		};
		// PrAdj( NULL, ADJ_NULL)
		static const PrAdj s_cprNoTgl;

		SFWTglInf():
			m_poiTop(0), m_poiLft(0),
			m_poiRgt(0), m_poiCen(0),
			m_pLftSub(NULL), m_pRgtSub(NULL),
			m_pTglBas(NULL), m_bSplit(false)	{}

		// Three vertexes' index
		WORD m_poiTop, m_poiLft, m_poiRgt, m_poiCen;
			
		// Adjacent edges triangles
		//+_+: second: defined which edge triangle(not self) adj
		PrAdj m_tglLftAdj, m_tglRgtAdj, m_tglBtmAdj;

		// Base triangle
		SFWTglInf* m_pTglBas;
		// Sub triangle
		SFWTglInf* m_pLftSub;
		SFWTglInf* m_pRgtSub;
		
		// Whether be split
		bool m_bSplit;

		// Vertex array
		static std::vector<SFWVertex> s_ayVtxs;
	};
	typedef SFWTglInf::EFWAdjEdg EFWAdjEdg;
	typedef SFWTglInf::PrAdj PrAdj;
private:
	// Distance Between rectangle center and eye position /
	// rectangle's length
	float pvt_fC1;
	// Distance Between rectangle center and eye position /
	// height distance of center point to edge center
	float pvt_fC2;
	CPoint pvt_posEye;
	CSqCone pvt_cnView;
	SFWTglInf* pvt_pTglLT;
	SFWTglInf* pvt_pTglRB;
private:
	void splitTgl( SFWTglInf*, bool = false);
	void splitBtmAdj(SFWTglInf*);
	std::vector<WORD> buildIdxByTgl(SFWTglInf*);
	inline float getHgtByLoc( float fXI, float fZI);
protected:
	virtual CFWParam genFixdTrnInitPam();
	virtual CFWParam genAutoTrnInitPam();
	static std::vector<float> loadHgtMap(
		string strFileI, UINT uWidI, UINT uHgtI, float fSclI);
public: CHK_IS_CLASS;
};