#pragma once
#include "FWMacro.h"
#include "FWObject.h"
#include "FWParam.h"
#include "d3dx9.h"
#include "../OPMath/Math.h"

class CFWEffect: public CFWObject
{
public: virtual HRESULT enable( CFWParam, bool = true) = 0;
public: CHK_IS_CLASS;
};

class CFWEffAmbLgt: public CFWEffect
{
	SINGLE_DECLARE(CFWEffAmbLgt)
	//	Required parameter:	"lgtCol" = Light color, default as white
public: virtual HRESULT enable( CFWParam, bool = true);
public: CHK_IS_CLASS;
};

class CFWEffShwEdg: public CFWEffect
{
	SINGLE_DECLARE(CFWEffShwEdg)
	//	Required parameter:	"mtlEdg" = Edge's material
	//								If not given, effect will take current
	//								action skin's material's negative to set.
	//								If there's no action skin, it will use the
	//								background color of device.
	//						"mshID" = Defined which mesh to show edge.
	//								If not given, effect will take current 
	//								drawing mesh to show
public: virtual HRESULT enable( CFWParam, bool = true);
private: D3DMATERIAL9 pvt_mtlFmr;
public: CHK_IS_CLASS;
};

class CFWEffAdvBd: public CFWEffect
{
	SINGLE_DECLARE(CFWEffAdvBd)
	// +_+: 
public: virtual HRESULT enable( CFWParam, bool = true);
private:
	OPMath::CVector pvt_dirLook;
	OPMath::CMatrix pvt_matToCam;
public: CHK_IS_CLASS;
};

class CFWEffZDepth: public CFWEffect
{
	SINGLE_DECLARE(CFWEffZDepth)
public: virtual HRESULT enable( CFWParam, bool = true);
public: CHK_IS_CLASS;
};

class CFWEffAlpha: public CFWEffect
{
	SINGLE_DECLARE(CFWEffAlpha)
public: virtual HRESULT enable( CFWParam, bool = true);
public: CHK_IS_CLASS;
};