#pragma once
#include "stdafx.h"
#include "FWIntf.h"

CFWIntf::CFWIntf( const CFWIntf& rIfI ) :
	ptd_pBindObj(rIfI.ptd_pBindObj),
	ptd_pNumRef(rIfI.ptd_pNumRef)
{
	if(ptd_pNumRef)	{ ++(*ptd_pNumRef); }
}

CFWIntf& CFWIntf::operator=(const CFWIntf& rIfI)
{
	ptd_pBindObj = rIfI.ptd_pBindObj;
	ptd_pNumRef = rIfI.ptd_pNumRef;
	if(ptd_pNumRef)	{ ++(*ptd_pNumRef); }
	return *this;
}

void CFWIntf::Release()
{
	if(ptd_pNumRef && *ptd_pNumRef > 1)
	{
		ptd_pBindObj = NULL;
		--*ptd_pNumRef;
	}
	else
	{
		SAFE_DELETE(ptd_pBindObj);
		SAFE_DELETE(ptd_pNumRef);
	}
}

bool CFWIntf::ChkIsClass( DWORD dwClsIdI ) const
{
	return ptd_pBindObj->chkIsClass(dwClsIdI);
}

std::set<DWORD> CFWIntf::s_stUsedAddr = std::set<DWORD>();

bool CFWIntf::chkAddrExs(DWORD dwAddrI)
{
	return s_stUsedAddr.find(dwAddrI) != s_stUsedAddr.end();
}

void CFWIntf::addAddr(DWORD dwAddrI)
{
	s_stUsedAddr.insert(dwAddrI);
}

void CFWIntf::rmvAddr(DWORD dwAddrI)
{
	s_stUsedAddr.erase(dwAddrI);
}