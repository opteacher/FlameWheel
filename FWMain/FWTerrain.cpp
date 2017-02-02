#pragma once
#include "stdafx.h"
#include "FWTerrain.h"
#include "FWCamIntf.h"
#include "FWFactory.h"
#include "FWManager.h"
using std::vector;

INI_TYPID(CFWTerrain);

const CFWTerrain::SFWTglInf::PrAdj CFWTerrain::SFWTglInf::s_cprNoTgl( NULL, ADJ_NULL);
std::vector<SFWVertex> CFWTerrain::SFWTglInf::s_ayVtxs = std::vector<SFWVertex>();

void CFWTerrain::splitTgl( SFWTglInf* pTglI, bool bFocSplI)
{
	if(pTglI == NULL)	{ return; }
	if(pTglI->m_bSplit)
	{
		if(bFocSplI)	{ return; }
		splitTgl(pTglI->m_pLftSub);
		splitTgl(pTglI->m_pRgtSub);
	}

	// Collect data for checking split
	SFWVertex vtxLft = SFWTglInf::s_ayVtxs[pTglI->m_poiLft];
	CPoint poiLft = CPoint( vtxLft.x, vtxLft.y, vtxLft.z);
	SFWVertex vtxRgt = SFWTglInf::s_ayVtxs[pTglI->m_poiRgt];
	CPoint poiRgt = CPoint( vtxRgt.x, vtxRgt.y, vtxRgt.z);
	SFWVertex vtxTop = SFWTglInf::s_ayVtxs[pTglI->m_poiTop];
	CPoint poiTop = CPoint( vtxTop.x, vtxTop.y, vtxTop.z);

	// All three vertex are out of the view cone,
	// don't needs build the terrain
	//if(!pvt_cnView.isInside(poiLft)
	//&& !pvt_cnView.isInside(poiRgt)
	//&& !pvt_cnView.isInside(poiTop))	{ return false; }

	// Get distance between top point and eye location
	float fDisVw = CVector( poiTop, pvt_posEye).getVecLen();
	// Get triangle's bottom length
	float fLenEdg = CVector( poiLft, poiRgt).getVecLen();
	// Get center point on the bottom edge
	CPoint poiCen = (poiLft + poiRgt)*.5;
	// Get height from height map
	float fMaxHgt = 0, fMinHgt = 0;
	poiTop.m_fY = getHgtByLoc( poiTop.m_fX, poiTop.m_fZ);
	if(poiTop.m_fY > fMaxHgt)	{ fMaxHgt = poiTop.m_fY; }
	if(poiTop.m_fY < fMinHgt)	{ fMinHgt = poiTop.m_fY; }
	poiLft.m_fY = getHgtByLoc( poiLft.m_fX, poiLft.m_fZ);
	if(poiLft.m_fY > fMaxHgt)	{ fMaxHgt = poiLft.m_fY; }
	if(poiLft.m_fY < fMinHgt)	{ fMinHgt = poiLft.m_fY; }
	poiRgt.m_fY = getHgtByLoc( poiRgt.m_fX, poiRgt.m_fZ);
	if(poiRgt.m_fY > fMaxHgt)	{ fMaxHgt = poiRgt.m_fY; }
	if(poiRgt.m_fY < fMinHgt)	{ fMinHgt = poiRgt.m_fY; }
	//poiCen.m_fY = getHgtByLoc( poiCen.m_fX, poiCen.m_fZ);
	//if(poiCen.m_fY > fMaxHgt)	{ fMaxHgt = poiCen.m_fY; }
	//if(poiCen.m_fY < fMinHgt)	{ fMinHgt = poiCen.m_fY; }
	// Calculate waviness degree with the top and bottom center
	float fWavDeg = fMaxHgt - fMinHgt;

	// Check whether need to split(L / (D*R*C1*C2) < 1)
	if((fDisVw / (fLenEdg*fWavDeg*pvt_fC1*pvt_fC2)) < 1 || bFocSplI)
	{
		//if(!bFocSplI)	{ ++g_iNumSplit; }

		// Add the center point into vertex array
		SFWTglInf* pTglAdj = pTglI->m_tglBtmAdj.first;
		if(pTglAdj != NULL && pTglAdj->m_bSplit)
		{
			pTglI->m_poiCen = pTglAdj->m_poiCen;
		}
		else
		{
			pTglI->m_poiCen = SFWTglInf::s_ayVtxs.size();
			SFWTglInf::s_ayVtxs.push_back(SFWVertex(poiCen));
		}

		// Split the triangle
		pTglI->m_bSplit = true;
		pTglI->m_pLftSub = new SFWTglInf();
		pTglI->m_pLftSub->m_pTglBas = pTglI;
		pTglI->m_pLftSub->m_poiTop = pTglI->m_poiCen;
		pTglI->m_pLftSub->m_poiLft = pTglI->m_poiTop;
		pTglI->m_pLftSub->m_poiRgt = pTglI->m_poiLft;
		if(pTglI->m_tglLftAdj.first != NULL && pTglI->m_tglLftAdj.first->m_bSplit)
		{
			SFWTglInf* pLftAdj = pTglI->m_tglLftAdj.first;
			pTglI->m_pLftSub->m_tglBtmAdj = PrAdj( pLftAdj->m_pRgtSub, EFWAdjEdg::ADJ_BOTTOM);
			pLftAdj->m_pRgtSub->m_tglBtmAdj = PrAdj( pTglI->m_pLftSub, EFWAdjEdg::ADJ_BOTTOM);
		}
		else
		{
			pTglI->m_pLftSub->m_tglBtmAdj = pTglI->m_tglLftAdj;
		}

		pTglI->m_pRgtSub = new SFWTglInf();
		pTglI->m_pRgtSub->m_pTglBas = pTglI;
		pTglI->m_pRgtSub->m_poiTop = pTglI->m_poiCen;
		pTglI->m_pRgtSub->m_poiLft = pTglI->m_poiRgt;
		pTglI->m_pRgtSub->m_poiRgt = pTglI->m_poiTop;
		if(pTglI->m_tglRgtAdj.first != NULL && pTglI->m_tglRgtAdj.first->m_bSplit)
		{
			SFWTglInf* pRgtAdj = pTglI->m_tglRgtAdj.first;
			pTglI->m_pRgtSub->m_tglBtmAdj = PrAdj( pRgtAdj->m_pLftSub, EFWAdjEdg::ADJ_BOTTOM);
			pRgtAdj->m_pLftSub->m_tglBtmAdj = PrAdj( pTglI->m_pRgtSub, EFWAdjEdg::ADJ_BOTTOM);
		}
		else
		{
			pTglI->m_pRgtSub->m_tglBtmAdj = pTglI->m_tglRgtAdj;
		}

		pTglI->m_pLftSub->m_tglLftAdj = PrAdj( pTglI->m_pRgtSub, EFWAdjEdg::ADJ_RIGHT);
		pTglI->m_pRgtSub->m_tglRgtAdj = PrAdj( pTglI->m_pLftSub, EFWAdjEdg::ADJ_LEFT);

		// Check and split bottom adjacent triangle
		if(pTglI->m_tglBtmAdj.first)	{ splitBtmAdj(pTglI); }

		if(!bFocSplI)
		{
			// Split subset
			splitTgl(pTglI->m_pLftSub);
			splitTgl(pTglI->m_pRgtSub);
		}
	}
}

