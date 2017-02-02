#pragma once
#include "stdafx.h"
#include "SpaceGenerator.h"
#pragma comment( lib, "../Debug/OPTools.lib")
#include "../OPTools/String.h"
using OPTools::numToStr;
#pragma comment( lib, "../Debug/OPMath.lib")
#include "../FWMain/FWManager.h"
#include "../FWMain/FWFactory.h"
using std::copy;
using std::back_inserter;

void destroy(SSpaceBox* pBoxI)
{
	if(pBoxI == NULL)	{ return; }
	destroy(pBoxI->m_pRight);
	if(pBoxI->m_pFront)	{ pBoxI->m_pFront->m_pBack = NULL; }
	if(pBoxI->m_pBack)	{ pBoxI->m_pBack->m_pFront = NULL; }
	if(pBoxI->m_pLeft)	{ pBoxI->m_pLeft->m_pRight = NULL; }
	if(pBoxI->m_pRight)	{ pBoxI->m_pRight->m_pLeft = NULL; }
	if(pBoxI->m_pTop)	{ pBoxI->m_pTop->m_pBottom = NULL; }
	if(pBoxI->m_pBottom)	{ pBoxI->m_pBottom->m_pTop = NULL; }
	SAFE_DELETE(pBoxI);
}

void CSpaceGenerator::Destroy()
{
	if(ptd_pBoxLTF == NULL)	{ return; }
	
	for( SSpaceBox* pLTF = ptd_pBoxLTF; pLTF;)
	{
		SSpaceBox* pNxtLTF = pLTF->m_pBottom;
		for( SSpaceBox* pLF = pLTF; pLF;)
		{
			SSpaceBox* pNxtLF = pLF->m_pBack;
			destroy(pLF);
			pLF = pNxtLF;
		}
		pLTF = pNxtLTF;
	}
}

SSpaceBox* CSpaceGenerator::BuildSpace( float fBoxSzI, UINT uEdgeSzI, UINT uHgtI)
{
	float fHlfBoxSz = fBoxSzI * .5;
	ptd_fBoxSz = fBoxSzI;
	ptd_fEdgeLen = fBoxSzI*uEdgeSzI;
	ptd_pBoxLTF = new SSpaceBox();
	float fHlfSz = ptd_fEdgeLen * .5;
	ptd_pBoxLTF->m_poiLTF = SFWQuate( fHlfSz, uHgtI*fBoxSzI, fHlfSz);
	float fRBBAx = fHlfSz - fBoxSzI;
	ptd_pBoxLTF->m_poiRBB = SFWQuate( fRBBAx, uHgtI*fBoxSzI - fBoxSzI, fRBBAx);
	ptd_pBoxLTF->m_poiCen = SFWQuate(
		(ptd_pBoxLTF->m_poiLTF.m_fX + ptd_pBoxLTF->m_poiRBB.m_fX)*.5,
		(ptd_pBoxLTF->m_poiLTF.m_fY + ptd_pBoxLTF->m_poiRBB.m_fY)*.5,
		(ptd_pBoxLTF->m_poiLTF.m_fZ + ptd_pBoxLTF->m_poiRBB.m_fZ)*.5);
	// The source box has been created,
	// so the rest box number should subtract 1
	UINT uNumCrte = uEdgeSzI - 1;

	// Generate one row boxes
	geneRow( ptd_pBoxLTF, uNumCrte - 1);

	// Generate col by first row
	SSpaceBox* pBoxTmp = ptd_pBoxLTF;
	for( UINT i = 0; i < uNumCrte && pBoxTmp; ++i)
	{
		SSpaceBox* pBoxFst = pBoxTmp;
		geneCol(pBoxTmp);
		pBoxTmp = pBoxFst->m_pBack;
	}

	// Generate height by the top block of boxes
	pBoxTmp = ptd_pBoxLTF;
	for( UINT i = 0; i < uHgtI && pBoxTmp; ++i)
	{
		SSpaceBox* pBoxFst = pBoxTmp;
		geneHgt(pBoxTmp);
		if(pBoxFst->m_pBottom)	{ pBoxTmp = pBoxFst->m_pBottom; }
		else					{ pBoxTmp = pBoxFst; }
	}

	// Set the bottom block as base
	for( UINT i = 0; i < 1; ++i)
	{
		SSpaceBox* pBoxFmr = NULL;
		if(pBoxTmp->m_pTop != NULL)
		{
			pBoxFmr = pBoxTmp->m_pTop;
		}
		setBoxTypFmLT( SSpaceBox::BTP_GRASS, pBoxTmp);
		pBoxTmp = pBoxFmr;
	}
	
	return ptd_pBoxLTF;
}

