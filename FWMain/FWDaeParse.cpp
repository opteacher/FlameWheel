#pragma once
#include "stdafx.h"
#include "FWDaeParse.h"
#include "../OPTools/File.h"
using OPTools::GetDataFromXml;

CFWDaeParse::PrNamPam CFWDaeParse::expMdlFile(string strFileI)
{
	TiXmlDocument docCfg(strFileI.c_str());
	bool bSuccedLoad = docCfg.LoadFile();
	if(bSuccedLoad)
	{
		// If Succeed load XML, get all data from it
		SXmlNode* pNode = OPTools::GetDataFromXml(&docCfg);
		if(!pNode)	{ return PrNamPam(); }
		
		
	}
	else
	{
		return PrNamPam();
	}
}