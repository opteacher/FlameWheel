#pragma once
#include "FWParam.h"
#include "FWMisc.h"
#include <vector>
#include <map>
#include <set>

class CFWExpParse
{
public:
	static const string s_cstrMeshIdx;//|ME@SH|
	static const string s_cstrSubIdx;//|SUB@MSH|
	static const string s_cstrMtrlIdx;//|MT@RL|
	static const string s_cstrBoneIdx;//|BO@NE|
	static const string s_cstrVtxWgtIdx;//|VER@WGT|
	static const string s_cstrAnimIdx;//|ACT@ION|
	static const string s_cstrDataIdx;//|DA@TA|
public:
	typedef std::pair<UINT, UINT> PrBegEnd;
	typedef std::vector<string> AryStr;
	typedef std::pair<DWORD, string> PrIdxVal;
	typedef std::pair<float, float> PrTexUV;
	typedef std::set<string> StStr;
	typedef std::vector<SFWVertex> AryVtx;
	typedef std::vector<CFWParam> AryPam;
	typedef std::pair<string, AryPam> PrNamPam;
public: PrNamPam expMdlFile(string strFileI);
private:
	UINT findMultiIdxFromFile(
		string strDatI,
		string strIdxI,
		std::vector<PrBegEnd>& ayLocO,
		UINT* pFmrEndO,
		UINT uLocFmrI = 0);
	string getSlgDatByIdx(string strIdxI);
	AryStr getMulDatByIdx(string strRagIdxI);
	static SFWVertex cvtStrToVertex(string strI);
	static SFWColor cvtStrToColor(string strI);
	string collectMshData(string strIdxI);
	// Return used material's name, to maintain to read them
	string collectSubData( string strIdxI, CFWParam& rPamO);
	// Return material's name
	string collectMtlData( string strIdxI, CFWParam& rPamO);
	inline PrTexUV splitTexUV(string strI);
	inline string getColByIdx(string strIdxI);
	inline void addTexFile(
		string strIdxI,//Texture file's index
		string strKeyI,//Texture channel
		CFWParam& rPamO);
private:
	AryStr pvt_ayDataPt;// All data(by string)
	AryVtx pvt_ayVtx;
	SFWColor pvt_colVtx;// Model use one color
};