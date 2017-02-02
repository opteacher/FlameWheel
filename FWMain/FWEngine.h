#pragma once
#include "FWMacro.h"
#include "FWParam.h"

// @_@: For now, this is a export class, 
// but it should be set into interface and export
class FWMAIN_API CFWEngine
{
	SINGLE_DECLARE(CFWEngine)
public:
	/*
	*	Required parameters:
	*		"wndHdl" = Render window's handle
	*		[N]"cfgFile" = Config files
	*/
	virtual HRESULT initialize(CFWParam);
	virtual void display(float fDeltaI);
	virtual void clearUp();
};