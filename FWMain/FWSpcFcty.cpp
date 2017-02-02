#pragma once
#include "stdafx.h"
#include "FWFactory.h"
#include "FWSpace.h"
#include "FWSpcIntf.h"
#include "FWPool.h"
#include "FWNode.h"
#include "../OPMath/Math.h"

SINGLE_IMPLEMENT(CFWSpcFcty)

CFWIntf& CFWSpcFcty::Create(CFWParam& pamIO)
{
	// Create a new space according user's required
	CFWSpace* pObjNew = NULL;
	if(pamIO.has("lock") || pamIO.has("mass"))
	{
		pObjNew = new CFWRigid();
		CFWRigid* pObjTmp = (CFWRigid*)pObjNew;
		pObjTmp->ptd_bLocked = pamIO.has("lock") ? pamIO.getBool("lock") : false;
		pObjTmp->ptd_fMass = pamIO.has("mass") ? pamIO.getFloat("mass") : 0;
		if(pamIO.has("outSrdBox"))
		{
			pObjTmp->ptd_boxOL = pamIO.getAABBBox("outSrdBox");
		}
		if(pamIO.has("outSrdSph"))
		{
			SFWQuate qatSph = pamIO.getSphere("outSrdSph");
			pObjTmp->ptd_sphOL = CSphere( qatSph, qatSph.m_fH);
		}
	}
	else
	{
		pObjNew = new CFWSpace();
	}
	if(pamIO.has("spcPos"))
	{
		pObjNew->ptd_codLoc = CCoord(pamIO.getQuate("spcPos"));
	}
	else if(pamIO.has("spcLoc"))
	{
		pObjNew->ptd_codLoc = pamIO.getCoordinate("spcLoc");
	}
	if(pamIO.has("speed") && pObjNew->chkIsClass(CFWRigid::getTypID()))
	{
		((CFWRigid*)pObjNew)->ptd_dirSpeed = CRay(pamIO.getQuate("speed"));
		((CFWRigid*)pObjNew)->ptd_dirSpeed.setPoiBeg(pObjNew->ptd_codLoc.m_poiCenter);
	}

	// Add new skin into pool
	DWORD dwHashID = CFWPool::getInstance()->add(pObjNew);

	// Set new skin to a skin interface, and insert into the map
	CFWIntf* pIntfTmp = new CFWSpcIntf(pObjNew);
	ptd_mpIntfs.insert(VtpIntf( dwHashID, pIntfTmp));

	// Add gravity if needs
	if(pObjNew->chkIsClass(CFWRigid::getTypID())
	&& !(pamIO.has("addGravity") && !pamIO.getBool("addGravity")))
	{
		((CFWSpcIntf*)pIntfTmp)->AddForce(getGravity());
	}

	// Return the reference of the skin interface
	return *pIntfTmp;
}

CFWIntf& CFWSpcFcty::Copy(DWORD dwSpcIdI)
{
	CFWIntf* pIntf = GetIntfByID(dwSpcIdI);
	if(pIntf == NULL || !pIntf->ChkIsClass(CFWSpace::getTypID()))
	{
		throw std::runtime_error("Error type: Given ID isn't a space");
	}
	return Create(((CFWSpcIntf*)pIntf)->GenCrteInfo());
}

