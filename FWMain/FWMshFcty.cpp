#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWMesh.h"
#include "FWPool.h"
#include "FWMisc.h"
#include "FWMshIntf.h"
#include "FWSkin.h"
#include "FWD3DMisc.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
#include "d3dx9.h"

SINGLE_IMPLEMENT(CFWMshFcty)

CFWIntf& CFWMshFcty::Create(CFWParam& pamI)
{
	// Create a new mesh according user's required
	CFWMesh* pMshCrte = NULL;
	if(pamI.has("primTyp"))
	{
		pMshCrte = new CFWPrim();
		UINT uPmTp = pamI.getUint("primTyp");
		if(uPmTp > CFWMshIntf::PRIN_TP_TRIANGLE)
		{
			uPmTp = CFWMshIntf::PRIN_TP_TRIANGLE;
		}
		((CFWPrim*)pMshCrte)->ptd_uPrimTyp = uPmTp;

		// If index buffer is existed, follow it to fill the vertex buffer
		if(pamI.has("aryIdxs") && pamI.has("numIdxs"))
		{
			DWORD dwNumVtx = pamI.getDword("numPois");
			SFWVertex* pOrgVtx = (SFWVertex*)pamI.getDword("aryPois");

			DWORD dwNumIdx = pamI.getDword("numIdxs");
			WORD* pIdx = (WORD*)pamI.getDword("aryIdxs");

			SFWVertex* pNewVtx = new SFWVertex[dwNumIdx];
			memset( pNewVtx, NULL, sizeof(SFWVertex) * dwNumIdx);
			for( DWORD i = 0; i < dwNumIdx; ++i)
			{
				WORD wIdx = pIdx[i];
				if(wIdx >= dwNumVtx)	{ continue; }
				pNewVtx[i] = pOrgVtx[wIdx];
			}

			delete [dwNumVtx](pOrgVtx);
			delete [dwNumIdx](pIdx);
			pamI.rmv("numIdxs");
			pamI.rmv("aryIdxs");
			pamI.upd( "numPois", numToStr<DWORD>(dwNumIdx));
			pamI.upd( "aryPois", numToStr<DWORD>((DWORD)pNewVtx));
		}
	}
	else
	{
		pMshCrte = new CFWMesh();
	}

	// Input parameter into new mesh
	pMshCrte->ptd_dwNumVtx = pamI.getDword("numPois");
	if(pamI.has("numIdxs"))
	{
		pMshCrte->ptd_dwNumFac = pamI.getDword("numIdxs") / 3;
	}
	SFWColor colFromCol;
	if(pamI.has("vtxCol"))
	{
		colFromCol = pamI.getColor("vtxCol");
		pMshCrte->ptd_colVtx = tunToD3DCol(colFromCol);
	}
	else
	{
		colFromCol = SFWColor::geneRandCol();
		pMshCrte->ptd_colVtx = tunToD3DCol(colFromCol);
	}

	// Calculate out every vertex's size and FVF
	pMshCrte->ptd_dwPerVtx = sizeof(SFWVertex);
	pMshCrte->ptd_dwFVF = D3DFVF_XYZ;
	// Except "genNmls" be set as false, other conditions vertexes will have normals
	if(!(pamI.has("genNmls") && !pamI.getBool("genNmls")) && !pamI.has("primTyp"))
	{
		pMshCrte->ptd_dwPerVtx += sizeof(SFWQuate);
		pMshCrte->ptd_dwFVF |= D3DFVF_NORMAL;
	}
	if(colFromCol.beSetCorrect())
	{
		pMshCrte->ptd_dwPerVtx += sizeof(DWORD);
		pMshCrte->ptd_dwFVF |= D3DFVF_DIFFUSE;
	}
	for( int iTexId = 1; iTexId <= MAX_TEX && !pamI.has("primTyp"); ++iTexId)
	{
		if(pamI.has("texUV" + numToStr<int>(iTexId)))
		{
			pMshCrte->ptd_dwPerVtx += 2*sizeof(float);
			pMshCrte->ptd_dwFVF |= D3DFVF_TEX1*iTexId;
		}
	}

	// Add new device into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pMshCrte);

	// Set new mesh to a mesh interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWMshIntf(pMshCrte);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));

	return *pIntfTmp;
}

SFWVertex poiToVtx(CPoint poiI)
{
	return SFWVertex( poiI.m_fX, poiI.m_fY, poiI.m_fZ);
}

