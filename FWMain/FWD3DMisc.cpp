#pragma once
#include "stdafx.h"
#include "FWD3DMisc.h"

FWMAIN_API D3DCOLORVALUE tunToD3DCol(const SFWColor& colI)
{
	D3DCOLORVALUE colD3DRet;
	colD3DRet.a = (float)colI.m_iAlph/255;
	colD3DRet.r = (float)colI.m_iRed/255;
	colD3DRet.g = (float)colI.m_iGreen/255;
	colD3DRet.b = (float)colI.m_iBlue/255;
	return colD3DRet;
}

FWMAIN_API SFWColor tunToFWCol(D3DCOLORVALUE colI)
{
	return SFWColor( colI.r, colI.g, colI.b, colI.a);
}

FWMAIN_API D3DMATERIAL9 tunToD3DMtrl( const SFWMaterial& mtrlI )
{
	D3DMATERIAL9 mtrlRet;
	mtrlRet.Diffuse = tunToD3DCol(mtrlI.m_colDiffuse);
	mtrlRet.Ambient = tunToD3DCol(mtrlI.m_colAmbient);
	mtrlRet.Specular = tunToD3DCol(mtrlI.m_colSpecular);
	mtrlRet.Emissive = tunToD3DCol(mtrlI.m_colEmissive);
	mtrlRet.Power = mtrlI.m_fPower;
	return mtrlRet;
}

FWMAIN_API SFWMaterial tunToFWMtrl(D3DMATERIAL9 mtrlI)
{
	SFWMaterial mtlRet;
	mtlRet.m_colDiffuse = tunToFWCol(mtrlI.Diffuse);
	mtlRet.m_colAmbient = tunToFWCol(mtrlI.Ambient);
	mtlRet.m_colSpecular = tunToFWCol(mtrlI.Specular);
	mtlRet.m_colEmissive = tunToFWCol(mtrlI.Emissive);
	mtlRet.m_fPower = mtrlI.Power;
	return mtlRet;
}