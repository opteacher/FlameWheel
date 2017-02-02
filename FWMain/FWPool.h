#pragma once
#include "FWMacro.h"
#include "FWObject.h"
#include <map>

class CFWPool
{
	SINGLE_DECLARE(CFWPool)
protected:
	std::map<DWORD, CFWObject*> ptd_mpData;
public:
	DWORD add( CFWObject*, DWORD = 0);
	CFWObject* get(DWORD);
	void rmv(CFWObject*);
	void rmv(DWORD);
};