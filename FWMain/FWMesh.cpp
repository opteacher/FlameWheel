#pragma once
#include "stdafx.h"
#include "FWMesh.h"
#include "FWSkin.h"
#include "FWMisc.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWD3DMisc.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
using OPTools::strToNum;

using OPMath::CPoint;
using OPMath::CBoxAABB;
using OPMath::CSphere;

INI_TYPID(CFWMesh);

HRESULT CFWMesh::initialize(CFWParam pamI)
{
	// Get current device
	CFWDevIntf* pCurDev = (CFWDevIntf*)CFWDevFcty::getInstance()->getCurDev();
	if(!pCurDev || pCurDev->GetStatus() != CFWDevIntf::DEV_STT_INITIALIZED)
	{
		return E_FAIL;
	}
	LPDIRECT3DDEVICE9 pD3DDev = (LPDIRECT3DDEVICE9)pCurDev->GetImpDev();

	// Check necessary parameters' existed
	if(!pamI.has("aryPois") || !pamI.has("aryIdxs"))	{ return E_FAIL; }

	// Create a D3D mesh for storing vertexes and indexes
	if(FAILED(::D3DXCreateMeshFVF(
		ptd_dwNumFac,
		ptd_dwNumVtx,
		D3DXMESH_MANAGED,
		ptd_dwFVF,
		pD3DDev,
		&ptd_lpMesh)))	{ return E_FAIL; }
	DWORD dwNumIdx = ptd_dwNumFac * 3;

	// Collect vertexes' data from parameter
	SFWVertex* ayPois = (SFWVertex*)pamI.getDword("aryPois");
	SFWVertex* ayNmls = (pamI.has("aryNmls")) ? (SFWVertex*)pamI.getDword("aryNmls") : NULL;
	bool bGenNmls     = (pamI.has("genNmls")) ? pamI.getBool("genNmls") : (ayNmls == NULL);

	// Input vertex into mesh vertexes' buffer
	LPBYTE pVtxBuf = NULL;
	if(FAILED(ptd_lpMesh->LockVertexBuffer( 0, (LPVOID*)&pVtxBuf)))	{ return E_FAIL; }
	const DWORD dwSzVtx = sizeof(SFWVertex);
	const DWORD dwSzCol = sizeof(DWORD);
	const DWORD dwSzUV = sizeof(float) * 2;
	DWORD dwVtxCol = COL_TO_DWD(ptd_colVtx);
	for( DWORD i = 0; i < ptd_dwNumVtx; ++i)
	{
		// Vertexes' location information input into memory
		memcpy( pVtxBuf, &ayPois[i], dwSzVtx); pVtxBuf += dwSzVtx;

		// Check normal vectors exited
		if(ptd_dwFVF & D3DFVF_NORMAL)
		{
			if(ayNmls != NULL)	{ memcpy( pVtxBuf, &ayNmls[i], dwSzVtx); }
			// +_+: If the mode include normal or others, the memory must be hold
			pVtxBuf += dwSzVtx;
		}

		// Check vertexes' color existed
		if(ptd_dwFVF & D3DFVF_DIFFUSE)
		{
			memcpy( pVtxBuf, &dwVtxCol, dwSzCol); pVtxBuf += dwSzCol;
		}
		
		// Check textures' UV map()
		for( UINT uIdxTex = 1; uIdxTex < MAX_TEX; ++uIdxTex)
		{
			string strTexUV = "texUV" + numToStr<UINT>(uIdxTex);
			if((ptd_dwFVF & (D3DFVF_TEX1*uIdxTex) && pamI.has(strTexUV)))
			{
				float* pTexUV = (float*)pamI.getDword(strTexUV);
				memcpy( pVtxBuf, &pTexUV[2 * i], dwSzUV);
				pVtxBuf += dwSzUV;
			}
		}
	}
	if(FAILED(ptd_lpMesh->UnlockVertexBuffer()))	{ return E_FAIL; }

	// Collect indexes' data from parameter
	LPDWORD ayIdxs = (LPDWORD)pamI.getDword("aryIdxs");

	// Input indexes into mesh indexes' buffer
	LPWORD pIdxBuf = NULL;
	if(FAILED(ptd_lpMesh->LockIndexBuffer( 0, (LPVOID*)&pIdxBuf)))	{ return E_FAIL; }
	memcpy( pIdxBuf, ayIdxs, sizeof(WORD)*dwNumIdx);
	if(FAILED(ptd_lpMesh->UnlockIndexBuffer()))	{ return E_FAIL; }

	// Check attribute groups existed, and split it by parameter
	std::map<UINT, std::pair<DWORD, DWORD>> mpSubset;
	typedef std::map<UINT, std::pair<DWORD, DWORD>>::value_type VtpSubRg;
	typedef std::map<UINT, std::pair<DWORD, DWORD>>::iterator ItrSubRg;
	// Review all key in the parameter
	std::vector<string> ayKey = pamI.getKeySet();
	for( UINT i = 0; i < ayKey.size(); ++i)
	{
		// Find whether have subset key in the parameter
		string strTmp = ayKey[i];
		UINT uLocSub = strTmp.find("subset");
		if(uLocSub == string::npos)	{ continue; }

		// If existed, take the ID after "subset"
		UINT uSubID = strToNum<UINT>(strTmp.substr(uLocSub + 6));
		
		// Get face set range according to the key
		// (+_+: Which means face set must continuity)
		string strSubRg = pamI.getString(strTmp);
		UINT uLocSign = strSubRg.find('-');
		if(uLocSign == string::npos)	{ return E_FAIL; }
		std::pair<DWORD, DWORD> prFcRg;
		prFcRg.first = strToNum<DWORD>(strSubRg.substr( 0, uLocSign));
		prFcRg.second = strToNum<DWORD>(strSubRg.substr(uLocSign + 1));
		// Check face set range whether correct
		if(prFcRg.first >= prFcRg.second)	{ return E_FAIL; }
		if(prFcRg.second >= ptd_dwNumFac)	{ return E_FAIL; }

		// Insert into subset map and store subset ID for drawing check
		mpSubset.insert(VtpSubRg( uSubID, prFcRg));
		ptd_stSubID.insert(uSubID);
	}

	// Input face attribute into mesh face group buffer
	LPDWORD pAttrBuf = NULL;
	if(FAILED(ptd_lpMesh->LockAttributeBuffer( 0, &pAttrBuf)))	{ return E_FAIL; }
	if(mpSubset.empty())
	{
		memset( pAttrBuf, 0, sizeof(DWORD)*ptd_dwNumFac);
		ptd_stSubID.insert(0);
	}
	else
	{
		for( ItrSubRg iter = mpSubset.begin(); iter != mpSubset.end(); ++iter)
		{
			std::pair<DWORD, DWORD> prFcRg = iter->second;
			for( DWORD i = prFcRg.first; i <= prFcRg.second; ++i)
			{
				pAttrBuf[i] = iter->first;
			}
		}
	}
	if(FAILED(ptd_lpMesh->UnlockAttributeBuffer()))	{ return E_FAIL; }

	// Generate edges' information
	ptd_ayAdjBuf = new DWORD[dwNumIdx];
	if(FAILED(ptd_lpMesh->GenerateAdjacency( 0.0f, ptd_ayAdjBuf)))	{ return E_FAIL; }

	// Generate normal vector
	if(bGenNmls)
	{
		if(FAILED(D3DXComputeNormals( ptd_lpMesh, ptd_ayAdjBuf)))	{ return E_FAIL; }
	}

	// Weld vertexes
	if(pamI.has("weldVtx") && pamI.getBool("weldVtx"))
	{
		if(FAILED(D3DXWeldVertices(
			ptd_lpMesh,
			D3DXWELDEPSILONS_WELDALL,
			NULL,
			NULL,
			ptd_ayAdjBuf,
			NULL,
			NULL)))	{ return E_FAIL; }
	}

	// Optimize the mesh
	if(!pamI.has("optimize") || (pamI.has("optimize") && pamI.getBool("optimize")))
	{
		if(FAILED(ptd_lpMesh->OptimizeInplace(
			D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
			ptd_ayAdjBuf,
			0,
			0,
			0)))	{ return E_FAIL; }
	}

	// Get information from mesh
	ptd_lpMesh->GetVertexBuffer(&ptd_lpVtxBuf);
	ptd_lpMesh->GetIndexBuffer(&ptd_lpIdxBuf);
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	ptd_lpMesh->GetDeclaration(decl);
	if(FAILED(pD3DDev->CreateVertexDeclaration( decl, &ptd_lpDecl)))	{ return E_FAIL; }

	// Because did weld vertexes and optimize, update mesh self
	ptd_dwNumVtx = ptd_lpMesh->GetNumVertices();
	ptd_dwNumFac = ptd_lpMesh->GetNumFaces();
	ptd_dwPerVtx = ptd_lpMesh->GetNumBytesPerVertex();
	ptd_dwFVF = ptd_lpMesh->GetFVF();
	
	return S_OK;
}

