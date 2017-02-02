#pragma once
#include "stdafx.h"
#include "FWMshIntf.h"
#include "FWMesh.h"

using OPMath::CBoxAABB;
using OPMath::CSphere;

CFWMshIntf::CFWMshIntf( CFWObject* pMshI )
{
	if(pMshI->chkIsClass(CFWMesh::getTypID()))
	{
		ptd_pBindObj = pMshI;
		pvt_status = MSH_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = MSH_STT_UNKNOWN;
	}
}

bool CFWMshIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check The Binding Mesh Available
	CFWMesh* pMsh = (CFWMesh*)ptd_pBindObj;
	if(pMsh == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	bool bRet = false;
	switch(pvt_status)
	{
	case MSH_STT_DESTROYED:
	case MSH_STT_CREATED:
		if(sttToI == MSH_STT_INITIALIZED)
		{
			pvt_status = MSH_STT_INITIALIZED;
			return SUCCEEDED(pMsh->initialize(pamIO));
		}
		break;
	case MSH_STT_INITIALIZED:
		switch (sttToI)
		{
		case MSH_STT_DRAWING:
			pvt_status = MSH_STT_DRAWING;
			bRet = SUCCEEDED(pMsh->draw(
				pamIO.has("subsetID") ?
				pamIO.getUint("subsetID") : 0));
			pvt_status = MSH_STT_INITIALIZED;
			return bRet;
		case MSH_STT_DESTROYED:
			pvt_status = MSH_STT_DESTROYED;
			pMsh->destroy();
		}
		break;
	}
	return false;
}

HRESULT CFWMshIntf::Draw(UINT uSubIdI)
{
	// Check The Binding Mesh Available
	CFWMesh* pMsh = (CFWMesh*)ptd_pBindObj;
	if(pMsh == NULL || pvt_status < MSH_STT_INITIALIZED)	{ return E_FAIL; }

	pvt_status = MSH_STT_DRAWING;
	HRESULT hr = pMsh->draw(uSubIdI);
	if(SUCCEEDED(hr))	{ pvt_status = MSH_STT_INITIALIZED; }
	return hr;
}

bool CFWMshIntf::HasSubset(UINT uSubIdI)
{
	// Check The Binding Mesh Available
	CFWMesh* pMsh = (CFWMesh*)ptd_pBindObj;
	if(pMsh == NULL)	{ return false; }
	// @_@: 
	if(pMsh->chkIsClass(CFWPrim::getTypID()))
	{
		return true;
	}

	return pMsh->hasSubset(uSubIdI);
}

CFWAABBBox CFWMshIntf::GeneOutSrdBox() const
{
	// Check The Binding Mesh Available
	CFWMesh* pMsh = (CFWMesh*)ptd_pBindObj;
	if(pMsh == NULL)	{ return CFWAABBBox(); }

	return pMsh->geneOutSrdBox();
}

SFWQuate CFWMshIntf::GeneOutSrdSph() const
{
	// Check The Binding Mesh Available
	CFWMesh* pMsh = (CFWMesh*)ptd_pBindObj;
	if(pMsh == NULL)	{ return SFWQuate(); }

	CSphere sphRet = pMsh->geneOutSrdSph();
	SFWQuate qatRet = sphRet.getPoiCenter();
	qatRet.m_fH = sphRet.getRadius();
	return qatRet;
}
