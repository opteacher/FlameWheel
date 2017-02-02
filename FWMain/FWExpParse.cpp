#pragma once
#include "stdafx.h"
#include "FWExpParse.h"
#include <fstream>
using std::ifstream;
using std::vector;
using std::pair;
using std::map;
#include "../OPTools/String.h"
using OPTools::splitString;
using OPTools::strToNum;
using OPTools::numToStr;

const string CFWExpParse::s_cstrMeshIdx("|ME@SH|");
const string CFWExpParse::s_cstrSubIdx("|SUB@MSH|");
const string CFWExpParse::s_cstrMtrlIdx("|MT@RL|");
const string CFWExpParse::s_cstrBoneIdx("|BO@NE|");
const string CFWExpParse::s_cstrVtxWgtIdx("|VER@WGT|");
const string CFWExpParse::s_cstrAnimIdx("|ACT@ION|");
const string CFWExpParse::s_cstrDataIdx("|DA@TA|");

CFWExpParse::PrNamPam CFWExpParse::expMdlFile(string strFileI)
{
	ifstream ifStm( strFileI, std::ios_base::ate);
	if(!ifStm.is_open())	{ return PrNamPam(); }

	// Read whole data from file
	DWORD dwFileSz = (DWORD)ifStm.tellg();
	char* pData = new char[dwFileSz];
	memset( pData, 0, sizeof(char)*dwFileSz);
	ifStm.seekg( 0, std::ios::beg);
	ifStm.read( pData, dwFileSz);
	string strDat(pData);
			
		
	// Find mesh's index
	PrBegEnd prMshIdx;
	prMshIdx.first = strDat.find(s_cstrMeshIdx);
	if(prMshIdx.first == string::npos)	{ prMshIdx.first = 0; }
	UINT* pFmrEnd = &prMshIdx.second;

	// Find subsets' index
	vector<PrBegEnd> aySubIdx;
	UINT uSubIdx = findMultiIdxFromFile( \
		strDat, s_cstrSubIdx, aySubIdx, pFmrEnd, prMshIdx.first);
	if(!aySubIdx.empty())	{ pFmrEnd = &aySubIdx.back().second; }

	// Find materials' index
	vector<PrBegEnd> ayMtlIdx;
	UINT uMtlIdx = findMultiIdxFromFile( \
		strDat, s_cstrMtrlIdx, ayMtlIdx, pFmrEnd, uSubIdx);
	if(!ayMtlIdx.empty())	{ pFmrEnd = &ayMtlIdx.back().second; }

	// Find bones' index
	vector<PrBegEnd> ayBonIdx;
	UINT uBonIdx = findMultiIdxFromFile( \
		strDat, s_cstrBoneIdx, ayBonIdx, pFmrEnd, uMtlIdx);
	if(!ayBonIdx.empty())	{ pFmrEnd = &ayBonIdx.back().second; }

	// Find vertex weights' index
	vector<PrBegEnd> ayVtxWgtIdx;
	UINT uVtxWgtIdx = findMultiIdxFromFile( \
		strDat, s_cstrVtxWgtIdx, ayVtxWgtIdx, pFmrEnd, uBonIdx);
	if(!ayVtxWgtIdx.empty())	{ pFmrEnd = &ayVtxWgtIdx.back().second; }

	// Find actions' index
	vector<PrBegEnd> ayActIdx;
	UINT uActIdx = findMultiIdxFromFile( \
		strDat, s_cstrAnimIdx, ayActIdx, pFmrEnd, uVtxWgtIdx);
	if(!ayActIdx.empty())	{ pFmrEnd = &ayActIdx.back().second; }

	// Find data part
	*pFmrEnd = strDat.find( s_cstrDataIdx, uActIdx);
	string strDatPt = strDat.substr(*pFmrEnd + s_cstrDataIdx.length());
	pvt_ayDataPt = splitString( strDatPt, ';');

	PrNamPam prMdlDat;

	// Split mesh index and collect information
	DWORD dwIdxBeg = prMshIdx.first + s_cstrMeshIdx.length();
	string strMshIdx = strDat.substr(GET_SUB_STR( dwIdxBeg, prMshIdx.second));
	prMdlDat.first = collectMshData( strMshIdx);

	// Split material index and collect
	map<string, CFWParam> mpSknInit;
	for( UINT i = 0; i < ayMtlIdx.size(); ++i)
	{
		PrBegEnd prTmp = ayMtlIdx[i];
		DWORD dwIdxBeg = prTmp.first + s_cstrMtrlIdx.length();
		string strMtlIdx = strDat.substr(GET_SUB_STR( dwIdxBeg, prTmp.second));
		pair<string, CFWParam> prSknInit;
		prSknInit.first = collectMtlData( strMtlIdx, prSknInit.second);
		mpSknInit.insert(prSknInit);
	}

	// Split subset and build mesh initialize parameters
	for( UINT i = 0; i < aySubIdx.size(); ++i)
	{
		// Create a export parameter
		CFWParam pamExp;
		// Allow created meshes weld vertexes and optimize
		pamExp.add( "weldVtx", "true");
		pamExp.add( "optimize", "true");

		// Add vertex color, if existed
		if(pvt_colVtx.beSetCorrect())
		{
			pamExp.add( "vtxCol", pvt_colVtx.convertToStr());
		}

		// Get index
		PrBegEnd prBE = aySubIdx[i];
		UINT uIdxBeg = prBE.first + s_cstrSubIdx.length();
		string strSubIdx = strDat.substr(GET_SUB_STR( uIdxBeg, prBE.second));

		// Collect subset's data and fill in parameter
		string strMtl = collectSubData( strSubIdx, pamExp);

		// If used material was available, insert into export parameter
		typedef map<string, CFWParam>::const_iterator CitrSkn;
		CitrSkn iterFnd = mpSknInit.find(strMtl);
		if(iterFnd != mpSknInit.end())
		{
			pamExp.mrg(iterFnd->second);
		}

		// Add into model data for return
		prMdlDat.second.push_back(pamExp);
	}

	return prMdlDat;
}

