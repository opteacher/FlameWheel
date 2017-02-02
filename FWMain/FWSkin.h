#pragma once
#include "FWMacro.h"
#include "FWObject.h"
#include "FWMaterial.h"
#include "d3dx9.h"
#include "windows.h"
#include <string>
using std::string;

//Max support textures
#ifndef MAX_TEX
#define MAX_TEX	8
#endif

class CFWSkin: public CFWObject
{
public: friend class CFWSknFcty;
public:
	CFWSkin();
	virtual ~CFWSkin()	{ destroy(); }
protected:
	D3DMATERIAL9 ptd_mtrl;
	LPDIRECT3DBASETEXTURE9 ptd_ayTex[MAX_TEX];
	std::string ptd_ayTexFile[MAX_TEX];
	DWORD ptd_dwNumTex;
public:
	virtual HRESULT actSkin();
	virtual HRESULT clsFmBlend();
	virtual void destroy();
public:
	virtual void setMtl(const SFWMaterial&);
	virtual inline SFWMaterial getMtl() const;
	virtual void setTex( string, UINT);
	virtual inline string getTex(UINT) const;
public: CHK_IS_CLASS;
};