CSpaceGenerator::AryIniScn CSpaceGenerator::GeneScnIniInfo()
{
	AryIniScn ayRet = rvwSpaceToCrtePam();

	CFWParam pamEarthCore( "mdlName", "EarthCore");
	string strEdge = numToStr<float>(ptd_fEdgeLen);
	pamEarthCore.add( "plnLength", strEdge);
	pamEarthCore.add( "plnWidth", strEdge);
	pamEarthCore.add( "lock", "true");
	pamEarthCore.add( "mtlDif", "R255G127B0A255");
	ayRet.push_back(pamEarthCore);

	CFWParam pamCam( "camName", "ProjectCamera");
	pamCam.add( "locEye", "X150Y150Z150H1");
	pamCam.add( "locTgt", "X0Y0Z0H1");
	pamCam.add( "viewWid", "1680");
	pamCam.add( "viewHgt", "1050");
	pamCam.add( "clsProjPln", "1");
	pamCam.add( "farProjPln", "1000");
	pamCam.add( "setOperation", "true");
	ayRet.push_back(pamCam);

	CFWParam pamEff;
	pamEff.add( "effID", "AMBIENT_LGT");
	pamEff.add( "enable", "true");
	ayRet.push_back(pamEff);
	pamEff.cls();
	pamEff.add( "effID", "SHOW_EDGE");
	pamEff.add( "enable", "false");
	ayRet.push_back(pamEff);
	pamEff.cls();
	pamEff.add( "effID", "Z_DEPTH");
	pamEff.add( "enable", "true");
	ayRet.push_back(pamEff);

	CFWParam pamLgt( "lgtName", "MainLight");
	pamLgt.add( "lgtTyp", "SPOT");
	pamLgt.add( "lgtDif", "R255G255B255A255");
	pamLgt.add( "lgtAmb", "R255G255B255A255");
	pamLgt.add( "lgtSpc", "R255G255B255A255");
	pamLgt.add( "lgtPos", "X0Y100Z0H1");
	pamLgt.add( "lgtDir", "X0Y-1Z0H1");
	pamLgt.add( "lgtID", "0");
	pamLgt.add( "enable", "true");
	ayRet.push_back(pamLgt);

	CFWParam pamScn( "scnName", "MainScene");
	pamScn.add( "setCurrent", "true");
	ayRet.push_back(pamScn);

	return ayRet;
}

void CSpaceGenerator::geneRow( SSpaceBox* pBoxI, UINT uEdgeSzI)
{
	if(pBoxI == NULL)	{ return; }
	pBoxI->m_pRight = new SSpaceBox();
	SSpaceBox* pRgt = pBoxI->m_pRight;
	pRgt->m_pLeft = pBoxI;
	pRgt->m_poiLTF = pBoxI->m_poiLTF;
	pRgt->m_poiLTF.m_fX -= ptd_fBoxSz;
	pRgt->m_poiRBB = pBoxI->m_poiRBB;
	pRgt->m_poiRBB.m_fX -= ptd_fBoxSz;
	pRgt->m_poiCen = pBoxI->m_poiCen;
	pRgt->m_poiCen.m_fX -= ptd_fBoxSz;
	if(uEdgeSzI == 0)	{ return; }
	geneRow( pRgt, --uEdgeSzI);
}

