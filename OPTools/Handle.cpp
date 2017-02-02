#pragma once
#include "stdafx.h"
#include "Handle.h"

using namespace std;
using namespace OPTools;

#ifndef MAX_LOOP
#define MAX_LOOP	20000
#endif

#ifndef DO_UNTIL
#define DO_UNTIL( condition, unsucceed)	\
	UINT uTryInc = 0;\
	while(!(condition) && uTryInc < MAX_LOOP)	{ ++uTryInc; }\
	if(uTryInc == MAX_LOOP - 1)	{ unsucceed }
#endif

extern HANDLE getEvent()
{
	HANDLE hForRet = ::CreateEvent( NULL, FALSE, FALSE, NULL);
	::SetEvent(hForRet);	return hForRet;
}

extern HANDLE g_hEvent = getEvent();
extern set<DWORD> g_stInitHdl = set<DWORD>();

bool CHandle::chkHdlExist(DWORD dwAddrI)
{
	return CHK_ITER_EXS( g_stInitHdl, dwAddrI);
}

void CHandle::insInitHdl(DWORD dwAddrI)
{
	::WaitForSingleObject( g_hEvent, INFINITE);
	g_stInitHdl.insert(dwAddrI);
	::SetEvent(g_hEvent);
}

void CHandle::rmvInitHdl(DWORD dwAddrI)
{
	::WaitForSingleObject( g_hEvent, INFINITE);
	g_stInitHdl.erase(dwAddrI);
	::SetEvent(g_hEvent);
}

OPTools::CHandle::CHandle( const CHandle& rHdlI )
{
	this->release();
	if(!chkHdlExist((DWORD)&rHdlI))	{ return; }
	if(!rHdlI.m_pReal)	{ return; }

	m_pReal = rHdlI.m_pReal->clone();
	m_pUintUse = new UINT(1);
	m_bAutoCtl = rHdlI.m_bAutoCtl;
	insInitHdl((DWORD)this);
}

CHandle& OPTools::CHandle::operator=( CHandle& rHdlI )
{
	this->release();
	if(!chkHdlExist((DWORD)&rHdlI))	{ return *this; }

	m_pReal = rHdlI.m_pReal;
	m_pUintUse = rHdlI.m_pUintUse;
	m_bAutoCtl = rHdlI.m_bAutoCtl;
	if(m_pUintUse)	{ ++*m_pUintUse; }
	insInitHdl((DWORD)this);

	return *this;
}

bool OPTools::CHandle::operator==( const CHandle& rHdlI ) const
{
	return m_pReal == rHdlI.m_pReal
		&& m_pUintUse == rHdlI.m_pUintUse;
}

string OPTools::CHandle::getTypeNam() const
{
	if(!m_pReal)	{ return ""; }
	return m_pReal->type().name();
}

void OPTools::CHandle::release()
{
	if(!m_bAutoCtl)	{ return; }

	if(chkHdlExist((DWORD)this) && m_pUintUse && m_pReal)
	{
		rmvInitHdl((DWORD)this);
		if(--*m_pUintUse == 0)
		{
			SAFE_DELETE(m_pUintUse);
			SAFE_DELETE(m_pReal);
		}
		else
		{
			m_pUintUse = NULL;
			m_pReal = NULL;
		}
	}
}

OPTools::CHandle::~CHandle()
{
	release();
}

OPTools::CHdlEx::CHdlEx( const CHdlEx& rHdlI ) : CHandle(rHdlI)
{
	m_dwObjId	= rHdlI.m_dwObjId;
	m_strObjNam	= rHdlI.m_strObjNam;
	m_uOperId	= rHdlI.m_uOperId;
}

OPTools::CHdlEx::CHdlEx( const CHandle& rHdlI ) : CHandle((CHandle&)rHdlI)
{
	m_dwObjId	= 0;
	m_strObjNam	= "";
	m_uOperId	= 0;
}

CHdlEx& OPTools::CHdlEx::operator=( const CHdlEx& rHdlI )
{
	this->release();
	m_pReal = rHdlI.m_pReal;
	m_pUintUse = rHdlI.m_pUintUse;
	m_bAutoCtl = rHdlI.m_bAutoCtl;
	++*m_pUintUse;

	m_dwObjId	= rHdlI.m_dwObjId;
	m_strObjNam	= rHdlI.m_strObjNam;
	m_uOperId	= rHdlI.m_uOperId;
	return *this;
}
