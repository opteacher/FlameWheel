#pragma once
#include "FWNode.h"
#include "../OPMath/Math.h"
#include "../OPTools/String.h"
using OPTools::numToStr;

class FWMAIN_API CFWScene: public CFWNode
{
public: friend class CFWScnMngr;
public: CFWScene(): CFWNode("Scene" + numToStr<DWORD>(s_stUsedNams.size()))	{}
protected:
	virtual HRESULT preRender();
	virtual HRESULT sufRender();

public: CHK_IS_CLASS;
};