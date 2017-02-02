#pragma once
#include "stdafx.h"
#include "FWSpace.h"

INI_TYPID(CFWRigid);

CBoxAABB CFWRigid::getOutSrdAABBBox() const
{
	return CBoxAABB(
		ptd_boxOL.getPoiLTF() + ptd_codLoc.m_poiCenter,
		ptd_boxOL.getPoiRBB() + ptd_codLoc.m_poiCenter);
}

CSphere CFWRigid::getOutSrdSphere() const
{
	return CSphere( ptd_codLoc.m_poiCenter, ptd_sphOL.getRadius());
}

HRESULT CFWRigid::update(CFWParam pamI)
{
	ptd_dirSpeed += ptd_dirAccel;
	ptd_codLoc.m_poiCenter += ptd_dirSpeed;
	return S_OK;
}