CFWParam CFWMshFcty::GenPlnInitPam( float fLenI, float fWidI, UINT uLenSecI, UINT uWidSecI, bool bGrteWFI)
{
	if(uLenSecI == 0 || uWidSecI == 0)
	{
		throw std::runtime_error("error parameter: model plane's length/width section cant be set zero!!");
	}
	float fHalfLen = fLenI*0.5;
	float fHalfWid = fWidI*0.5;
	float fLen1Sec = 1/(float)uLenSecI;
	float fWid1Sec = 1/(float)uWidSecI;
	float fLenSec = fLenI*fLen1Sec;
	float fWidSec = fWidI*fWid1Sec;
	//+_+:want special location, build model and add space control
	float fXStart = fHalfLen;
	float fZStart = -fHalfWid;

	vector<SFWVertex> ayVtx;
	vector<float> ayUV;
	float fU = 0, fV = 0;
	for( UINT j = 0, fV = 0; j <= uWidSecI; ++j, fV += fWid1Sec)
	{
		for( UINT i = 0, fU = 0; i <= uLenSecI; ++i, fU += fLen1Sec)
		{
			ayVtx.push_back(SFWVertex( fXStart - i*fLenSec, 0, fZStart + j*fWidSec));
			ayUV.push_back(fU);
			ayUV.push_back(fV);
		}
	}

	vector<WORD> ayIdx;
	for( UINT j = 0; j < uWidSecI; ++j)
	{
		for( UINT i = 0; i < uLenSecI; ++i)
		{
			WORD wLT = i + j*(uLenSecI + 1);
			WORD wRT = wLT + 1;
			WORD wLB = wLT + uLenSecI + 1;
			WORD wRB = wLB + 1;

			ayIdx.push_back(wLT);
			ayIdx.push_back(wRT);
			ayIdx.push_back(wLB);

			ayIdx.push_back(wRT);
			ayIdx.push_back(wRB);
			ayIdx.push_back(wLB);
		}
	}

	CFWParam pamRet;

	// Insert vertexes into memory
	DWORD dwNumVtx = ayVtx.size();
	SFWVertex* pVtx = new SFWVertex[dwNumVtx];
	memcpy( pVtx, &ayVtx[0], dwNumVtx*sizeof(SFWVertex));

	// Insert indexes into memory
	DWORD dwNumIdx = ayIdx.size();
	WORD* pIdx = new WORD[dwNumIdx];
	memcpy( pIdx, &ayIdx[0], dwNumIdx*sizeof(WORD));

	// Insert UV into memory
	DWORD dwAllUV = 2*dwNumVtx;
	float* pUV = new float[dwAllUV];
	memcpy( pUV, &ayUV[0], dwAllUV*sizeof(float));

	// Add to parameter
	pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtx));
	pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));
	pamRet.add( "aryIdxs", numToStr<DWORD>((DWORD)pIdx));
	pamRet.add( "numIdxs", numToStr<DWORD>(dwNumIdx));
	pamRet.add( "texUV1", numToStr<DWORD>((DWORD)pUV));

	return pamRet;
}

