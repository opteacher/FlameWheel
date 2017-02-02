#pragma once
#include "FWMacro.h"
#include "FWObject.h"
#include "FWColor.h"
#include "../OPMath/Math.h"
using OPMath::CMatrix;
#include "d3dx9.h"
#include <string>
using std::string;

class FWMAIN_API CFWDevice: public CFWObject
{
public: typedef std::pair<UINT, UINT> PrScnWH;
public:
	friend class CFWDevFcty;
	CFWDevice():
		ptd_lpAdapter(NULL),
		ptd_lpDevice(NULL),
		ptd_lpSwapChain(NULL)	{}
	virtual ~CFWDevice()	{ destory(); }
protected:
	D3DDEVTYPE ptd_tpDev;
	LPDIRECT3D9 ptd_lpAdapter;
	D3DPRESENT_PARAMETERS ptd_pmPresent;
	LPDIRECT3DDEVICE9 ptd_lpDevice;
	LPDIRECT3DSWAPCHAIN9 ptd_lpSwapChain;
	D3DCOLORVALUE ptd_colBackGd;
public:
	virtual HRESULT initialize();
	virtual HRESULT restore();
	virtual void destory();
public:
	virtual HRESULT begPaint();
	virtual HRESULT endPaint();
public:
	FUNC_GET( ptd_pmPresent.hDeviceWindow, DispWnd, HWND);
	virtual CMatrix setWldMat(CMatrix);
	virtual CMatrix getWldMat() const;
	virtual void setViewMat(CMatrix);
	virtual void setProjMat(CMatrix);
	virtual inline SFWColor getBkGrdCol() const;
	virtual FUNC_GET( ptd_lpDevice, ImpDev, LPVOID);
public: CHK_IS_CLASS;
};
