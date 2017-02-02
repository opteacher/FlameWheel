#pragma once
#include "stdafx.h"
#include "FWCamera.h"
#include "../OPMath/Math.h"

INI_TYPID(CFWCamFree)

void CFWCamFree::rollByX(float fAngI)
{
	if(ptd_bLocked)	{ return; }
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &ptd_dirRight, fAngI);
	D3DXVec3TransformCoord( &ptd_dirUp, &ptd_dirUp, &T);
	D3DXVec3TransformCoord( &ptd_dirLook, &ptd_dirLook, &T);
}

void CFWCamFree::rollByY(float fAngI)
{
	if(ptd_bLocked)	{ return; }
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &ptd_dirUp, fAngI);
	D3DXVec3TransformCoord( &ptd_dirRight, &ptd_dirRight, &T);
	D3DXVec3TransformCoord( &ptd_dirLook, &ptd_dirLook, &T);
}

void CFWCamFree::rollByZ(float fAngI)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &ptd_dirLook, fAngI);
	D3DXVec3TransformCoord( &ptd_dirRight, &ptd_dirRight, &T);
	D3DXVec3TransformCoord( &ptd_dirUp, &ptd_dirUp, &T);
}

void CFWCamFree::moveByX(float fDisI)
{
	D3DXVECTOR3 dirMv = ptd_dirRight*fDisI;
	ptd_posEye += dirMv;
	updTgtLoc(CVT_POI_FW(dirMv));
}

void CFWCamFree::moveByY(float fDisI)
{
	D3DXVECTOR3 dirMv = ptd_dirUp*fDisI;
	ptd_posEye += dirMv;
	updTgtLoc(CVT_POI_FW(dirMv));
}

void CFWCamFree::moveByZ(float fDisI)
{
	D3DXVECTOR3 dirMv = ptd_dirLook*fDisI;
	ptd_posEye += dirMv;
	updTgtLoc(CVT_POI_FW(dirMv));
}

void CFWCamFree::updTgtLoc(CVector dirMvI)
{
	if(ptd_bLocked)
	{
		ptd_dirLook = ptd_posTar - ptd_posEye;
		lookAt(CVT_POI_FW(ptd_dirLook));
	}
	else
	{
		ptd_posTar += CVT_POI_D3D(dirMvI);
	}
}