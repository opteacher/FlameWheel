#pragma once
#include "FWIntf.h"

class FWMAIN_API CFWSknIntf: public CFWIntf
{
public:
	CFWSknIntf():
		pvt_status(SKN_STT_UNKNOWN)	{}
	CFWSknIntf(CFWObject* pSknI);
public:
	enum EFWTexTyp
	{
		TEX_DI,	//!< Diffuse
		TEX_AM,	//!< Ambient 
		TEX_SP,	//!< Specular
		TEX_OP,	//!< Opacity
		TEX_BU,	//!< Bump 
		TEX_RL,	//!< Reflection
		TEX_RR,	//!< Refraction 
		TEX_DP	//!< Displacement 
	};
	enum EFWSknStt
	{
		SKN_STT_UNKNOWN,
		SKN_STT_CREATED,
		SKN_STT_ACTIVED,
		SKN_STT_DESTROYED
	};
	/*
	*	
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWSknStt pvt_status;

public:
	virtual HRESULT ClsFmBlend();
	virtual SFWMaterial GetMtrl() const;
	virtual SFWMaterial SetMtrl(SFWMaterial);
	virtual string SetTex( UINT, string);
	virtual string GetTexFile(UINT) const;
};