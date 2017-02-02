#pragma once
#include "stdafx.h"
#include "Ray.h"
#include "Math.h"

using namespace OPMath;

CRay::CRay( CPoint poiBegI, CPoint poiTarI): CVector( poiBegI, poiTarI)
{
	m_poiBeg = poiBegI;
}

CRay::CRay( CPoint poiBegI, CVector vecDirI): CVector(vecDirI)
{
	m_poiBeg = poiBegI;
}

CRay::CRay(const CRay& refRayI): CVector(refRayI)
{
	m_poiBeg = refRayI.m_poiBeg;
}

CRay::CRay(const CVector& rVecI): CVector(rVecI)	{}

float CRay::disFromPoi(CPoint poiI)
{
	CVector vecDirLn(*this);
	CVector vecDirPoi( poiI, m_poiBeg);
	CVector vecDirLnToPoi(vecDirLn + vecDirPoi);
	if(!vecDirLnToPoi.isVertical(vecDirLn))	{
		vecDirLn = vecDirLn.getNegVec();
		vecDirLnToPoi = vecDirLn + vecDirPoi;
		if(!vecDirLnToPoi.isVertical(vecDirLn))	{
			throw std::invalid_argument("point on the line!");
		}
	}
	
	vecDirLn = vecDirLn.getNegVec();
	float fCos = vecDirLn.cosWithVec(vecDirPoi);
	if(fCos < 0)	{
		vecDirLn = vecDirLn.getNegVec();
		fCos = vecDirLn.cosWithVec(vecDirPoi);
	}
	float fDis = vecDirPoi.getVecLen();
	float fSin = (float)sqrt(1 - pow( fCos, 2));
	return(fDis*fSin);
}

CPoint CRay::poiProjOnLn(CPoint poiI)
{
	/*CVector vecDirPoi( m_poiBeg, poiI);
	CVector vecDirLn(*this);
	float fLenPoiToLn = vecDirPoi.getVecLen();
	if(!vecDirPoi.isSameDir_VerticalIn(vecDirLn))	{
		vecDirLn = getNegVec();
	}
	float fCos = vecDirLn.cosWithVec(vecDirPoi);
	float fLenOnLn = fLenPoiToLn*fCos;
	vecDirLn.normalize();
	CPoint poiRet = m_poiBeg + vecDirLn*fLenOnLn;
	return(poiRet);*/
	return CPlane( poiI, *this).getProjPoi(m_poiBeg);
}