string CFWExpParse::getSlgDatByIdx(string strIdxI)
{
	if(strIdxI.empty())	{ return ""; }
	DWORD dwIdx = strToNum<DWORD>(strIdxI);
	if(dwIdx > pvt_ayDataPt.size())	{ return ""; }

	return pvt_ayDataPt[dwIdx];
}

vector<string> CFWExpParse::getMulDatByIdx(string strRagIdxI)
{
	vector<string> ayRet;
	if(strRagIdxI.empty())	{ return ayRet; }
	UINT uSplitFg = strRagIdxI.find('-');
	string strTmp = strRagIdxI.substr( 0, uSplitFg);
	DWORD dwIdxBeg = strToNum<DWORD>(strTmp);
	strTmp = strRagIdxI.substr(uSplitFg + 1);
	DWORD dwIdxEnd = strToNum<DWORD>(strTmp);
	if(dwIdxBeg > pvt_ayDataPt.size()
	|| dwIdxEnd > pvt_ayDataPt.size()
	|| dwIdxBeg > dwIdxEnd)	{ return ayRet; }

	for( DWORD i = dwIdxBeg; i <= dwIdxEnd; ++i)
	{
		ayRet.push_back(pvt_ayDataPt[i]);
	}
	return ayRet;
}

UINT CFWExpParse::findMultiIdxFromFile( string strDatI, string strIdxI, vector<PrBegEnd>& ayLocI, UINT* pFmrEndO, UINT uLocFmrI)
{
	UINT uIdxTmp = strDatI.find( strIdxI, uLocFmrI);
	while(uIdxTmp != string::npos)
	{
		if(!ayLocI.empty())
		{
			ayLocI.back().second = uIdxTmp;
		}
		ayLocI.push_back(PrBegEnd( uIdxTmp, 0));
		uIdxTmp = strDatI.find( strIdxI, uIdxTmp + strIdxI.length());
	}
	if(!ayLocI.empty())
	{
		*pFmrEndO = ayLocI.front().first;
		return ayLocI.back().first;
	}
	else
	{
		return uLocFmrI;
	}
}

SFWVertex CFWExpParse::cvtStrToVertex(string strI)
{
	SFWVertex vtxRet;
	//split the XYZ
	AryStr ayXYZ = ::splitString( strI, ',');
	if(ayXYZ.size() != 3)	{ return vtxRet; }

	//convert to number value
	vtxRet.x = ::strToNum<float>(ayXYZ[0]);
	vtxRet.y = ::strToNum<float>(ayXYZ[1]);
	vtxRet.z = ::strToNum<float>(ayXYZ[2]);
	return vtxRet;
}

