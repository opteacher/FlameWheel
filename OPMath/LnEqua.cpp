#pragma once
#include "stdafx.h"
#include "LnEqua.h"
#include "Math.h"

using namespace OPMath;

void ILnEqua::setFunType(int iFunTpI)
{
	if(::between<int>( iFunTpI, LESS, BIGGER, true))
	{
		m_iTypeFun = iFunTpI;
	}
}
void ILnEqua::enableEquals(bool bEnableI)
{
	m_bEnbEquals = bEnableI;
}