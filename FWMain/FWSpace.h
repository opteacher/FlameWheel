#pragma once
#include "FWObject.h"
#include "FWParam.h"
#include "../OPMath/Math.h"
using namespace OPMath;

class CFWSpace: public CFWObject
{
public: friend class CFWSpcFcty;
public: virtual HRESULT update(CFWParam)	{ return S_OK; }
protected: CCoord ptd_codLoc;
public:
	virtual FUNC_GS( ptd_codLoc.m_poiCenter, Location, CPoint);
	virtual FUNC_SET( ptd_codLoc, CoordSys, CCoord);
	virtual FUNC_GET( CRay( ptd_codLoc.m_poiCenter, ptd_codLoc.m_vecAxis[AXIS_X]), AxisX, CRay);
	virtual FUNC_GET( CRay( ptd_codLoc.m_poiCenter, ptd_codLoc.m_vecAxis[AXIS_Y]), AxisY, CRay);
	virtual FUNC_GET( CRay( ptd_codLoc.m_poiCenter, ptd_codLoc.m_vecAxis[AXIS_Z]), AxisZ, CRay);
	virtual FUNC_GET( ptd_codLoc.transformToWld(), WldTsfMat, CMatrix);
public:
	virtual void movTo(CPoint);
	virtual void rotBy( float, CRay);
public: virtual CFWParam genCrteInfo() const;
public: CHK_IS_CLASS;
};

class CFWRigid: public CFWSpace
{
public: friend class CFWSpcFcty;
public: CFWRigid(): ptd_bLocked(false), ptd_fMass(0)	{}
protected:
	bool ptd_bLocked;
	float ptd_fMass;
	CBoxAABB ptd_boxOL;
	CSphere ptd_sphOL;
	CRay ptd_dirAccel;
	CRay ptd_dirSpeed;
public:
	FUNC_IS( ptd_bLocked, Locked);
	FUNC_GET( ptd_fMass, Mass, float);
	FUNC_GS( ptd_dirAccel, Accel, CRay);
	FUNC_GS( ptd_dirSpeed, Speed, CRay);
	FUNC_GET( ((CVector&)ptd_dirAccel*ptd_fMass), ResForce, CRay);
	CBoxAABB getOutSrdAABBBox() const;
	CSphere getOutSrdSphere() const;
	bool isBalance() const	{ return ptd_dirAccel.isZeroVec(); }
public: virtual HRESULT update(CFWParam);
protected: std::vector<SFWForce> ptd_ayExForce;
public:
	void addForce(SFWForce fcI)
	{
		if(!ptd_bLocked)	{ ptd_ayExForce.push_back(fcI); }
	}
	std::vector<SFWForce> getAllForce()	{ return ptd_ayExForce; }
public: CHK_IS_CLASS;
};