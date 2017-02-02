#pragma once
#include "FWMacro.h"
#include "FWMisc.h"
#include "FWSpcIntf.h"
#include "../OPMath/Math.h"
using OPMath::CMatrix;
#include <map>
#include <set>
using std::string;

class FWMAIN_API CFWNode
{
protected:
	static std::set<string> s_stUsedNams;
public:
	CFWNode(string = "");
	virtual ~CFWNode()	{ s_stUsedNams.erase(ptd_strName); }

public:
	/*
	*	Required parameters:
	*		"matFather"=Father transform matrix
	*		"dispSetg" = Whether render this model and it's subset
	*/
	virtual HRESULT render( CMatrix, DWORD = MAIN_OBJ);
	// Generate world transform matrix as the second parameter
	virtual HRESULT update( CMatrix, CMatrix&);
protected:
	virtual HRESULT renderAux( CMatrix, DWORD);
	virtual HRESULT renderSelf(CMatrix)	{ return S_OK; }
	virtual HRESULT preRender();
	virtual HRESULT sufRender();

protected: string ptd_strName;
public:
	inline bool resetNam(string strNamI);
	FUNC_GET( ptd_strName, Nam, string);

protected: CFWSpcIntf ptd_spcLoc;
public:
	virtual void resetSpc(DWORD);
	virtual void setCoordSys(OPMath::CCoord);
	virtual void setLocation(OPMath::CPoint);

protected: DWORD ptd_dwObjID;
public:
	virtual string setIncObj(DWORD dwIdI);
	FUNC_IS( (ptd_dwObjID != 0), BindObj);

public:
	enum EFWAuxObj
	{
		AUX_UNKNOWN = 0x0000,
		MAIN_OBJ	= 0x0001,
		AUX_COD_WLD	= 0x0002,
		AUX_LGT_LOC	= 0x0004,
		AUX_GRID	= 0x0008,
		AUX_UNUSED	= 0xFFFF,
		ALL_AUX_OBJ	= MAIN_OBJ
					| AUX_COD_WLD
					| AUX_LGT_LOC
					| AUX_GRID
	};
	typedef std::map<EFWAuxObj, CFWNode*> MapAuxObj;
	typedef MapAuxObj::iterator ItrAuxObj;
	typedef MapAuxObj::const_iterator CtrAuxObj;
	typedef MapAuxObj::value_type VtpAuxObj;
protected:
	EFWAuxObj ptd_tpAux;
	MapAuxObj ptd_mpAuxObjs;
public:
	FUNC_IS( (ptd_tpAux != 0), AuxObj);
	FUNC_GS( ptd_tpAux, AuxTyp, EFWAuxObj);
	FUNC_GET( ptd_mpAuxObjs, AuxObjs, MapAuxObj);
private:
	void bindIncObj( DWORD dwIdI, string strObjTpI);
	virtual bool chkIsLgtAndAdd(DWORD dwIdI);
	virtual bool chkIsMshAndAdd(DWORD dwIdI);
	virtual bool chkIsCamAndAdd(DWORD dwIdI);
	virtual void genWldCoord(float fAxisI = 0);

protected:
	typedef std::map<string, CFWNode*> MapSubset;
	typedef std::map<string, CFWNode*>::iterator ItrSub;
	typedef std::map<string, CFWNode*>::value_type VtpSub;
	typedef std::map<string, CFWNode*>::const_iterator CiterSub;
protected: MapSubset ptd_mpSubset;
public:
	virtual void addSubNode(CFWNode* pNodeI);
	//+_+: Just cut the relationship between sub,
	//		be cut sub node still existed
	virtual bool rmvSubNode(string strNamI);
	virtual bool rmvSubNode(CFWNode* pNodeI);
	virtual void clsSubNode();
	virtual CFWNode* findNodeFromSub(string strNamI);
	virtual FUNC_GET( ptd_mpSubset, Subset, MapSubset);

protected:
	typedef std::map<UINT, CFWParam> MapParam;
	typedef MapParam::iterator IterPam;
	typedef MapParam::value_type VtpPam;
protected: MapParam ptd_mpEffect;
public:
	// +_+: Cant add effect during create, 
	// and enable/disable include in the same parameter
	virtual void AddEffect( UINT uEffIdI, CFWParam pamI = CFWParam());
	
public: virtual CFWNode* Clone(CFWNode* = NULL) const;
public: CHK_IS_CLASS_BS;
};

#define REVIEW_SUBSET_MAP	for( ItrSub iter = ptd_mpSubset.begin(); iter != ptd_mpSubset.end(); ++iter)
