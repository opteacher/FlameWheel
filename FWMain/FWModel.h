#pragma once
#include "FWNode.h"
#include "../OPTools/String.h"
using OPTools::numToStr;
#include "FWMshIntf.h"
#include "FWSknIntf.h"
#include "FWParam.h"
#include "FWObject.h"
#include <map>

class FWMAIN_API CFWModel: public CFWNode
{
public: friend class CFWMdlMngr;
protected:
	typedef std::map<UINT, CFWParam> MapParam;
	typedef MapParam::iterator IterPam;
	typedef MapParam::value_type VtpPam;
public:
	typedef std::map<UINT, CFWSknIntf> MapSknIntf;
	typedef MapSknIntf::iterator ItrSknIntf;
	typedef MapSknIntf::value_type VtpSknIntf;
public:
	CFWModel(): CFWNode("Model" + numToStr<DWORD>(s_stUsedNams.size()))	{}
	CFWModel(string strNamI): CFWNode(strNamI)	{}
protected:
	CFWMshIntf ptd_mshIntf;
	MapSknIntf ptd_mpSknIntf;
public:
	FUNC_GET_I( ptd_mshIntf, Mesh, CFWMshIntf);
public:
	/*
	*	Required parameters:
	*		"matFather" = Before transfer self, do father's transfer first
	*/
	virtual HRESULT renderSelf(CMatrix);

public: virtual CFWNode* Clone(CFWNode* = NULL) const;
public: CHK_IS_CLASS;
};