bool CFWSpcFcty::checkCollide(CFWIntf* pSpcI)
{
	typedef std::pair<float, float> PrCmp;

	if(pSpcI == NULL || !pSpcI->ChkIsClass(CFWSpace::getTypID()))
	{
		return false;
	}
	CFWSpcIntf* pIptSpc = (CFWSpcIntf*)pSpcI;
	CVector dirSpeed = pIptSpc->GetSpeed();
	CVector dirSpdNg(dirSpeed.getIndVec().getNegVec());

	CBoxAABB boxIpt = pIptSpc->GetOutSrdAABBBox();
	CSphere sphIpt = pIptSpc->GetOutSrdSphere();
	CPoint poiIptCn(pIptSpc->GetLocation());
	float fIptRadius = sphIpt.getRadius();
	// @_@: can't process multi objects' colliding event
	for( IterIntf iter = ptd_mpIntfs.begin(); iter != ptd_mpIntfs.end(); ++iter)
	{
		// Same to input the space object, don't need compare and continue
		if(iter->second->GetObjID() == pSpcI->GetObjID())	{ continue; }
		CFWSpcIntf* pTmpSpc = (CFWSpcIntf*)iter->second;
		if(!pTmpSpc->ChkIsClass(CFWRigid::getTypID()))	{ continue; }

		CBoxAABB boxTmp = pTmpSpc->GetOutSrdAABBBox();
		CSphere sphTmp = pTmpSpc->GetOutSrdSphere();
		CPoint poiTmpCn(pTmpSpc->GetLocation());
		
		//float fDis = CVector( poiIptCn, poiTmpCn).getVecLen();
		//float fCttDis = fIptRadius + sphTmp.getRadius();
		//if(fDis < fCttDis)
		//{
		//	// @_@: For now, just test out surround sphere
		//	pIptSpc->MoveBy(dirSpdNg * (fCttDis - fDis));
		//	
		//	// @_@ This step should  do during the sub collide test
		//	// If collided object is locked object(like terrain)
		//	if(pTmpSpc->GetMass() == 0)
		//	{
		//		// Generate the resultant force's negative force
		//		// @_@ Should generate the force depend on the contact surface
		//		pIptSpc->TurnToForceBalance();
		//	}
		//	return true;
		//}
		//else
		{
			CPoint poiIptLTF = boxIpt.getPoiLTF();
			CPoint poiIptRBB = boxIpt.getPoiRBB();
			CPoint poiTmpLTF = boxTmp.getPoiLTF();
			CPoint poiTmpRBB = boxTmp.getPoiRBB();
			bool bCollided = true;
			// @_@: For now, just test out surround box
			// @_@ This step should  do during the sub collide test
			// If collided object is locked object(like terrain)
			if(pTmpSpc->IsLocked()
			&& (between<float>( poiIptLTF.m_fX, poiTmpLTF.m_fX, poiTmpRBB.m_fX, true)
			 || between<float>( poiIptRBB.m_fX, poiTmpLTF.m_fX, poiTmpRBB.m_fX, true)
			 || between<float>( poiTmpLTF.m_fX, poiIptLTF.m_fX, poiIptRBB.m_fX, true)
			 || between<float>( poiTmpRBB.m_fX, poiIptLTF.m_fX, poiIptRBB.m_fX, true))
			&& (between<float>( poiIptLTF.m_fY, poiTmpLTF.m_fY, poiTmpRBB.m_fY, true)
			 || between<float>( poiIptRBB.m_fY, poiTmpLTF.m_fY, poiTmpRBB.m_fY, true)
			 || between<float>( poiTmpLTF.m_fY, poiIptLTF.m_fY, poiIptRBB.m_fY, true)
			 || between<float>( poiTmpRBB.m_fY, poiIptLTF.m_fY, poiIptRBB.m_fY, true))
			&& (between<float>( poiIptLTF.m_fZ, poiTmpLTF.m_fZ, poiTmpRBB.m_fZ, true)
			 || between<float>( poiIptRBB.m_fZ, poiTmpLTF.m_fZ, poiTmpRBB.m_fZ, true)
			 || between<float>( poiTmpLTF.m_fZ, poiIptLTF.m_fZ, poiIptRBB.m_fZ, true)
			 || between<float>( poiTmpRBB.m_fZ, poiIptLTF.m_fZ, poiIptRBB.m_fZ, true)))
			{
				// Generate the resultant force's negative force
				// @_@ Should generate the force depend on the contact surface
				pIptSpc->TurnToForceBalance();

				// @_@: Only for terrain's collide test
				float fIptBtmY = (poiIptLTF.m_fY < poiIptRBB.m_fY) ? poiIptLTF.m_fY : poiIptRBB.m_fY;
				float fTmpTopY = (poiTmpLTF.m_fY > poiTmpRBB.m_fY) ? poiTmpLTF.m_fY : poiTmpRBB.m_fY;
				pIptSpc->MoveBy(dirSpdNg * abs(fTmpTopY - fIptBtmY));

				return true;
			}
		}
	}
	return false;
}