SFWColor CFWExpParse::cvtStrToColor(string strI)
{
	//split the color value
	AryStr aColNodes = splitString( strI, ':');
	if(aColNodes.size() < 3)	{ return SFWColor(); }

	//convert to number value
	SFWColor colRet;
	colRet.m_iRed = strToNum<int>(aColNodes[0]);
	colRet.m_iGreen = strToNum<int>(aColNodes[1]);
	colRet.m_iBlue = strToNum<int>(aColNodes[2]);
	if(aColNodes.size() == 4)
	{
		colRet.m_iAlph = strToNum<int>(aColNodes[3]);
	}
	else	{ colRet.m_iAlph = 255; }

	return colRet;
}

string CFWExpParse::collectMshData(string strIdxI)
{
	// Pop last element, because the last element was be spited by last ';'
	AryStr ayMshIdx = splitString( strIdxI, ';');	ayMshIdx.pop_back();

	// If user defined name, reset model's name
	string strNam = getSlgDatByIdx(ayMshIdx[0]);

	// Collect vertexes' data[N]
	AryStr ayRagDat = getMulDatByIdx(ayMshIdx[1]);
	for( DWORD i = 0; i < ayRagDat.size(); ++i)
	{
		pvt_ayVtx.push_back(cvtStrToVertex(ayRagDat[i]));
	}

	// Collect vertexes' color data
	ayRagDat = getMulDatByIdx(ayMshIdx[2]);
	if(!ayRagDat.empty())
	{
		// +_+: This FW version only can process one color mesh
		// it will take the first color as the whole mesh's color
		pvt_colVtx = cvtStrToColor(ayRagDat[0]);
	}

	return strNam;
}

string CFWExpParse::collectSubData( string strIdxI, CFWParam& rPamO)
{
	if(strIdxI.empty())	{ return ""; }

	// Pop last element, because the last element was be spited by last ';'
	AryStr aySubIdx = splitString( strIdxI, ';');	aySubIdx.pop_back();

	// Get sub id[N]
	UINT uSubID = 0;
	DWORD dwIdIdx = strToNum<DWORD>(aySubIdx[0]);
	if(dwIdIdx < pvt_ayDataPt.size())
	{
		uSubID = strToNum<UINT>(pvt_ayDataPt[dwIdIdx]);
	}

	// Get indexes[N] to build vertexes' buffer
	AryStr ayIdxDat = getMulDatByIdx(aySubIdx[1]);
	DWORD dwNumIdxs = ayIdxDat.size();
	SFWVertex* pVtxs = new SFWVertex[dwNumIdxs];
	memset( pVtxs, NULL, sizeof(SFWVertex)*dwNumIdxs);
	for( DWORD i = 0; i < dwNumIdxs; ++i)
	{
		WORD wVtxIdx = strToNum<WORD>(ayIdxDat[i]);
		pVtxs[i] = pvt_ayVtx[wVtxIdx];
	}
	string strNumIdxs = numToStr<DWORD>(dwNumIdxs);
	rPamO.add( "aryPois", numToStr<DWORD>((DWORD)pVtxs));
	rPamO.add( "numPois", strNumIdxs);

	// Create simple indexes' buffer
	WORD* pIdxs = new WORD[dwNumIdxs];
	for( WORD i = 0; i < dwNumIdxs; ++i)	{ pIdxs[i] = i; }
	rPamO.add( "aryIdxs", numToStr<DWORD>((DWORD)pIdxs));
	rPamO.add( "numIdxs", strNumIdxs);

	// Get used material name
	string strMtlNam = "";
	if(!aySubIdx[2].empty())
	{
		DWORD dwNamIdx = strToNum<DWORD>(aySubIdx[2]);
		if(dwNamIdx < pvt_ayDataPt.size())
		{
			strMtlNam = pvt_ayDataPt[dwNamIdx];
		}
	}
	
	// Get normals array
	SFWVertex* pNmls = NULL;
	if(!aySubIdx[3].empty())
	{
		AryStr ayNmlsDat = getMulDatByIdx(aySubIdx[3]);
		// When the model has only one subset, 
		// normals' number will be same to vertexes' index
		DWORD dwNumNmls = dwNumIdxs;
		pNmls = new SFWVertex[dwNumNmls];
		memset( pNmls, NULL, sizeof(SFWVertex)*dwNumNmls);
		for( DWORD i = 0; i < ayNmlsDat.size(); ++i)
		{
			pNmls[i] = cvtStrToVertex(ayNmlsDat[i]);
		}
		rPamO.add( "aryNmls", numToStr<DWORD>((DWORD)pNmls));
	}

	// Get UV map
	float* pTexUV = NULL;
	if(!aySubIdx[4].empty())
	{
		AryStr ayTexUV = getMulDatByIdx(aySubIdx[4]);
		// When the model has only one subset, texture 
		// UVs' number will be twice than vertexes' index
		DWORD dwNumUV = 2 * dwNumIdxs;
		pTexUV = new float[dwNumUV];
		memset( pTexUV, 0, sizeof(float)*dwNumUV);
		for( DWORD i = 0; i < ayTexUV.size(); ++i)
		{
			PrTexUV prUV = splitTexUV(ayTexUV[i]);
			pTexUV[i]		= prUV.first;
			pTexUV[i + 1]	= prUV.second;
		}
		rPamO.add( "texUV", numToStr<DWORD>((DWORD)pTexUV));
	}

	return strMtlNam;
}

