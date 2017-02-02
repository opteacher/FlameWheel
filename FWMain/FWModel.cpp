#pragma once
#include "stdafx.h"
#include "FWModel.h"
#include "FWFactory.h"
#include "FWDevIntf.h"
#include "FWEffIntf.h"
#include "FWManager.h"
#include "../OPMath/Math.h"
using OPMath::CMatrix;

INI_TYPID(CFWModel);

HRESULT CFWModel::renderSelf(CMatrix matI)
{
	//FUNC_BEG;
	// Get and check current device whether existed and status set in the PAINTING
	CFWIntf* pIntf = CFWDevFcty::getInstance()->getCurDev();
	if(!pIntf || pIntf->GetStatus() != CFWDevIntf::DEV_STT_PAINTING)
	{
		return E_FAIL;
	}
	CFWDevIntf* pCurDev = (CFWDevIntf*)pIntf;

	// Transform to world space
	pCurDev->SetWldMat(matI);

	// Draw mesh
	CFWMshFcty* pMshMgr = CFWMshFcty::getInstance();
	CFWSknFcty* pSknMgr = CFWSknFcty::getInstance();
	if(ptd_mshIntf.BeBound())
	{
		// Set this mesh to current drawing mesh
		if(pMshMgr->getDrwMshID() != ptd_mshIntf.GetObjID())
		{
			pMshMgr->setDrwMshID(ptd_mshIntf.GetObjID());
		}

		for( ItrSknIntf iter = ptd_mpSknIntf.begin(); iter != ptd_mpSknIntf.end(); ++iter)
		{
			CFWSknIntf sknIntf = iter->second;
			// Active skin effect(+_+: If there no mesh, it's no useful to active skin)
			if(sknIntf.BeBound() && sknIntf.GetObjID() != pSknMgr->getActSknID())
			{
				if(!sknIntf.TransferTo(CFWSknIntf::SKN_STT_ACTIVED))	{ return E_FAIL; }
			}

			if(FAILED(this->preRender()))	{ return E_FAIL; }
			if(FAILED(ptd_mshIntf.Draw(iter->first)))	{ return E_FAIL; }
			if(FAILED(this->sufRender()))	{ return E_FAIL; }

			// Reset active skin
			if(!sknIntf.TransferTo(CFWSknIntf::SKN_STT_CREATED))	{ return E_FAIL; }
		}
	}

	// Reset world transform matrix
	pCurDev->ResetWldMat();
	//FUNC_END;
	return S_OK;
}

CFWNode* CFWModel::Clone(CFWNode* pNodeI) const
{
	if(pNodeI == NULL)	{ pNodeI = new CFWModel(); }
	((CFWModel*)pNodeI)->ptd_mshIntf = ptd_mshIntf;
	((CFWModel*)pNodeI)->ptd_mpSknIntf = ptd_mpSknIntf;

	return __super::Clone(pNodeI);
}