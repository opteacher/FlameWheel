#pragma once
#include "stdafx.h"
#include "FWCamera.h"
#include "FWMacro.h"

INI_TYPID(CFWCamera);

void CFWCamera::regulate()
{
	D3DXVec3Normalize( &ptd_dirLook, &ptd_dirLook);
	D3DXVECTOR3 vecYI( 0.0f, 1.0f, 0.0f);
	if(vecYI.y == ptd_dirLook.y || vecYI.y == -ptd_dirLook.y)
	{
		vecYI	= D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	}

	D3DXVec3Cross( &ptd_dirRight, &vecYI, &ptd_dirLook);
	D3DXVec3Cross( &ptd_dirUp, &ptd_dirLook, &ptd_dirRight);
	D3DXVec3Normalize( &ptd_dirUp, &ptd_dirUp);
	D3DXVec3Cross( &ptd_dirRight, &ptd_dirUp, &ptd_dirLook);
	D3DXVec3Normalize( &ptd_dirRight, &ptd_dirRight);
}

void CFWCamera::lookAt( CVector dirLookI )
{
	ptd_dirLook = CVT_POI_D3D(dirLookI);
	D3DXVec3Normalize( &ptd_dirLook, &ptd_dirLook);
	regulate();
}

CMatrix CFWCamera::getProjMat()
{
	//calculate project matrix by d3d function
	D3DXMATRIXA16 matProj;
	float fAspectRatio = (float)ptd_uViewWid / (float)ptd_uViewHgt;
	if(!ptd_fClsPln)	{ ptd_fClsPln = 1; }
	if(!ptd_fFarPln)	{ ptd_fFarPln = 1000; }
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI/2,
		fAspectRatio,
		ptd_fClsPln,
		ptd_fFarPln);

	return CMatrix(matProj.m);
}

CMatrix CFWCamera::getViewMat()
{
	D3DXVec3Normalize( &ptd_dirLook, &ptd_dirLook);
	D3DXVec3Cross( &ptd_dirUp, &ptd_dirLook, &ptd_dirRight);
	D3DXVec3Normalize( &ptd_dirUp, &ptd_dirUp);
	D3DXVec3Cross( &ptd_dirRight, &ptd_dirUp, &ptd_dirLook);
	D3DXVec3Normalize( &ptd_dirRight, &ptd_dirRight);

	float x = -D3DXVec3Dot( &ptd_dirRight, &ptd_posEye);
	float y = -D3DXVec3Dot( &ptd_dirUp, &ptd_posEye);
	float z = -D3DXVec3Dot( &ptd_dirLook, &ptd_posEye);

	D3DXMATRIX matRet;
	(matRet)(0,0) = ptd_dirRight.x;	(matRet)(0, 1) = ptd_dirUp.x;	(matRet)(0, 2) = ptd_dirLook.x;	(matRet)(0, 3) = 0.0f;
	(matRet)(1,0) = ptd_dirRight.y;	(matRet)(1, 1) = ptd_dirUp.y;	(matRet)(1, 2) = ptd_dirLook.y;	(matRet)(1, 3) = 0.0f;
	(matRet)(2,0) = ptd_dirRight.z;	(matRet)(2, 1) = ptd_dirUp.z;	(matRet)(2, 2) = ptd_dirLook.z;	(matRet)(2, 3) = 0.0f;
	(matRet)(3,0) = x;				(matRet)(3, 1) = y;				(matRet)(3, 2) = z;				(matRet)(3, 3) = 1.0f;

	return CMatrix(matRet.m);
}

CSqCone CFWCamera::getViewCone()
{
	D3DXMATRIXA16 matWld;
	D3DXMatrixMultiply( &matWld,
		new D3DXMATRIXA16((FLOAT*)getViewMat().m_fVal),
		new D3DXMATRIXA16((FLOAT*)getProjMat().m_fVal));

	CSqCone cnView;
	// Close plane
	cnView.insertPln(CPlane(
		matWld._13,
		matWld._23,
		matWld._33,
		matWld._43));
	// Farther plane
	cnView.insertPln(CPlane(
		matWld._14 - matWld._13,
		matWld._24 - matWld._23,
		matWld._34 - matWld._33,
		matWld._44 - matWld._43));
	// Left plane
	cnView.insertPln(CPlane(
		matWld._14 + matWld._11,
		matWld._24 + matWld._21,
		matWld._34 + matWld._31,
		matWld._44 + matWld._41));
	// Right plane
	cnView.insertPln(CPlane(
		matWld._14 - matWld._11,
		matWld._24 - matWld._21,
		matWld._34 - matWld._31,
		matWld._44 - matWld._41));
	// Top plane
	cnView.insertPln(CPlane(
		matWld._14 - matWld._12,
		matWld._24 - matWld._22,
		matWld._34 - matWld._32,
		matWld._44 - matWld._42));
	// Bottom plane
	cnView.insertPln(CPlane(
		matWld._14 + matWld._12,
		matWld._24 + matWld._22,
		matWld._34 + matWld._32,
		matWld._44 + matWld._42));

	// Make inside mode
	D3DXVECTOR3 poiTmp = ptd_posEye;
	poiTmp += ptd_dirLook * abs(ptd_fFarPln - ptd_fClsPln)*.5;
	cnView.mkInside(CVT_POI_FW(poiTmp));

	return cnView;
}