string CFWExpParse::collectMtlData( string strIdxI, CFWParam& rPamO)
{
	if(strIdxI.empty())	{ return ""; }

	// Pop last element, because the last element was be spited by last ';'
	AryStr ayMtlIdx = splitString( strIdxI, ';');	ayMtlIdx.pop_back();

	// Get material name
	DWORD dwIdx = strToNum<DWORD>(ayMtlIdx[0]);
	string strMtl = pvt_ayDataPt[dwIdx];

	// Get all color for material
	rPamO.add( "mtlAmb", getColByIdx(ayMtlIdx[1]));
	rPamO.add( "mtlDif", getColByIdx(ayMtlIdx[2]));
	rPamO.add( "mtlSpc", getColByIdx(ayMtlIdx[3]));
	
	// All mesh subsets used this material
	// +_+: For now, it wont be used
	//AryStr aySubIDs = getMulDatByIdx(ayMtlIdx[4]);

	// Get all texture file name
	addTexFile( ayMtlIdx[4], "ambTex", rPamO);
	addTexFile( ayMtlIdx[5], "difTex", rPamO);
	addTexFile( ayMtlIdx[6], "spcTex", rPamO);
	addTexFile( ayMtlIdx[7], "obcTex", rPamO);
	addTexFile( ayMtlIdx[8], "bmpTex", rPamO);
	addTexFile( ayMtlIdx[9], "relTex", rPamO);
	addTexFile( ayMtlIdx[10], "refTex", rPamO);
	addTexFile( ayMtlIdx[11], "dspTex", rPamO);

	return strMtl;
}

pair<float, float> CFWExpParse::splitTexUV(string strI)
{
	PrTexUV prRet;
	// Split U and V
	UINT uSplFg = strI.find('&');
	if(uSplFg == string::npos)	{ return prRet; }
	prRet.first = strToNum<float>(strI.substr( 0, uSplFg));
	prRet.second = strToNum<float>(strI.substr(uSplFg + 1));
	return prRet;
}

string CFWExpParse::getColByIdx(string strIdxI)
{
	if(strIdxI.empty())	{ return ""; }
	DWORD dwIdx = strToNum<DWORD>(strIdxI);
	if(dwIdx >= pvt_ayDataPt.size())	{ return ""; }
	string strTmp = pvt_ayDataPt[dwIdx];
	SFWColor colTmp = cvtStrToColor(strTmp);
	return colTmp.convertToStr();
}

void CFWExpParse::addTexFile( string strIdxI, string strKeyI, CFWParam& rPamO)
{
	if(!strIdxI.empty())
	{
		DWORD dwIdx = strToNum<DWORD>(strIdxI);
		if(dwIdx < pvt_ayDataPt.size())
		{
			rPamO.add( strKeyI, pvt_ayDataPt[dwIdx]);
		}
	}
}