void CSpaceGenerator::geneCol(SSpaceBox* pBoxI)
{
	if(pBoxI == NULL)	{ return; }
	pBoxI->m_pBack = new SSpaceBox();
	SSpaceBox* pBck = pBoxI->m_pBack;
	pBck->m_poiLTF = pBoxI->m_poiLTF;
	pBck->m_poiLTF.m_fZ -= ptd_fBoxSz;
	pBck->m_poiRBB = pBoxI->m_poiRBB;
	pBck->m_poiRBB.m_fZ -= ptd_fBoxSz;
	pBck->m_poiCen = pBoxI->m_poiCen;
	pBck->m_poiCen.m_fZ -= ptd_fBoxSz;
	pBck->m_pFront = pBoxI;
	if(pBoxI->m_pLeft)
	{
		SSpaceBox* pLeft = pBoxI->m_pLeft;
		pBck->m_pLeft = pLeft->m_pBack;
		if(pLeft->m_pBack)
		{
			SSpaceBox* pLftBck = pLeft->m_pBack;
			pLftBck->m_pRight = pBck;
		}
	}
	if(pBoxI->m_pRight == NULL)	{ return; }
	geneCol(pBoxI->m_pRight);
}

void CSpaceGenerator::geneHgt(SSpaceBox* pBoxI)
{
	for( SSpaceBox* pFB = pBoxI; pFB; pFB = pFB->m_pBack)
	{
		for( SSpaceBox* pLR = pFB; pLR; pLR = pLR->m_pRight)
		{
			pLR->m_pBottom = new SSpaceBox();
			SSpaceBox* pBtm = pLR->m_pBottom;
			pBtm->m_poiLTF = pLR->m_poiLTF;
			pBtm->m_poiLTF.m_fY -= ptd_fBoxSz;
			pBtm->m_poiRBB = pLR->m_poiRBB;
			pBtm->m_poiRBB.m_fY -= ptd_fBoxSz;
			pBtm->m_poiCen = pLR->m_poiCen;
			pBtm->m_poiCen.m_fY -= ptd_fBoxSz;
			pBtm->m_pTop = pLR;
			if(pLR->m_pFront && pLR->m_pFront->m_pBottom)
			{
				SSpaceBox* pFrtBtm = pLR->m_pFront->m_pBottom;
				pBtm->m_pFront = pFrtBtm;
				pFrtBtm->m_pBack = pBtm;
			}
			if(pLR->m_pBack && pLR->m_pBack->m_pBottom)
			{
				SSpaceBox* pBckBtm = pLR->m_pBack->m_pBottom;
				pBtm->m_pBack = pBckBtm;
				pBckBtm->m_pFront = pBtm;
			}
			if(pLR->m_pLeft && pLR->m_pLeft->m_pBottom)
			{
				SSpaceBox* pLftBtm = pLR->m_pLeft->m_pBottom;
				pBtm->m_pLeft = pLftBtm;
				pLftBtm->m_pRight = pBtm;
			}
			if(pLR->m_pRight && pLR->m_pRight->m_pBottom)
			{
				SSpaceBox* pRgtBtm = pLR->m_pRight->m_pBottom;
				pBtm->m_pRight = pRgtBtm;
				pRgtBtm->m_pLeft = pBtm;
			}
		}
	}
}

VTP_INI_MAP( SSpaceBox::EBoxTyp, UINT)
BEG_INI_MAP( SSpaceBox::EBoxTyp, UINT, 2, s_ayBoxNum)
	VtpKeyVal( SSpaceBox::BTP_BED_ROCK, 0),
	VtpKeyVal( SSpaceBox::BTP_GRASS, 0)
END_INI_MAP( SSpaceBox::EBoxTyp, UINT, 2, s_ayBoxNum, s_mpBoxNum);

