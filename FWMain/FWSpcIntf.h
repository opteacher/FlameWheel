#pragma once
#include "FWIntf.h"
#include "../OPMath/Math.h"
using namespace OPMath;

class FWMAIN_API CFWSpcIntf: public CFWIntf
{
public:
	CFWSpcIntf():
		pvt_status(SPC_STT_UNKNOWN)	{}
	CFWSpcIntf(CFWObject* pSpcI);
public:
	enum EFWSpcStt
	{
		SPC_STT_UNKNOWN,
		SPC_STT_CREATED,
		SPC_STT_INSTABI
	};

	/*
	*	Transfer map:
	*		SPC_STT_CREATED -> SPC_STT_UPDATE cause update()
	*				Required parameters:	"matFather" = Father transform matrix
	*											If the parameter has not this "matFather"
	*											the function will add self transfer matrix.
	*											Or update self into this "matFather"
	*		SPC_STT_CREATED -> SPC_STT_SET_WLD cause Device->SetWldMat()
	*				Of course, this transform will GetWldMat() first for adding self transfer
	*				matrix. Also, you can get this space transform matrix by GetTrfMat(), and
	*				SetWldMat by self
	*		+_+: Status wont maintain at SPC_STT_UPDATE/SPC_STT_SET_WLD, after process the
	*			status will back to SPC_STT_CREATED
	*		
	*/
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam());
	virtual FUNC_GET_I( pvt_status, Status, UINT);
private:
	EFWSpcStt pvt_status;
public:
	virtual CMatrix GetWldTrfMat(CMatrix = CMatrix()) const;
	virtual void SetCoordSys(CCoord);
	virtual void SetLocation(CPoint);
	virtual CPoint GetLocation() const;
	virtual CFWParam GenCrteInfo() const;
	virtual void MoveTo(CPoint);
	virtual void MoveBy(CVector);
	virtual void RollBy( float, CRay);
	virtual void RollByX(float);
	virtual void RollByY(float);
	virtual void RollByZ(float);
public:// Following function can only use for rigid
	virtual bool IsLocked() const;
	virtual float GetMass() const;
	virtual CRay GetSpeed() const;
	virtual CBoxAABB GetOutSrdAABBBox() const;
	virtual CSphere GetOutSrdSphere() const;
	virtual void AddForce(SFWForce);
	// Add the resultant force's negative force
	virtual void TurnToForceBalance();
	virtual SFWForce CalResForce() const;
private:
	static DWORD procAdjSelf(LPVOID);
};