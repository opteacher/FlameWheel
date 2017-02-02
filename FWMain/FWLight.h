#pragma once
#include "FWObject.h"
#include "FWMisc.h"
#include "d3dx9.h"

#ifndef MAX_LIGHT
#define MAX_LIGHT	9
#endif

class CFWLight: public CFWObject
{
public: friend class CFWLgtFcty;
public: CFWLight();
protected:
	D3DLIGHT9 ptd_lgtD3D;
	int ptd_iUsedID;
	bool ptd_bEnable;
public:
	virtual HRESULT setLgt(int iLgtIdI);
	virtual HRESULT enable(bool = true);
public:
	// Only for point light and spot light
	virtual SFWQuate getLgtLoc() const;
	virtual UINT getLgtType() const;
public: CHK_IS_CLASS;
};