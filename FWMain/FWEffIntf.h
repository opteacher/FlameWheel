#pragma once
#include "FWMacro.h"
#include "FWIntf.h"

class FWMAIN_API CFWEffIntf: public CFWIntf
{
public:
	CFWEffIntf():
		pvt_status(EFF_STT_UNKNOWN)	{}
	CFWEffIntf(CFWObject* pEffI);
public:
	enum EFWEffStt
	{
		EFF_STT_UNKNOWN,
		EFF_STT_DSABLE,
		EFF_STT_ENABLE
	};
	/*
	*	EFF_STT_DSABLE -> EFF_STT_ENABLE cause effect->enable(pam)
	*	EFF_STT_ENABLE -> EFF_STT_DSABLE cause effect->enable( pam, false)
	*	Required parameter depend on which effect
	*		Ambient Light:	[E]"lgtCol" = Light color
	*		Show Edge:		[E]"mtlEdg" = Edge's material
	*
	*	+_+: If transfer to status equal to current status, the effect wont
	*	waste system resource to reactive again, and the input parameter will
	*	be add a value by key "effNoChg" to identified user.
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private: EFWEffStt pvt_status;
	
public:
	//	Required parameter: "enable" = Whether enable effect
	//		Others were the same to TransferTo()
	inline bool Enable(CFWParam& pamIO);
	// Switch effect's status
	inline bool Negative(CFWParam& pamIO);
};