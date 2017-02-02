#pragma once
#include "FWMacro.h"
#include "FWIntf.h"

class FWMAIN_API CFWTxtIntf: public CFWIntf
{
public:
	CFWTxtIntf():
		pvt_status(TXT_STT_UNKNOWN)	{}
	CFWTxtIntf(CFWObject*);
public:
	enum EFWTxtStt
	{
		TXT_STT_UNKNOWN,
		TXT_STT_CREATED,
		TXT_STT_INITIALIZED,
		TXT_STT_PAINTING
	};
	enum EFWDockInRect
	{
		LFT_TOP,	TOP_CEN,	RGT_TOP,
		LFT_CEN,	CENTER,		RGT_CEN,
		LFT_BTM,	BTM_CEN,	RGT_BTM
	};
	/*
	*	TXT_STT_CREATED -> TXT_STT_INITIALIZED
	*		Cause to call CFWTextXX->initialize()
	*		Required parameters:
	*			"fontName" = Font's name
	*	TXT_STT_INITIALIZED -> TXT_STT_PAINTING
	*		Cause to call CFWTextXX->draw()
	*		Required parameters:
	*			[N]"showTxt" = String for show
	*			"outSrdRect" = Text's out surround rectangle
	*				+_+: Default rectangle will be the screen
	*			"dockInRect" = Identified where to show in the rectangle
	*				(+_+: Use EFWDockInRect to fill in, default center)
	*			"beCut" = Whether cut the beyond parts. If choice not cut,
	*				the rectangle will resize to fit show the text
	*			"ignWrap" = Ignore wrap sign
	*			"textCol" = Text's color
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWTxtStt pvt_status;
};