void CFWTerrain::splitBtmAdj(SFWTglInf* pTglI)
{
	SFWTglInf* pAdjTgl = pTglI->m_tglBtmAdj.first;
	if(pAdjTgl == NULL)	{ return; }
	switch (pTglI->m_tglBtmAdj.second)
	{
	case EFWAdjEdg::ADJ_BOTTOM:
		if(pAdjTgl->m_bSplit)
		{
			// Relate the sub triangle's LR adjacent
			pTglI->m_pLftSub->m_tglRgtAdj = PrAdj( pAdjTgl->m_pRgtSub, EFWAdjEdg::ADJ_LEFT);
			pTglI->m_pRgtSub->m_tglLftAdj = PrAdj( pAdjTgl->m_pLftSub, EFWAdjEdg::ADJ_RIGHT);
			pAdjTgl->m_pRgtSub->m_tglLftAdj = PrAdj( pTglI->m_pLftSub, EFWAdjEdg::ADJ_RIGHT);
			pAdjTgl->m_pLftSub->m_tglRgtAdj = PrAdj( pTglI->m_pRgtSub, EFWAdjEdg::ADJ_LEFT);
		}
		else
		{
			// Forced split the adjacent triangle
			splitTgl( pAdjTgl, true);
		}
		break;
	case EFWAdjEdg::ADJ_LEFT:
		splitTgl( pAdjTgl, true);
		splitTgl( pAdjTgl->m_pLftSub, true);
		break;
	case EFWAdjEdg::ADJ_RIGHT:
		splitTgl( pAdjTgl, true);
		splitTgl( pAdjTgl->m_pRgtSub, true);
		break;
	}
}

