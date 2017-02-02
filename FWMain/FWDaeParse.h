#pragma once
#include "FWParam.h"
#include <vector>
#include <string>
using std::string;

class CFWDaeParse
{
public:
	typedef std::vector<CFWParam> AryPam;
	typedef std::pair<string, AryPam> PrNamPam;
public: PrNamPam expMdlFile(string strFileI);
};