CFWParam CFWMshFcty::GenRectInitPam(
		CPoint poiLTI, CPoint poiRTI,
		CPoint poiLBI, CPoint poiRBI, bool bGrteWFI)
{
	if(bGrteWFI)
	{
		vector<SFWVertex> ayVtx;
		ayVtx.push_back(poiLTI);	ayVtx.push_back(poiRTI);
		ayVtx.push_back(poiRTI);	ayVtx.push_back(poiRBI);
		ayVtx.push_back(poiRBI);	ayVtx.push_back(poiLBI);
		ayVtx.push_back(poiLBI);	ayVtx.push_back(poiLTI);

		CFWParam pamRet;
		pamRet.add( "primTyp", numToStr<UINT>(CFWMshIntf::PRIM_TP_LN_LST));

		// Insert vertexes into memory
		DWORD dwNumVtx = ayVtx.size();
		SFWVertex* pVtx = new SFWVertex[dwNumVtx];
		memcpy( pVtx, &ayVtx[0], dwNumVtx*sizeof(SFWVertex));

		// Add to parameter
		pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtx));
		pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));

		return pamRet;
	}
	else
	{
		vector<SFWVertex> ayVtx;
		ayVtx.push_back(poiLTI);//0
		ayVtx.push_back(poiRTI);//1
		ayVtx.push_back(poiLBI);//2
		ayVtx.push_back(poiRBI);//3

		vector<WORD> ayIdx;
		ayIdx.push_back(0);		ayIdx.push_back(1);		ayIdx.push_back(2);
		ayIdx.push_back(1);		ayIdx.push_back(3);		ayIdx.push_back(2);

		vector<float> ayUV;
		ayUV.push_back(0);	ayUV.push_back(0);
		ayUV.push_back(1);	ayUV.push_back(0);
		ayUV.push_back(0);	ayUV.push_back(1);
		ayUV.push_back(1);	ayUV.push_back(1);

		CFWParam pamRet;
		// Insert vertexes into memory
		DWORD dwNumVtx = ayVtx.size();
		SFWVertex* pVtx = new SFWVertex[dwNumVtx];
		memcpy( pVtx, &ayVtx[0], dwNumVtx*sizeof(SFWVertex));

		// Insert indexes into memory
		DWORD dwNumIdx = ayIdx.size();
		WORD* pIdx = new WORD[dwNumIdx];
		memcpy( pIdx, &ayIdx[0], dwNumIdx*sizeof(WORD));

		// Insert UV into memory
		DWORD dwAllUV = 2*dwNumVtx;
		float* pUV = new float[dwAllUV];
		memcpy( pUV, &ayUV[0], dwAllUV*sizeof(float));

		// Add to parameter
		pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtx));
		pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));
		pamRet.add( "aryIdxs", numToStr<DWORD>((DWORD)pIdx));
		pamRet.add( "numIdxs", numToStr<DWORD>(dwNumIdx));
		pamRet.add( "texUV1", numToStr<DWORD>((DWORD)pUV));

		return pamRet;
	}
}

