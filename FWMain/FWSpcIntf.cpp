#pragma once
#include "stdafx.h"
#include "FWSpcIntf.h"
#include "FWSpace.h"
#include "FWDevIntf.h"
#include "FWFactory.h"

CFWSpcIntf::CFWSpcIntf( CFWObject* pSpcI )
{
	if(pSpcI->chkIsClass(CFWSpace::getTypID()))
	{
		ptd_pBindObj = pSpcI;
		pvt_status = SPC_STT_CREATED;
	}
	else
	{
		ptd_pBindObj = NULL;
		pvt_status = SPC_STT_UNKNOWN;
	}
}

bool CFWSpcIntf::TransferTo( DWORD sttToI, CFWParam& pamIO)
{
	// Check The Binding Camera Available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return false; }

	// According Current Status, Do Callback Functions
	switch (pvt_status)
	{
	case SPC_STT_CREATED:
		if(sttToI == SPC_STT_INSTABI)
		{
			if(!pSpc->chkIsClass(CFWRigid::getTypID()))	{ return false; }
			if(((CFWRigid*)pSpc)->isLocked()
			|| ((CFWRigid*)pSpc)->isBalance())			{ return false; }

			// Start adjust thread(input bind space)
			HANDLE hThread = NULL;
			hThread = ::CreateThread( 0, 0,
				(LPTHREAD_START_ROUTINE)procAdjSelf, this, 0, NULL);
			::CloseHandle(hThread);
			pvt_status = (EFWSpcStt)sttToI;
			return true;
		}
		break;
	case SPC_STT_INSTABI:
		if(sttToI == SPC_STT_CREATED)
		{
			// @_@: If forces' balance, how to clear speed
			if(pSpc->chkIsClass(CFWRigid::getTypID()))
			{
				((CFWRigid*)pSpc)->setSpeed(CRay());
			}
			pvt_status = (EFWSpcStt)sttToI;
			return true;
		}
		break;
	}

	return false;
}

CMatrix CFWSpcIntf::GetWldTrfMat(CMatrix matI) const
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return CMatrix(); }
	return pSpc->getWldTsfMat().dotRight(matI);
}

void CFWSpcIntf::SetCoordSys(OPMath::CCoord codI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->setCoordSys(codI);
}

void CFWSpcIntf::SetLocation(OPMath::CPoint posI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->setLocation(posI);
}

CPoint CFWSpcIntf::GetLocation() const
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return CPoint(); }
	return pSpc->getLocation();
}

CFWParam CFWSpcIntf::GenCrteInfo() const
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return CFWParam(); }
	return pSpc->genCrteInfo();
}

void CFWSpcIntf::MoveTo(CPoint posI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->movTo(posI);
}

void CFWSpcIntf::MoveBy(CVector dirI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->movTo(pSpc->getLocation() + dirI);
}

void CFWSpcIntf::RollBy( float fAglI, CRay rayAxsI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->rotBy( fAglI, rayAxsI);
}

void CFWSpcIntf::RollByX(float fAglI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->rotBy( fAglI, pSpc->getAxisX());
}

void CFWSpcIntf::RollByY(float fAglI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->rotBy( fAglI, pSpc->getAxisY());
}

void CFWSpcIntf::RollByZ(float fAglI)
{
	// Check the binding space available
	CFWSpace* pSpc = (CFWSpace*)ptd_pBindObj;
	if(pSpc == NULL)	{ return; }
	pSpc->rotBy( fAglI, pSpc->getAxisZ());
}

bool CFWSpcIntf::IsLocked() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return false;
	}
	return pSpc->isLocked();
}

float CFWSpcIntf::GetMass() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return 0;
	}
	return pSpc->getMass();
}

CRay CFWSpcIntf::GetSpeed() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return CVector();
	}
	return pSpc->getSpeed();
}

CBoxAABB CFWSpcIntf::GetOutSrdAABBBox() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return CBoxAABB();
	}
	return pSpc->getOutSrdAABBBox();
}

CSphere CFWSpcIntf::GetOutSrdSphere() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return CSphere();
	}
	return pSpc->getOutSrdSphere();
}

void CFWSpcIntf::AddForce(SFWForce fcI)
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return;
	}
	pSpc->addForce(fcI);

	// Check resultant of forces' balance
	SFWForce fcRes;
	std::vector<SFWForce> ayAllFc = pSpc->getAllForce();
	for( UINT i = 0; i < ayAllFc.size(); ++i)
	{
		fcRes += ayAllFc[i];
	}
	fcRes.regulate();
	CRay dirAccel = fcRes.getIndVec();
	dirAccel.setPoiBeg(pSpc->getLocation());
	dirAccel = dirAccel * fcRes.getVecLen();
	if(pSpc->getMass() != 0)
	{
		dirAccel = dirAccel / pSpc->getMass();
		pSpc->setAccel(dirAccel);
	}
	else
	{
		pSpc->setAccel(CRay());
	}

	// Resultant of force isn't zero vector
	TransferTo((!fcRes.isZeroVec()) ? SPC_STT_INSTABI : SPC_STT_CREATED);
}

void CFWSpcIntf::TurnToForceBalance()
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return;
	}
	AddForce((SFWForce&)pSpc->getResForce().getNegVec());
}

SFWForce CFWSpcIntf::CalResForce() const
{
	// Check the binding space available
	CFWRigid* pSpc = (CFWRigid*)ptd_pBindObj;
	if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
	{
		return SFWForce();
	}
	return pSpc->getResForce();
}

DWORD CFWSpcIntf::procAdjSelf(LPVOID pPamI)
{
	CFWSpcIntf* pIntf = (CFWSpcIntf*)pPamI;
	if(pIntf == NULL)	{ return -1; }

	while(pIntf->GetStatus() == SPC_STT_INSTABI)
	{
		// Check the binding space available
		CFWRigid* pSpc = (CFWRigid*)pIntf->ptd_pBindObj;
		if(pSpc == NULL || !pSpc->chkIsClass(CFWRigid::getTypID()))
		{
			return -1;
		}

		// Update self with accelerate and speed
		if(FAILED(pSpc->update(CFWParam())))	{ return -1; }

		// Check new space object whether collide in the space
		CFWSpcFcty::getInstance()->checkCollide(pIntf);

		// Sleep for while
		::Sleep(100);
	}
	return 0;
}