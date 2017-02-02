#pragma once
#include "FWObject.h"
#include "FWParam.h"
#include "d3dx9.h"

class CFWText: public CFWObject
{
public:
	virtual HRESULT initialize(CFWParam)	{ return S_OK; }
	virtual HRESULT destroy()	{ return S_OK; }
	virtual HRESULT draw(CFWParam) = 0;
public: CHK_IS_CLASS;
};

class CFWText2D: public CFWText
{
public:
	CFWText2D(): ptd_pFont(NULL)	{}
protected:
	LPD3DXFONT ptd_pFont;
public:
	virtual HRESULT initialize(CFWParam);
	virtual HRESULT destroy();
	virtual HRESULT draw(CFWParam);
public: CHK_IS_CLASS;
};