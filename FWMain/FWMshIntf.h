#pragma once
#include "FWMacro.h"
#include "FWIntf.h"
#include "FWMisc.h"
#include "../OPMath/Math.h"

class FWMAIN_API CFWMshIntf: public CFWIntf
{
public:
	CFWMshIntf():
		pvt_status(MSH_STT_UNKNOWN)	{}
	CFWMshIntf(CFWObject* pMshI);
public:
	// Primitives type
	enum EFWPmTyp
	{
		PRIM_TP_POINT,
		PRIM_TP_LINE,
		PRIM_TP_LN_LST,
		PRIN_TP_TRIANGLE
	};

	enum EFWMshStt
	{
		MSH_STT_UNKNOWN,
		MSH_STT_CREATED,
		MSH_STT_DESTROYED,
		MSH_STT_INITIALIZED,
		MSH_STT_DRAWING
	};
	/*
	*	MSH_STT_CREATED -> MSH_STT_INITIALIZED cause to call initialize()
	*		Required parameter: [N]"aryPois" = Pointer of quaternion array
	*							[N]"aryIdxs" = Pointer of indexes' array
	*							"subset" + subID = Identified subset range and ID
	*								(+_+: Store range pair by format "faceBeg-faceEnd")
	*							"texUV" + texID = Texture UV map, first U, second V
	*							"aryNmls" = Pointer of normal vectors' array
	*							"genNmls" = Use mesh auto normals instead of givens
	*							"weldVtx" = If vertexes are in same location, weld them together
	*							"optimize" = Optimize mesh, if needed
	*	MSH_STT_INITIALIZED -> MSH_STT_DRAWING cause to call draw()
	*		Required parameter: "subsetID" = Subset ID
	*	MSH_STT_INITIALIZED -> MSH_STT_DESTROYED cause to call destroy()
	*	MSH_STT_DESTROYED -> MSH_STT_INITIALIZED cause to call initialize()
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWMshStt pvt_status;
public:
	HRESULT Draw(UINT);
	bool HasSubset(UINT);
	CFWAABBBox GeneOutSrdBox() const;
	SFWQuate GeneOutSrdSph() const;
};