CFWParam CFWMshFcty::GenBoxInitPam(CBoxAABB boxI, bool bGrteWFI)
{
	SFWVertex vtxLTF = poiToVtx(boxI.getPoiLTF());
	SFWVertex vtxRTF = poiToVtx(boxI.getPoiRTF());
	SFWVertex vtxLBF = poiToVtx(boxI.getPoiLBF());
	SFWVertex vtxRBF = poiToVtx(boxI.getPoiRBF());

	SFWVertex vtxRTB = poiToVtx(boxI.getPoiRTB());
	SFWVertex vtxLTB = poiToVtx(boxI.getPoiLTB());
	SFWVertex vtxRBB = poiToVtx(boxI.getPoiRBB());
	SFWVertex vtxLBB = poiToVtx(boxI.getPoiLBB());

	if(bGrteWFI)
	{
		vector<SFWVertex> ayVtx;
		// Front
		ayVtx.push_back(vtxLTF);	ayVtx.push_back(vtxRTF);
		ayVtx.push_back(vtxRTF);	ayVtx.push_back(vtxRBF);
		ayVtx.push_back(vtxRBF);	ayVtx.push_back(vtxLBF);
		ayVtx.push_back(vtxLBF);	ayVtx.push_back(vtxLTF);
		// Back
		ayVtx.push_back(vtxRTB);	ayVtx.push_back(vtxLTB);
		ayVtx.push_back(vtxLTB);	ayVtx.push_back(vtxLBB);
		ayVtx.push_back(vtxLBB);	ayVtx.push_back(vtxRBB);
		ayVtx.push_back(vtxRBB);	ayVtx.push_back(vtxRTB);

		ayVtx.push_back(vtxLTF);	ayVtx.push_back(vtxLTB);
		ayVtx.push_back(vtxRTF);	ayVtx.push_back(vtxRTB);
		ayVtx.push_back(vtxLBF);	ayVtx.push_back(vtxLBB);
		ayVtx.push_back(vtxRBF);	ayVtx.push_back(vtxRBB);

		CFWParam pamRet;
		pamRet.add( "primTyp", numToStr<UINT>(CFWMshIntf::PRIM_TP_LN_LST));

		// Insert vertexes into memory
		DWORD dwNumVtx = ayVtx.size();
		SFWVertex* pVtx = new SFWVertex[dwNumVtx];
		memcpy( pVtx, &ayVtx[0], dwNumVtx*sizeof(SFWVertex));
		pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtx));
		pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));

		return pamRet;
	}
	else
	{
		vector<SFWVertex> ayVtx;
		// Front
		ayVtx.push_back(vtxLTF);//0
		ayVtx.push_back(vtxRTF);//1
		ayVtx.push_back(vtxLBF);//2
		ayVtx.push_back(vtxRBF);//3

		// Back
		ayVtx.push_back(vtxRTB);//4
		ayVtx.push_back(vtxLTB);//5
		ayVtx.push_back(vtxRBB);//6
		ayVtx.push_back(vtxLBB);//7

		// Left
		ayVtx.push_back(vtxLTB);//8
		ayVtx.push_back(vtxLTF);//9
		ayVtx.push_back(vtxLBB);//10
		ayVtx.push_back(vtxLBF);//11

		// Right
		ayVtx.push_back(vtxRTF);//12
		ayVtx.push_back(vtxRTB);//13
		ayVtx.push_back(vtxRBF);//14
		ayVtx.push_back(vtxRBB);//15

		// Top
		ayVtx.push_back(vtxLTB);//16
		ayVtx.push_back(vtxRTB);//17
		ayVtx.push_back(vtxLTF);//18
		ayVtx.push_back(vtxRTF);//19

		// Bottom
		ayVtx.push_back(vtxLBF);//20
		ayVtx.push_back(vtxRBF);//21
		ayVtx.push_back(vtxLBB);//22
		ayVtx.push_back(vtxRBB);//23

		vector<WORD> ayIdx;
		// Front
		ayIdx.push_back(0);		ayIdx.push_back(1);		ayIdx.push_back(2);
		ayIdx.push_back(1);		ayIdx.push_back(3);		ayIdx.push_back(2);

		// Back
		ayIdx.push_back(4);		ayIdx.push_back(5);		ayIdx.push_back(6);
		ayIdx.push_back(5);		ayIdx.push_back(7);		ayIdx.push_back(6);

		// Left
		ayIdx.push_back(8);		ayIdx.push_back(9);		ayIdx.push_back(10);
		ayIdx.push_back(9);		ayIdx.push_back(11);	ayIdx.push_back(10);

		// Right
		ayIdx.push_back(12);	ayIdx.push_back(13);	ayIdx.push_back(14);
		ayIdx.push_back(13);	ayIdx.push_back(15);	ayIdx.push_back(14);

		// Top
		ayIdx.push_back(16);	ayIdx.push_back(17);	ayIdx.push_back(18);
		ayIdx.push_back(17);	ayIdx.push_back(19);	ayIdx.push_back(18);

		// Bottom
		ayIdx.push_back(20);	ayIdx.push_back(21);	ayIdx.push_back(22);
		ayIdx.push_back(21);	ayIdx.push_back(23);	ayIdx.push_back(22);

		vector<float> ayUV;
		for( UINT i = 0; i < 3; ++i)
		{
			ayUV.push_back(0);	ayUV.push_back(0);
			ayUV.push_back(0);	ayUV.push_back(1);
			ayUV.push_back(1);	ayUV.push_back(0);
			ayUV.push_back(1);	ayUV.push_back(1);

			ayUV.push_back(0);	ayUV.push_back(0);
			ayUV.push_back(1);	ayUV.push_back(0);
			ayUV.push_back(0);	ayUV.push_back(1);
			ayUV.push_back(1);	ayUV.push_back(1);
		}

		CFWParam pamRet;

		// Insert vertexes into memory
		DWORD dwNumVtx = ayVtx.size();
		SFWVertex* pVtx = new SFWVertex[dwNumVtx];
		memcpy( pVtx, &ayVtx[0], dwNumVtx*sizeof(SFWVertex));

		// Insert indexes into memory
		DWORD dwNumIdx = ayIdx.size();
		WORD* pIdx = new WORD[dwNumIdx];
		memcpy( pIdx, &ayIdx[0], dwNumIdx*sizeof(WORD));

		// Insert UV into memory
		DWORD dwAllUV = 2*dwNumVtx;
		float* pUV = new float[dwAllUV];
		memcpy( pUV, &ayUV[0], dwAllUV*sizeof(float));

		// Add to parameter
		pamRet.add( "aryPois", numToStr<DWORD>((DWORD)pVtx));
		pamRet.add( "numPois", numToStr<DWORD>(dwNumVtx));
		pamRet.add( "aryIdxs", numToStr<DWORD>((DWORD)pIdx));
		pamRet.add( "numIdxs", numToStr<DWORD>(dwNumIdx));
		pamRet.add( "texUV1", numToStr<DWORD>((DWORD)pUV));

		return pamRet;
	}
}