bool CRay::intersectsTgl( CPoint poiAI, CPoint poiBI, CPoint poiCI)
{
	//1.check if not insert plane of the triangle
	CVector vecEdgeA(poiAI - poiCI);
	vecEdgeA.normalize();
	CVector vecEdgeB(poiBI - poiCI);
	vecEdgeB.normalize();
	
	this->normalize();
	CVector vecChk = this->cross(vecEdgeB);
	//if close to 0, ray is parallel
	float fDet = vecEdgeA.dot(vecChk).sun();
	if(abs(fDet) < MIN_VAL && abs(fDet) > -MIN_VAL)	{
		return false;
	}
	
	//distance to plane, <0 means ray behind the plane
	CVector vecTA(m_poiBeg - poiAI);
	vecTA.normalize();
	if(isSameDir_VerticalIn(vecTA))	{
		return false;
	}
	
	/*CVector vecTB = vecTA.cross(vecEdgeB);
	float fDirB = dot(vecTB).sun();
	if(fDirB < 0.0f || fDirB+fDirA > fDet)
		return false;*/
	
	//2.project the triangle to plane XY, follow the ray
	float fNumOffA = -poiAI.m_fZ/m_fZ;
	CPoint poiAProj(
			poiAI.m_fX + m_fX*fNumOffA,
			poiAI.m_fY + m_fY*fNumOffA,
			0.0f);
	float fNumOffB = -poiBI.m_fZ/m_fZ;
	CPoint poiBProj(
			poiBI.m_fX + m_fX*fNumOffB,
			poiBI.m_fY + m_fY*fNumOffB,
			0.0f);
	float fNumOffC = -poiCI.m_fZ/m_fZ;
	CPoint poiCProj(
			poiCI.m_fX + m_fX*fNumOffC,
			poiCI.m_fY + m_fY*fNumOffC,
			0.0f);
	//3.check the three points are locate on a line
	CVector vecTmpA(poiAProj - poiCProj);
	CVector vecTmpB(poiBProj - poiCProj);
	float fNumOffRay = 0.0f;
	CPoint poiRayProj;
	if(vecTmpA.isParallel(vecTmpB) || 0 == m_fZ)	{
		//three points in a same line
		//choise another plane (XZ) for projecting
		fNumOffA = -poiAI.m_fY/m_fY;
		poiAProj = CPoint(
				poiAI.m_fX + m_fX*fNumOffA,
				poiAI.m_fZ + m_fZ*fNumOffA,
				0.0f);
		fNumOffB = -poiBI.m_fY/m_fY;
		poiBProj = CPoint(
				poiBI.m_fX + m_fX*fNumOffB,
				poiBI.m_fZ + m_fZ*fNumOffB,
				0.0f);
		fNumOffC = -poiCI.m_fY/m_fY;
		poiCProj = CPoint(
				poiCI.m_fX + m_fX*fNumOffC,
				poiCI.m_fZ + m_fZ*fNumOffC,
				0.0f);
		//check the three points are locate on a line
		vecTmpA = poiAProj - poiCProj;
		vecTmpB = poiBProj - poiCProj;
		if(vecTmpA.isParallel(vecTmpB) || 0 == m_fY)	{
			//three points in a same line
			//choise the last plane (YZ) for projecting
			fNumOffA = -poiAI.m_fX/m_fX;
			poiAProj = CPoint(
					poiAI.m_fY + m_fY*fNumOffA,
					poiAI.m_fZ + m_fZ*fNumOffA,
					0.0f);
			fNumOffB = -poiBI.m_fX/m_fX;
			poiBProj = CPoint(
					poiBI.m_fY + m_fY*fNumOffB,
					poiBI.m_fZ + m_fZ*fNumOffB,
					0.0f);
			fNumOffC = -poiCI.m_fX/m_fX;
			poiCProj = CPoint(
					poiCI.m_fY + m_fY*fNumOffC,
					poiCI.m_fZ + m_fZ*fNumOffC,
					0.0f);
			//check the three points are locate on a line
			vecTmpA = poiAProj - poiCProj;
			vecTmpB = poiBProj - poiCProj;
			if(vecTmpA.isParallel(vecTmpB) || 0 == m_fX)	{
				//the triangle's three points are all zero points!
				return false;
			}
			else	{
				//calculate the point that the ray insert
				fNumOffRay = -m_poiBeg.m_fX/m_fX;
				poiRayProj = CPoint(
						m_poiBeg.m_fY + m_fY*fNumOffRay,
						m_poiBeg.m_fZ + m_fZ*fNumOffRay,
						0.0f);
			}
		}
		else	{
			//calculate the point that the ray insert
			fNumOffRay = -m_poiBeg.m_fY/m_fY;
			poiRayProj = CPoint(
					m_poiBeg.m_fX + m_fX*fNumOffRay,
					m_poiBeg.m_fZ + m_fZ*fNumOffRay,
					0.0f);
		}
	}
	else	{
		//calculate the point that the ray insert
		fNumOffRay = -m_poiBeg.m_fZ/m_fZ;
		poiRayProj = CPoint(
				m_poiBeg.m_fX + m_fX*fNumOffRay,
				m_poiBeg.m_fY + m_fY*fNumOffRay,
				0.0f);
	}
	//4.check point inside
	//use three projected points to build 3 line function
	//AB
	float fLnABK = 0.0f;
	float fLnABD = 0.0f;
	CPoint poiTmp = poiBProj - poiAProj;
	bool bABVerticalX = false;
	if(0 == poiTmp.m_fX)	{
		fLnABK = 1.0f;
		fLnABD = poiBProj.m_fX;
		bABVerticalX = true;
	}
	else	{
		fLnABK = poiTmp.m_fY/poiTmp.m_fX;
		fLnABD = poiAProj.m_fY - fLnABK*poiAProj.m_fX;
	}
	//BC
	float fLnBCK = 0.0f;
	float fLnBCD = 0.0f;
	poiTmp = poiCProj - poiBProj;
	bool bBCVerticalX = false;
	if(0 == poiTmp.m_fX)	{
		fLnBCK = 1.0f;
		fLnBCD = poiCProj.m_fX;
		bBCVerticalX = true;
	}
	else	{
		fLnBCK = poiTmp.m_fY/poiTmp.m_fX;
		fLnBCD = poiBProj.m_fY - fLnBCK*poiBProj.m_fX;
	}
	//AC
	float fLnACK = 0.0f;
	float fLnACD = 0.0f;
	poiTmp = poiAProj - poiCProj;
	bool bACVerticalX = false;
	if(0 == poiTmp.m_fX)	{
		fLnACK = 1.0f;
		fLnACD = poiAProj.m_fX;
		bACVerticalX = true;
	}
	else	{
		fLnACK = poiTmp.m_fY/poiTmp.m_fX;
		fLnACD = poiAProj.m_fY - fLnACK*poiAProj.m_fX;
	}
	//5.check the point that the ray inserted whether insert the triangle
	//AB
	if(bABVerticalX)	{
		//check which side is inside
		int iFlag = 0;
		if(poiCProj.m_fX > fLnABD)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		if(poiRayProj.m_fX == fLnABD)	{
			return true;
		}
		switch(iFlag)	{
		case 1:
			if(poiRayProj.m_fX < fLnABD)	{
				return false;
			}
			break;
		case -1:
			if(poiRayProj.m_fX > fLnABD)	{
				return false;
			}
			break;
		}
	}
	else	{
		//check which side is inside
		int iFlag = 0;
		float fResult = poiCProj.m_fX*fLnABK + fLnABD;
		if(fResult > poiCProj.m_fY)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		fResult = poiRayProj.m_fX*fLnABK + fLnABD;
		if(fResult == poiRayProj.m_fY)	{
			return true;
		}
		switch(iFlag)	{
		case 1:	
			if(fResult < poiRayProj.m_fY)	{
				return false;
			}
			break;
		case -1:
			if(fResult > poiRayProj.m_fY)	{
				return false;
			}
			break;
		}
	}
	//BC
	if(bBCVerticalX)	{
		//check which side is inside
		int iFlag = 0;
		if(poiAProj.m_fX > fLnBCD)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		if(poiRayProj.m_fX == fLnBCD)	{
			return true;
		}
		switch(iFlag)	{
		case 1:
			if(poiRayProj.m_fX < fLnBCD)	{
				return false;
			}
			break;
		case -1:
			if(poiRayProj.m_fX > fLnBCD)	{
				return false;
			}
			break;
		}
	}
	else	{
		//check which side is inside
		int iFlag = 0;
		float fResult = poiAProj.m_fX*fLnBCK + fLnBCD;
		if(fResult > poiAProj.m_fY)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		fResult = poiRayProj.m_fX*fLnBCK + fLnBCD;
		if(fResult == poiRayProj.m_fY)	{
			return true;
		}
		switch(iFlag)	{
		case 1:	
			if(fResult < poiRayProj.m_fY)	{
				return false;
			}
			break;
		case -1:
			if(fResult > poiRayProj.m_fY)	{
				return false;
			}
			break;
		}
	}
	//AC
	if(bACVerticalX)	{
		//check which side is inside
		int iFlag = 0;
		if(poiBProj.m_fX > fLnACD)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		if(poiRayProj.m_fX == fLnACD)	{
			return true;
		}
		switch(iFlag)	{
		case 1:
			if(poiRayProj.m_fX < fLnACD)	{
				return false;
			}
			break;
		case -1:
			if(poiRayProj.m_fX > fLnACD)	{
				return false;
			}
			break;
		}
	}
	else	{
		//check which side is inside
		int iFlag = 0;
		float fResult = poiBProj.m_fX*fLnACK + fLnACD;
		if(fResult > poiBProj.m_fY)
			iFlag = 1;
		else
			iFlag = -1;
		//check the true ray projecting point
		fResult = poiRayProj.m_fX*fLnACK + fLnACD;
		if(fResult == poiRayProj.m_fY)	{
			return true;
		}
		switch(iFlag)	{
		case 1:	
			if(fResult < poiRayProj.m_fY)	{
				return false;
			}
			break;
		case -1:
			if(fResult > poiRayProj.m_fY)	{
				return false;
			}
			break;
		}
	}
	return true;
}

