#pragma once
#include "FWObject.h"
#include "../OPMath/Math.h"
using namespace OPMath;
#include "d3dx9.h"

class CFWCamera: public CFWObject
{
public:
	friend class CFWCamFcty;
protected:
	D3DXVECTOR3	ptd_posEye;
	D3DXVECTOR3 ptd_posTar;

	D3DXVECTOR3	ptd_dirLook;
	D3DXVECTOR3	ptd_dirRight;
	D3DXVECTOR3	ptd_dirUp;

	FLOAT ptd_fClsPln;
	FLOAT ptd_fFarPln;

	UINT ptd_uViewWid;
	UINT ptd_uViewHgt;
public:
	virtual void regulate();
	virtual void lookAt(CVector dirLookI);
	virtual CMatrix getProjMat();
	virtual CMatrix getViewMat();
	virtual CSqCone getViewCone();
	virtual FUNC_GET( CVT_POI_FW(ptd_posEye), EyeLoc, CPoint);
	virtual FUNC_GET( CVT_POI_FW(ptd_dirLook), LookDir, CVector);
	virtual FUNC_GET( CVT_POI_FW(ptd_dirRight), RightDir, CVector);
public: CHK_IS_CLASS;
};

class CFWCamFree: public CFWCamera
{
public: CFWCamFree(): ptd_bLocked(true)	{}
protected: bool ptd_bLocked;
protected: inline void updTgtLoc(CVector dirMvI);
public:
	virtual void rollByX(float fAngI);
	virtual void rollByY(float fAngI);
	virtual void rollByZ(float fAngI);
	virtual void moveByX(float fDisI);
	virtual void moveByY(float fDisI);
	virtual void moveByZ(float fDisI);
public: CHK_IS_CLASS;
};