std::vector<WORD> CFWTerrain::buildIdxByTgl(SFWTglInf* pTglI)
{
	vector<WORD> ayIdxs;
	if(pTglI == NULL)	{ return ayIdxs; }

	if(!pTglI->m_bSplit)
	{
		ayIdxs.push_back(pTglI->m_poiLft);
		ayIdxs.push_back(pTglI->m_poiTop);
		ayIdxs.push_back(pTglI->m_poiRgt);
		return ayIdxs;
	}
	else
	{
		vector<WORD> aySub;
		aySub = buildIdxByTgl(pTglI->m_pLftSub);
		std::copy( aySub.begin(), aySub.end(), std::back_inserter(ayIdxs));
		aySub = buildIdxByTgl(pTglI->m_pRgtSub);
		std::copy( aySub.begin(), aySub.end(), std::back_inserter(ayIdxs));
	}

	return ayIdxs;
}

float CFWTerrain::getHgtByLoc( float fXI, float fZI)
{
	UINT uHgtWid = (UINT)(fXI + ptd_fLength*.5) * ptd_prTrnHgt.first;
	UINT uHgtHgt = (UINT)(fZI + ptd_fWidth*.5) * ptd_prTrnHgt.second;
	if(uHgtWid == ptd_uHgtWid)	{ --uHgtWid; }
	if(uHgtHgt == ptd_uHgtHgt)	{ --uHgtHgt; }
	DWORD dwLocHgt = uHgtWid*ptd_uHgtHgt + uHgtHgt;
	if(dwLocHgt > ptd_ayHgt.size())
	{
		throw std::runtime_error("Error member: error rate of Terrain to Height");
	}
	return ptd_ayHgt[dwLocHgt];
}

CFWParam CFWTerrain::genFixdTrnInitPam()
{
	CFWParam pamIniMsh = CFWMshFcty::GenPlnInitPam(
		ptd_fLength, ptd_fWidth, ptd_uLenSec, ptd_uWidSec);
	SFWVertex* pVtx = (SFWVertex*)pamIniMsh.getDword("aryPois");
	DWORD dwNumVtx = pamIniMsh.getDword("numPois");

	// Fill in the height
	UINT uVtxCol = ptd_uLenSec + 1;
	UINT uVtxRow = ptd_uWidSec + 1;
	float fWidRate = ptd_uHgtWid / ptd_uLenSec;
	float fHgtRate = ptd_uHgtHgt / ptd_uWidSec;
	for( UINT j = 0; j < uVtxRow; ++j)
	{
		for( UINT i = 0; i < uVtxCol; ++i)
		{
			DWORD dwIdxVtx = j*uVtxCol + i;
			if(dwIdxVtx >= dwNumVtx)
			{
				throw std::runtime_error("Error index: Out of vertex array!");
			}
			DWORD dwIdxHgt = i*fWidRate*ptd_uHgtHgt + j*fHgtRate;
			if(dwIdxHgt >= ptd_ayHgt.size())
			{
				throw std::runtime_error("Error index: Out of height map!");
			}
			pVtx[dwIdxVtx].y = ptd_ayHgt[dwIdxHgt];
		}
	}

	return pamIniMsh;
}

