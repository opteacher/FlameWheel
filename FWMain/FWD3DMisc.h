#pragma once
#include "FWMacro.h"
#include "FWMaterial.h"
#include "d3dx9.h"

FWMAIN_API D3DCOLORVALUE tunToD3DCol(const SFWColor& colI);

FWMAIN_API SFWColor tunToFWCol(D3DCOLORVALUE colI);

FWMAIN_API D3DMATERIAL9 tunToD3DMtrl(const SFWMaterial& mtrlI);

FWMAIN_API SFWMaterial tunToFWMtrl(D3DMATERIAL9 mtrlI);