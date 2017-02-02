#pragma once
#include "FWMacro.h"
#include "FWParam.h"
#include "FWObject.h"
#include <set>

class FWMAIN_API CFWIntf
{
public:
	CFWIntf(CFWObject* pObjI = NULL):
		ptd_pBindObj(pObjI),
		ptd_pNumRef((!pObjI) ? new DWORD(1) : NULL)	{}
	CFWIntf(const CFWIntf&);
	virtual ~CFWIntf()	{ Release(); }
	CFWIntf& operator=(const CFWIntf&);
	void Release();
public:
	virtual bool TransferTo( DWORD, CFWParam& = CFWParam()) = 0;
	virtual UINT GetStatus() const = 0;
protected:
	CFWObject* ptd_pBindObj;
	DWORD* ptd_pNumRef;
public:
	virtual bool BeBound() const	{ return ptd_pBindObj; }
	virtual FUNC_GET_I( ptd_pBindObj->getObjID(), ObjID, DWORD);
	virtual FUNC_GET_I( typeid(*ptd_pBindObj).name(), Type, string);
	inline bool ChkIsClass(DWORD) const;
private:
	// When do the operator= process, the target 
	// interface may not be ready for initializing.
	// That means target's construct function haven't
	// been called, and the pointer of bind object is
	// be set as unknown value. it can't for test with NULL
	// This why the static parts of interface will collect 
	// all used addresses and check the target interface 
	// before operator=
	static std::set<DWORD> s_stUsedAddr;
public:
	static bool chkAddrExs(DWORD);
	static void addAddr(DWORD);
	static void rmvAddr(DWORD);
};