CFWParam CFWTerrain::genAutoTrnInitPam()
{
	// Get operation camera for building terrain
	CFWCamIntf* pOprCam = (CFWCamIntf*)CFWCamFcty::getInstance()->getOprCam();
	if(pOprCam == NULL)	{ return CFWParam(); }

	float fHlfLen = ptd_fLength * .5;
	float fHlfWid = ptd_fWidth * .5;
	CPoint poiLT( fHlfLen,	getHgtByLoc( fHlfLen, -fHlfWid),	-fHlfWid);
	CPoint poiRT( -fHlfLen,	getHgtByLoc( -fHlfLen, -fHlfWid),	-fHlfWid);
	CPoint poiLB( fHlfLen,	getHgtByLoc( fHlfLen, fHlfWid),		fHlfWid);
	CPoint poiRB( -fHlfLen,	getHgtByLoc( -fHlfLen, fHlfWid),	fHlfWid);
	// All four vertex are out of the view cone,
	// don't needs build the terrain
	//pvt_cnView = pOprCam->GetViewCone();
	//if(!pvt_cnView.isInside(poiLT)
	//&& !pvt_cnView.isInside(poiRT)
	//&& !pvt_cnView.isInside(poiLB)
	//&& !pvt_cnView.isInside(poiRB)
	//&& !pvt_cnView.isInside(poiCen))	{ return CFWParam(); }

	// Push back four points of rectangle
	SFWTglInf::s_ayVtxs.push_back(poiLT);//0
	SFWTglInf::s_ayVtxs.push_back(poiRT);//1
	SFWTglInf::s_ayVtxs.push_back(poiLB);//2
	SFWTglInf::s_ayVtxs.push_back(poiRB);//3
	pvt_pTglLT = new SFWTglInf();
	pvt_pTglRB = new SFWTglInf();
	pvt_pTglLT->m_poiTop = 0;
	pvt_pTglLT->m_poiLft = 2;
	pvt_pTglLT->m_poiRgt = 1;
	pvt_pTglLT->m_tglBtmAdj = PrAdj( pvt_pTglRB, EFWAdjEdg::ADJ_BOTTOM);
	pvt_pTglRB->m_poiTop = 3;
	pvt_pTglRB->m_poiLft = 1;
	pvt_pTglRB->m_poiRgt = 2;
	pvt_pTglRB->m_tglBtmAdj = PrAdj( pvt_pTglLT, EFWAdjEdg::ADJ_BOTTOM);

	// Calculate distance to eye position
	CMatrix matTsf = ptd_spcLoc.GetWldTrfMat();
	pvt_posEye = matTsf.multiPoiLeft(pOprCam->GetEyeLoc());

	// Check the first function(L/D < C1)
	splitTgl(pvt_pTglLT);
	splitTgl(pvt_pTglRB);

	// Input data into memory
	DWORD dwNumVtx = SFWTglInf::s_ayVtxs.size();
	SFWVertex* pVtxs = new SFWVertex[dwNumVtx];
	memcpy( pVtxs, &SFWTglInf::s_ayVtxs[0], sizeof(SFWVertex)*dwNumVtx);

	// Build indexes by two main triangle
	vector<WORD> ayIdxTglLT = buildIdxByTgl(pvt_pTglLT);
	vector<WORD> ayIdxTglRB = buildIdxByTgl(pvt_pTglRB);
	DWORD dwNumIdx = ayIdxTglLT.size() + ayIdxTglRB.size();
	WORD* pIdxs = new WORD[dwNumIdx];
	memcpy( pIdxs,						&ayIdxTglLT[0], sizeof(WORD)*ayIdxTglLT.size());
	memcpy( pIdxs + ayIdxTglLT.size(),	&ayIdxTglRB[0], sizeof(WORD)*ayIdxTglRB.size());

	// Add to parameter
	CFWParam pamRet;
	pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtxs));
	pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));
	pamRet.add( "aryIdxs", numToStr<DWORD>((DWORD)pIdxs));
	pamRet.add( "numIdxs", numToStr<DWORD>(dwNumIdx));

	return pamRet;
}

std::vector<float> CFWTerrain::loadHgtMap(
	string strFileI, UINT uWidI, UINT uHgtI, float fSclI)
{
	std::vector<BYTE> ayDat(uWidI * uHgtI);
	std::ifstream ifStm( strFileI, std::ios_base::binary);
	if(!ifStm.is_open())	{ throw std::runtime_error("Error parameter: No such file!"); }
	ifStm.read( (char*)&ayDat[0], ayDat.size());
	ifStm.close();

	std::vector<float> ayHgt;
	for( DWORD i = 0; i < ayDat.size(); ++i)	{ ayHgt.push_back(ayDat[i] * fSclI); }

	return ayHgt;
}