CFWParam CFWMshFcty::GenLineInitPam( CPoint poiBegI, CPoint poiEndI)
{
	SFWVertex* ayVtx = new SFWVertex[2];
	ayVtx[0] = poiToVtx(poiBegI);
	ayVtx[1] = poiToVtx(poiEndI);

	CFWParam pamRet;
	pamRet.add( "aryPois", numToStr<DWORD>((DWORD)ayVtx));
	pamRet.add( "numPois", "2");
	pamRet.add( "primTyp", "1"/* PRIM_TP_LINE */);

	return pamRet;
}

CFWParam CFWMshFcty::GenTriangleInitPam( CPoint poiAI, CPoint poiBI, CPoint poiCI, bool bGrteWFI)
{
	SFWVertex* ayVtx = new SFWVertex[3];
	ayVtx[0] = poiToVtx(poiAI);
	ayVtx[1] = poiToVtx(poiBI);
	ayVtx[2] = poiToVtx(poiCI);

	WORD* ayIdx = new WORD[3];
	ayIdx[0] = 0;
	ayIdx[1] = 1;
	ayIdx[2] = 2;

	CFWParam pamRet;
	pamRet.add( "aryPois", numToStr<DWORD>((DWORD)ayVtx));
	pamRet.add( "numPois", "3");
	pamRet.add( "aryIdxs", numToStr<DWORD>((DWORD)ayIdx));
	pamRet.add( "numIdxs", "3");

	return pamRet;
}

void CFWMshFcty::ClsCrtePam(CFWParam pamI)
{
	// Delete vertex array
	if(pamI.has("aryPois") && pamI.has("numPois"))
	{
		DWORD dwNumVtx = pamI.getDword("numPois");
		SFWVertex* pVtx = (SFWVertex*)pamI.getDword("aryPois");
		if(pVtx != NULL)	{ delete[dwNumVtx](pVtx); }
	}

	// Delete index array
	DWORD dwNumIdx = 0;
	if(pamI.has("aryIdxs") && pamI.has("numIdxs"))
	{
		dwNumIdx = pamI.getDword("numIdxs");
		WORD* pIdx = (WORD*)pamI.getDword("aryIdxs");
		if(pIdx != NULL)	{ delete[dwNumIdx](pIdx); }
	}

	// Delete normals array
	if(pamI.has("aryNmls"))
	{
		SFWVertex* pNml = (SFWVertex*)pamI.getDword("aryNmls");
		if(pNml != NULL)	{ delete[dwNumIdx](pNml); }
	}

	// Delete texture map
	for( UINT i = 1; i <= MAX_TEX; ++i)
	{
		string strTex = "texUV" + numToStr<UINT>(i);
		if(pamI.has(strTex))
		{
			float* pUV = (float*)pamI.getDword(strTex);
			if(pUV != NULL)	{ delete[2*dwNumIdx](pUV); }
		}
	}
}

