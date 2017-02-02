#pragma once
#include "FWMacro.h"
#include "FWIntf.h"
#include "FWObject.h"

class FWMAIN_API CFWLgtIntf: public CFWIntf
{
public:
	CFWLgtIntf():
		pvt_status(LGT_STT_UNKNOWN)	{}
	CFWLgtIntf(CFWObject* pLgtI);
public:
	enum EFELgtTyp
	{
		LGT_TP_POINT,
		LGT_TP_SPOT,
		LGT_TP_DIRECT,
		LGT_TP_UNKNOW
	};
	enum EFWLgtStt
	{
		LGT_STT_UNKNOWN,
		LGT_STT_CREATED,
		LGT_STT_BE_SET,
		LGT_STT_BE_ENB
	};
	/*
	*	LGT_STT_CREATED -> LGT_STT_BE_SET	cause light->setLgt()
	*						required parameter: [N]"lgtID" = Identified which ID set to
	*	LGT_STT_BE_SET -> LGT_STT_BE_ENB	cause light->enable()
	*						required parameter: "enable" = Identified which ID set to
	*	LGT_STT_BE_ENB -> LGT_STT_BE_SET	cause light->enable(false)
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private: EFWLgtStt pvt_status;

public:
	virtual SFWQuate GetLgtLoc() const;
	virtual UINT GetLgtType() const;
};