HRESULT CFWMesh::draw(UINT uSubIdI)
{
	if(!CHK_EXS_STL( ptd_stSubID, uSubIdI))	{ return E_FAIL; }
	if(ptd_lpMesh)	{ return ptd_lpMesh->DrawSubset(uSubIdI); }
	return E_FAIL;
}

void CFWMesh::destroy()
{
	SAFE_RELEASE(ptd_lpMesh);
	SAFE_RELEASE(ptd_lpVtxBuf);
	SAFE_RELEASE(ptd_lpIdxBuf);
	SAFE_RELEASE(ptd_lpDecl);
	SAFE_DELETE(ptd_ayAdjBuf);
}

CBoxAABB CFWMesh::geneOutSrdBox() const
{
	if(ptd_lpMesh == NULL)	{ return CBoxAABB(); }

	D3DXVECTOR3* pVtxBuf	= NULL;
	ptd_lpMesh->LockVertexBuffer( 0, (LPVOID*)&pVtxBuf);

	D3DXVECTOR3 poiLTF, poiRBB;
	if(FAILED(D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVtxBuf,
		ptd_dwNumVtx,
		D3DXGetFVFVertexSize(ptd_dwFVF),
		&poiLTF,
		&poiRBB)))	{ return CBoxAABB(); }

	ptd_lpMesh->UnlockVertexBuffer();

	return CBoxAABB(
		CPoint( poiLTF.x, poiLTF.y, poiLTF.z),
		CPoint( poiRBB.x, poiRBB.y, poiRBB.z));
}

CSphere CFWMesh::geneOutSrdSph() const
{
	if(ptd_lpMesh == NULL)	{ return CSphere(); }

	D3DXVECTOR3* pVtxBuf = NULL;
	ptd_lpMesh->LockVertexBuffer( 0, (LPVOID*)&pVtxBuf);

	D3DXVECTOR3 poiCenter;
	float fRadius = 0.0f;
	if(FAILED(D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVtxBuf,
		ptd_dwNumVtx,
		D3DXGetFVFVertexSize(ptd_dwFVF),
		&poiCenter,
		&fRadius)))	{ return CSphere(); }
	CPoint poiCen( poiCenter.x, poiCenter.y, poiCenter.z);

	ptd_lpMesh->UnlockVertexBuffer();

	return CSphere( poiCen, fRadius);
}