CFWParam CFWMshFcty::MrgPam( CFWParam& pamBasIO, CFWParam pamSubI, UINT uSubIdI)
{
	// Check base whether already has subset
	DWORD dwNumIdxBas = pamBasIO.getDword("numIdxs");
	DWORD dwNumFacBas = dwNumIdxBas / 3;
	if(!pamBasIO.hasSuchHead("subset"))
	{
		pamBasIO.add( "subset0", "0-" + numToStr<DWORD>(dwNumFacBas - 1));
	}
	
	// Adjust subset ID
	string strKey = "subset" + numToStr<UINT>(uSubIdI);
	for( UINT i = uSubIdI + 1; pamBasIO.has(strKey); ++i)
	{
		strKey = "subset" + numToStr<UINT>(i);
	}
	DWORD dwNumIdxSub = pamSubI.getDword("numIdxs");
	DWORD dwNumFacSub = dwNumIdxSub / 3;
	DWORD dwLastFacIdx = dwNumFacBas + dwNumFacSub - 1;
	string strSubFcRg = numToStr<DWORD>(dwNumFacBas);
	strSubFcRg += "-" + numToStr<DWORD>(dwLastFacIdx);
	pamBasIO.add( strKey, strSubFcRg);

	// Take out vertexes' number of base and subset
	DWORD dwNumVtxBas = pamBasIO.getDword("numPois");
	DWORD dwNumVtxSub = pamSubI.getDword("numPois");
	DWORD dwNumVtxNew = dwNumVtxBas + dwNumVtxSub;
	pamBasIO.upd( "numPois", numToStr<DWORD>(dwNumVtxNew));

	// According to the vertex's number, new vertex buffer
	DWORD dwVtxSz = sizeof(SFWVertex);
	SFWVertex* pNewVtx = new SFWVertex[dwNumVtxNew];
	memset( pNewVtx, 0, dwVtxSz * dwNumVtxNew);

	// Merge base vertex and subset vertexes' buffer
	SFWVertex* pBasVtx = (SFWVertex*)pamBasIO.getDword("aryPois");
	memcpy( pNewVtx, pBasVtx, dwVtxSz * dwNumVtxBas);
	SFWVertex* pSubVtx = (SFWVertex*)pamSubI.getDword("aryPois");
	memcpy( pNewVtx + dwNumVtxBas, pSubVtx, dwVtxSz * dwNumVtxSub);
	// Update to base vertexes' buffer
	pamBasIO.upd( "aryPois", numToStr<DWORD>((DWORD)pNewVtx));

	// Take out indexes' number of base and subset
	DWORD dwNumIdxNew = dwNumIdxBas + dwNumIdxSub;
	pamBasIO.upd( "numIdxs", numToStr<DWORD>(dwNumIdxNew));

	// According to the index's number, new index buffer
	DWORD dwIdxSz = sizeof(WORD);
	WORD* pNewIdx = new WORD[dwNumIdxNew];
	memset( pNewIdx, 0, dwIdxSz * dwNumIdxNew);

	// Merge base index and subset indexes' buffer
	WORD* pBasIdx = (WORD*)pamBasIO.getDword("aryIdxs");
	memcpy( pNewIdx, pBasIdx, dwIdxSz * dwNumIdxBas);
	WORD* pSubIdx = (WORD*)pamSubI.getDword("aryIdxs");
	// Subset indexes should plus the base vertex's number
	for( DWORD i = 0; i < dwNumIdxSub; i++)
	{
		pNewIdx[i + dwNumIdxBas] = pSubIdx[i] + dwNumVtxBas;
	}
	// Update to base indexes' buffer
	pamBasIO.upd( "aryIdxs", numToStr<DWORD>((DWORD)pNewIdx));

	// Get and merge UV from base and subset
	DWORD dwFltSz = sizeof(float);
	DWORD dwNumTexBas = 2 * dwNumVtxBas;
	DWORD dwNumTexSub = 2 * dwNumVtxSub;
	for( int i = 0; i < MAX_TEX; ++i)
	{
		// Check base and subset whether has UV
		string strUV = "texUV" + numToStr<int>(i);
		bool bBasHasUV = pamBasIO.has(strUV);
		bool bSubHasUV = pamSubI.has(strUV);
		DWORD dwNumTexNew = 0;
		if(bBasHasUV)	{ dwNumTexNew += dwNumTexBas; }
		if(bSubHasUV)	{ dwNumTexNew += dwNumTexSub; }
		if(dwNumTexNew == 0)	{ continue; }

		// According to the number new UV buffer
		float* pNewUV = new float[dwNumTexNew];
		memset( pNewUV, 0, dwFltSz * dwNumTexNew);

		// Input UV
		if(bBasHasUV)
		{
			float* pUV = (float*)pamBasIO.getDword(strUV);
			memcpy( pNewUV, pUV, dwFltSz * dwNumTexBas);
		}
		if(bSubHasUV)
		{
			float* pUV = (float*)pamSubI.getDword(strUV);
			memcpy( pNewUV + dwNumTexBas, pUV, dwFltSz * dwNumTexSub);
		}
		// Update to base UV buffer
		pamBasIO.upd( strUV, numToStr<DWORD>((DWORD)pNewUV));
	}

	// Merge base normals and subset normals' buffer
	if(pamBasIO.has("aryNmls") && pamSubI.has("aryNmls"))
	{
		SFWVertex* pNewNml = new SFWVertex[dwNumVtxNew];
		memset( pNewNml, 0, dwVtxSz * dwNumVtxNew);

		SFWVertex* pBasNml = (SFWVertex*)pamBasIO.getDword("aryNmls");
		memcpy( pNewNml, pBasNml, dwVtxSz * dwNumVtxBas);
		SFWVertex* pSubNml = (SFWVertex*)pamSubI.getDword("aryNmls");
		memcpy( pNewNml + dwNumVtxBas, pSubNml, dwVtxSz * dwNumVtxSub);

		pamBasIO.upd( "aryNmls", numToStr<DWORD>((DWORD)pNewNml));
	}
	else
	{
		pamBasIO.add( "genNmls", "true");
	}
	ClsCrtePam(pamSubI);
	return pamBasIO;
}