CPoint CRay::intersectsTgl(CTriangle tglI)
{
	CPoint poiRet = this->intersectsPln(tglI);
	
	//3.check inside the triangle
	if(tglI.isPoiInside(poiRet))	{
		return(poiRet);
	}
	
	throw std::runtime_error("W:there's no intersection point on the triangle!");
}

CPoint CRay::intersectsRay(CRay rayI)
{
	this->normalize();
	rayI.normalize();
	if(this->isParallel(rayI))	{
		throw std::invalid_argument("W:parallel each other! has not intersection point!");
	}
	
	float fK = 0;
	float fValUp = 0;
	float fValDn = rayI.m_fX*this->m_fY - this->m_fX*rayI.m_fY;
	if(0 == fValDn)	{
		fValDn = rayI.m_fY*this->m_fZ - this->m_fY*rayI.m_fZ;
		if(0 == fValDn)	{
			fValDn = rayI.m_fX*this->m_fZ - this->m_fX*rayI.m_fZ;
			if(0 == fValDn)	{
				throw std::runtime_error("W:there no intersection point between the two points!");
			}
			else	{
				fValUp = this->m_fZ*this->m_poiBeg.m_fX - this->m_fZ*rayI.m_poiBeg.m_fX + 
						 this->m_fX*rayI.m_poiBeg.m_fZ - this->m_fX*this->m_poiBeg.m_fZ;
			}
		}
		else	{
			fValUp = this->m_fZ*this->m_poiBeg.m_fY - this->m_fZ*rayI.m_poiBeg.m_fY + 
					 this->m_fY*rayI.m_poiBeg.m_fZ - this->m_fY*this->m_poiBeg.m_fZ;
		}
	}
	else	{
		fValUp = this->m_fY*this->m_poiBeg.m_fX - this->m_fY*rayI.m_poiBeg.m_fX + 
				 this->m_fX*rayI.m_poiBeg.m_fY - this->m_fX*this->m_poiBeg.m_fY;
	}
	fK = fValUp/fValDn;
	
	CPoint poiRet = rayI*fK + rayI.m_poiBeg;
	poiRet.regulate();
	
	return(poiRet);
}

