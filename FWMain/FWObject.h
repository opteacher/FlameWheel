#pragma once
#include <typeinfo>
#include <map>
#include "windows.h"
#include "FWMacro.h"

class FWMAIN_API CFWObject
{
public: friend class CFWPool;
private: DWORD pvt_dwHashID;
public: FUNC_GET( pvt_dwHashID, ObjID, DWORD);
public: CHK_IS_CLASS_BS;
};