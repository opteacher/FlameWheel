#pragma once
#include "FWObject.h"
#include "FWParam.h"
#include "FWMisc.h"
#include "d3dx9.h"
#include "../OPMath/Math.h"
#include <set>

class CFWMesh: public CFWObject
{
public: friend class CFWMshFcty;
public:
	CFWMesh():
		ptd_lpMesh(NULL),
		ptd_lpVtxBuf(NULL),
		ptd_lpIdxBuf(NULL),
		ptd_lpDecl(NULL)	{}
	virtual ~CFWMesh()	{ destroy(); }
protected:
	LPD3DXMESH ptd_lpMesh;
	LPDIRECT3DVERTEXBUFFER9 ptd_lpVtxBuf;	//Vertex Buffer
	LPDIRECT3DINDEXBUFFER9 ptd_lpIdxBuf;	//Index Buffer
	LPDIRECT3DVERTEXDECLARATION9 ptd_lpDecl;//Vertex Group
	DWORD ptd_dwNumVtx;						//Number Vertexes
	DWORD ptd_dwNumFac;						//Number Faces
	DWORD ptd_dwPerVtx;						//Vertex's Size
	DWORD ptd_dwFVF;						//Vertex's FVF(defined which informations in every vertex)
	D3DCOLORVALUE ptd_colVtx;				//Mesh color
	DWORD* ptd_ayAdjBuf;					//Edges Array
	std::set<DWORD> ptd_stSubID;			//Subset ID set
public:
	virtual HRESULT initialize(CFWParam pamI);
	virtual HRESULT draw(UINT uSubIdI = 0);
	virtual void destroy();
public:
	virtual OPMath::CBoxAABB geneOutSrdBox() const;
	virtual OPMath::CSphere geneOutSrdSph() const;
public:
	bool hasSubset(UINT uSubIdI) const	{ return CHK_EXS_STL( ptd_stSubID, uSubIdI); }
public: CHK_IS_CLASS;
};

class CFWPrim: public CFWMesh
{
public: friend class CFWMshFcty;
protected: UINT ptd_uPrimTyp;
public:
	virtual HRESULT initialize(CFWParam pamI);
	virtual HRESULT draw(UINT uSubIdI = 0);
public: CHK_IS_CLASS;
};