#define CRTE_RECT( whichRect, whichRectID, difMtl, difTex) \
	if(!pBoxI->whichRect || pBoxI->whichRect->m_tpBox == SSpaceBox::BTP_UNKNOWN)\
	{\
		CFWParam pamIniMsh = geneRectCrtePam( pBoxI, whichRectID);\
		if(!pamBox.has("aryPois"))	{ pamBox.mrg(pamIniMsh); }\
		else	{ CFWMshFcty::MrgPam( pamBox, pamIniMsh, uSubID); }\
		string strSubID = numToStr<UINT>(uSubID);\
		pamBox.add( "mtlDif" + strSubID, difMtl);\
		pamBox.add( "difTex" + strSubID, difTex);\
		++uSubID;\
	}

CFWParam CSpaceGenerator::geneCrtePam(SSpaceBox* pBoxI)
{
	CFWParam pamBox;
	if(pBoxI == NULL)	{ return pamBox; }
	
	UINT uNumTmp = (s_mpBoxNum[pBoxI->m_tpBox]++);
	UINT uSubID = 0;
	switch(pBoxI->m_tpBox)
	{
	case SSpaceBox::BTP_BED_ROCK:
		pamBox.add( "mdlName", "RedRock" + numToStr<UINT>(uNumTmp));
		CRTE_RECT( m_pFront, SSpaceBox::BRT_FRONT, "R255G255B255A255", "bedrock.png");
		CRTE_RECT( m_pBack, SSpaceBox::BRT_BACK, "R255G255B255A255", "bedrock.png");
		CRTE_RECT( m_pLeft, SSpaceBox::BRT_LEFT, "R255G255B255A255", "bedrock.png");
		CRTE_RECT( m_pRight, SSpaceBox::BRT_RIGHT, "R255G255B255A255", "bedrock.png");
		CRTE_RECT( m_pTop, SSpaceBox::BRT_TOP, "R255G255B255A255", "bedrock.png");
		CRTE_RECT( m_pBottom, SSpaceBox::BRT_BOTTOM, "R255G255B255A255", "bedrock.png");
		return pamBox;
	case SSpaceBox::BTP_GRASS:
		pamBox.add( "mdlName", "Grass" + numToStr<UINT>(uNumTmp));
		CRTE_RECT( m_pFront, SSpaceBox::BRT_FRONT, "R255G255B255A255", "grass_side.png");
		CRTE_RECT( m_pBack, SSpaceBox::BRT_BACK, "R255G255B255A255", "grass_side.png");
		CRTE_RECT( m_pLeft, SSpaceBox::BRT_LEFT, "R255G255B255A255", "grass_side.png");
		CRTE_RECT( m_pRight, SSpaceBox::BRT_RIGHT, "R255G255B255A255", "grass_side.png");
		CRTE_RECT( m_pTop, SSpaceBox::BRT_TOP, "R160G255B160A255", "grass_top.png");
		CRTE_RECT( m_pBottom, SSpaceBox::BRT_BOTTOM, "R255G255B255A255", "dirt.png");
		return pamBox;
	default: break;
	}
	return pamBox;
}

CFWParam CSpaceGenerator::geneRectCrtePam( SSpaceBox* pBoxI, SSpaceBox::EBoxRect uRectI)
{
	CBoxAABB box( pBoxI->m_poiLTF, pBoxI->m_poiRBB);
	switch(uRectI)
	{
	case SSpaceBox::BRT_FRONT:// Front
		return CFWMshFcty::GenRectInitPam(
			box.getPoiLTF(), box.getPoiRTF(),
			box.getPoiLBF(), box.getPoiRBF());
	case SSpaceBox::BRT_BACK:// Back
		return CFWMshFcty::GenRectInitPam(
			box.getPoiRTB(), box.getPoiLTB(),
			box.getPoiRBB(), box.getPoiLBB());
	case SSpaceBox::BRT_LEFT:// Left
		return CFWMshFcty::GenRectInitPam(
			box.getPoiLTB(), box.getPoiLTF(),
			box.getPoiLBB(), box.getPoiLBF());
	case SSpaceBox::BRT_RIGHT:// Right
		return CFWMshFcty::GenRectInitPam(
			box.getPoiRTF(), box.getPoiRTB(),
			box.getPoiRBF(), box.getPoiRBB());
	case SSpaceBox::BRT_TOP:// Top
		return CFWMshFcty::GenRectInitPam(
			box.getPoiLTB(), box.getPoiRTB(),
			box.getPoiLTF(), box.getPoiRTF());
	case SSpaceBox::BRT_BOTTOM:// Bottom
		return CFWMshFcty::GenRectInitPam(
			box.getPoiLBF(), box.getPoiRBF(),
			box.getPoiLBB(), box.getPoiRBB());
	default:
		break;
	}
	return CFWParam();
}

