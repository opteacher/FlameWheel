#pragma once
#include "stdafx.h"
#include "FWPool.h"
#include "../OPMath/Math.h"
#include "../OPTools/String.h"
#include <iterator>
using std::iterator;

SINGLE_IMPLEMENT(CFWPool)

typedef std::map<DWORD, CFWObject*> MapObjs;

DWORD CFWPool::add( CFWObject* pObjI, DWORD dwHashIdI)
{
	if(dwHashIdI == 0)
	{
		string strAddr = OPTools::numToStr<DWORD>((DWORD)pObjI);
		dwHashIdI = OPMath::geneHashVal(strAddr.c_str());
	}
	ptd_mpData.insert(MapObjs::value_type( dwHashIdI, pObjI));
	// Set ID to object self
	pObjI->pvt_dwHashID = dwHashIdI;
	return dwHashIdI;
}

CFWObject* CFWPool::get(DWORD dwHashIdI)
{
	MapObjs::iterator iterFnd = ptd_mpData.find(dwHashIdI);
	if(iterFnd != ptd_mpData.end())	{ return iterFnd->second; }
	else							{ return NULL; }
}

void CFWPool::rmv(CFWObject* pObjI)
{
	for( MapObjs::iterator iter = ptd_mpData.begin(); \
		iter != ptd_mpData.end(); ++iter)
	{
		if(iter->second == pObjI)
		{
			ptd_mpData.erase(iter);
		}
	}
}

void CFWPool::rmv(DWORD dwHashIdI)
{
	MapObjs::iterator iterFnd = ptd_mpData.find(dwHashIdI);
	if(iterFnd != ptd_mpData.end())
	{
		ptd_mpData.erase(iterFnd);
	}
}