CPoint CRay::intersectsPln(CPlane plnI)
{
	if(this->isZeroVec())	{
		throw std::runtime_error("W:ray's direction is zero vector!");
	}
	
	this->normalize();
	this->regulate();
	//if close to 0, ray is parallel
	float fDet = this->dot(plnI.getNorVec()).sun();
	if(isZero<float>(fDet))	{
		throw std::runtime_error("W:there's no intersection point on the triangle!");
	}
	
	//distance to plane, <0 means ray behind the plane
	CVector vecNor = plnI.getNorVec();
	float fValCalc = plnI.calculate( this->m_poiBeg.m_fX, this->m_poiBeg.m_fY, this->m_poiBeg.m_fZ);
	if((this->isSameDir_VerticalIn(vecNor) && fValCalc > plnI.getD())
	|| (!this->isSameDir_VerticalIn(vecNor) && fValCalc < plnI.getD()))	{
		throw std::runtime_error("W:there's no intersection point on the triangle!");
	}
	
	float fValLeft = plnI.getD();
	float fValRight = plnI.calculate( m_poiBeg.m_fX, m_poiBeg.m_fY, m_poiBeg.m_fZ);
	float fValDown = plnI.calculate( m_fX, m_fY, m_fZ);
	
	if(0 == fValDown)	{
		throw std::runtime_error("W:no intersect points");
	}
	float fK = (fValLeft - fValRight)/fValDown;
	CPoint poiRet = this->m_poiBeg + *this*fK;
	
	return(poiRet);
}