void CSpaceGenerator::setBoxTypFmLT( SSpaceBox::EBoxTyp typI, SSpaceBox* pBoxI)
{
	if(pBoxI == NULL)	{ return; }
	pBoxI->m_tpBox = typI;
	setBoxTypFmLT( typI, pBoxI->m_pRight);
	setBoxTypFmLT( typI, pBoxI->m_pBack);
}

void CSpaceGenerator::setBoxTypFmRB( SSpaceBox::EBoxTyp typI, SSpaceBox* pBoxI)
{
	if(pBoxI == NULL)	{ return; }
	pBoxI->m_tpBox = typI;
	setBoxTypFmRB( typI, pBoxI->m_pLeft);
	setBoxTypFmRB( typI, pBoxI->m_pFront);
}

#define RVW_TMP( funcNam, nxtBox)	\
	CSpaceGenerator::AryIniScn CSpaceGenerator::funcNam( SSpaceBox* pBoxI, ProcBox procI)	{\
		AryIniScn ayRet;	if(pBoxI == NULL)	{ return ayRet; }\
		CFWParam pamGen = procI(pBoxI);	if(!pamGen.isEmpty())	{ ayRet.push_back(pamGen); }\
		AryIniScn ayTmp = funcNam( pBoxI->nxtBox, procI);\
		copy( ayTmp.begin(), ayTmp.end(), back_inserter(ayRet));	return ayRet; }

RVW_TMP( rvwRowLR, m_pRight);
RVW_TMP( rvwRowRL, m_pLeft);
RVW_TMP( rvwColFB, m_pBack);
RVW_TMP( rvwColBF, m_pFront);

CSpaceGenerator::AryIniScn CSpaceGenerator::rvwPlnFrmLF( SSpaceBox* pBoxI, ProcBox procI)
{
	AryIniScn ayRet;
	for( SSpaceBox* pTmp = pBoxI; pTmp; pTmp = pTmp->m_pBack)
	{
		AryIniScn ayTmp = rvwRowLR( pTmp, procI);
		copy( ayTmp.begin(), ayTmp.end(), back_inserter(ayRet));
	}
	return ayRet;
}

CSpaceGenerator::AryIniScn CSpaceGenerator::rvwPlnFrmRB( SSpaceBox* pBoxI, ProcBox procI)
{
	AryIniScn ayRet;
	for( SSpaceBox* pTmp = pBoxI; pTmp; pTmp = pTmp->m_pFront)
	{
		AryIniScn ayTmp = rvwRowRL( pTmp, procI);
		copy( ayTmp.begin(), ayTmp.end(), back_inserter(ayRet));
	}
	return ayRet;
}

CSpaceGenerator::AryIniScn CSpaceGenerator::rvwSpaceToCrtePam()
{
	AryIniScn ayRet;
	if(ptd_pBoxLTF == NULL)	{ return ayRet; }
	
	for( SSpaceBox* pLF = ptd_pBoxLTF; pLF; pLF = pLF->m_pBottom)
	{
		AryIniScn ayTmp = rvwPlnFrmLF( pLF, geneCrtePam);
		copy( ayTmp.begin(), ayTmp.end(), back_inserter(ayRet));
	}
	return ayRet;
}