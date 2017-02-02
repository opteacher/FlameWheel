#pragma once
#include "stdafx.h"
#include "Triangle.h"
#include "Ray.h"
#include "Line2D.h"

using namespace std;
using namespace OPMath;

CTriangle::CTriangle( CPoint poiAI, CPoint poiBI, CPoint poiCI, UINT uProjablePlnI): CPlane( poiAI, poiBI, poiCI)
{
	//1.data check
	if(poiAI == poiBI || poiBI == poiCI || poiCI == poiAI)	{
		throw std::invalid_argument("E:the three points equals to each other, can't compose to a triangle!");
	}
	CVector vecAB( poiAI, poiBI);	vecAB.normalize();
	CVector vecAC( poiAI, poiCI);	vecAC.normalize();
	if(vecAB == vecAC || vecAB.getNegVec() == vecAC)	{
		throw std::invalid_argument("E:three points in a line, can't compose to a triangle!");
	}
	
	m_poiA = poiAI;	m_poiB = poiBI;	m_poiC = poiCI;
	
	if(uProjablePlnI >= 3)	{
		if(m_vecNor.isSameDir_VerticalIn(CVector( 0.0f, 0.0f, 1.0f))
		|| m_vecNor.isSameDir_VerticalIn(CVector( 0.0f, 0.0f, -1.0f)))	{
			m_uProjablePln = PLN_XY;
		}
		else if(m_vecNor.isSameDir_VerticalIn(CVector( 1.0f, 0.0f, 0.0f))
			 || m_vecNor.isSameDir_VerticalIn(CVector( -1.0f, 0.0f, 0.0f)))	{
			m_uProjablePln = PLN_YZ;
		}
		else if(m_vecNor.isSameDir_VerticalIn(CVector( 0.0f, 1.0f, 0.0f))
		 	 || m_vecNor.isSameDir_VerticalIn(CVector( 0.0f, -1.0f, 0.0f)))	{
			m_uProjablePln = PLN_XZ;
		}
		else	{
			throw std::invalid_argument("E:three points in a point, can't compose to a triangle!");
		}
	}
	else	{
		switch(uProjablePlnI)	{
		case PLN_XY:
			if(this->m_vecNor.isVertical(CVector( 0.0f, 0.0f, 1.0f))
			|| this->m_vecNor.isVertical(CVector( 0.0f, 0.0f, -1.0f)))	{
				throw std::invalid_argument("W:triangle is Vertical to the identify plane!");
			}
			break;
		case PLN_YZ:
			if(this->m_vecNor.isVertical(CVector( 1.0f, 0.0f, 0.0f))
			|| this->m_vecNor.isVertical(CVector( -1.0f, 0.0f, 0.0f)))	{
				throw std::invalid_argument("W:triangle is Vertical to the identify plane!");
			}
			break;
		case PLN_XZ:
			if(this->m_vecNor.isVertical(CVector( 0.0f, 1.0f, 0.0f))
			|| this->m_vecNor.isVertical(CVector( 0.0f, -1.0f, 0.0f)))	{
				throw std::invalid_argument("W:triangle is Vertical to the identify plane!");
			}
			break;
		default:
			throw std::invalid_argument("W:identify plane is error");
		}
		
		m_uProjablePln = uProjablePlnI;
	}
}

bool CTriangle::isPoiInside(CPoint poiI) const
{
	//1.check if on the plane
	CVector vecAB( m_poiA, m_poiB);	vecAB.normalize();
	CVector vecAC( m_poiA, m_poiC);	vecAC.normalize();
	if(vecAB.isZeroVec() || vecAC.isZeroVec()
	|| vecAB == vecAC || vecAB.getNegVec() == vecAC)	{
		throw std::runtime_error("E:the triangle has uninitlized, or data error!");
	}
	//2.points not on the plane of triangle
	if(!isFillToFun( poiI.m_fX, poiI.m_fY, poiI.m_fZ))	{
		return false;
	}
	//3.project to each coord plane, and check the point whether inside the triangle
	//XY plane
	CPoint poiAOnXY( m_poiA.m_fX, m_poiA.m_fY, 0);
	CPoint poiBOnXY( m_poiB.m_fX, m_poiB.m_fY, 0);
	CPoint poiCOnXY( m_poiC.m_fX, m_poiC.m_fY, 0);
	//check whether in a line
	CVector vecABOnXY( poiAOnXY, poiBOnXY);	vecABOnXY.normalize();
	CVector vecACOnXY( poiAOnXY, poiCOnXY);	vecACOnXY.normalize();
	if(vecABOnXY == vecACOnXY || vecABOnXY.getNegVec() == vecACOnXY)	{
		//YZ plane
		CPoint poiAOnYZ( m_poiA.m_fY, m_poiA.m_fZ, 0);
		CPoint poiBOnYZ( m_poiB.m_fY, m_poiB.m_fZ, 0);
		CPoint poiCOnYZ( m_poiC.m_fY, m_poiC.m_fZ, 0);
		
		CVector vecABOnYZ( poiAOnYZ, poiBOnYZ);	vecABOnYZ.normalize();
		CVector vecACOnYZ( poiAOnYZ, poiCOnYZ);	vecACOnYZ.normalize();
		if(vecABOnYZ == vecACOnYZ || vecABOnYZ.getNegVec() == vecACOnYZ)	{
			//XZ plane
			CPoint poiAOnXZ( m_poiA.m_fX, m_poiA.m_fZ, 0);
			CPoint poiBOnXZ( m_poiB.m_fX, m_poiB.m_fZ, 0);
			CPoint poiCOnXZ( m_poiC.m_fX, m_poiC.m_fZ, 0);
			
			CVector vecABOnXZ( poiAOnXZ, poiBOnXZ);	vecABOnXZ.normalize();
			CVector vecACOnXZ( poiAOnXZ, poiCOnXZ);	vecACOnXZ.normalize();
			
			if(vecABOnXZ == vecACOnXZ || vecABOnXZ.getNegVec() == vecACOnXZ)	{
				throw std::runtime_error("E:three points in a point!");
			}
			else	{
				CLine2D lnABOnXZ( poiAOnXZ, poiBOnXZ);
				CLine2D lnBCOnXZ( poiBOnXZ, poiCOnXZ);
				CLine2D lnCAOnXZ( poiCOnXZ, poiAOnXZ);
				
				lnABOnXZ.mkFunType( poiCOnXZ.m_fX, poiCOnXZ.m_fZ);
				lnBCOnXZ.mkFunType( poiAOnXZ.m_fX, poiAOnXZ.m_fZ);
				lnCAOnXZ.mkFunType( poiBOnXZ.m_fX, poiBOnXZ.m_fZ);
				
				if(lnABOnXZ.isFillToFun( poiI.m_fX, poiI.m_fZ, 0)
				&& lnBCOnXZ.isFillToFun( poiI.m_fX, poiI.m_fZ, 0)
				&& lnCAOnXZ.isFillToFun( poiI.m_fX, poiI.m_fZ, 0))	{
					return true;
				}
			}
		}
		else	{
			CLine2D lnABOnYZ( poiAOnYZ, poiBOnYZ);
			CLine2D lnBCOnYZ( poiBOnYZ, poiCOnYZ);
			CLine2D lnCAOnYZ( poiCOnYZ, poiAOnYZ);
			
			lnABOnYZ.mkFunType( poiCOnYZ.m_fY, poiCOnYZ.m_fZ);
			lnBCOnYZ.mkFunType( poiAOnYZ.m_fY, poiAOnYZ.m_fZ);
			lnCAOnYZ.mkFunType( poiBOnYZ.m_fY, poiBOnYZ.m_fZ);
			
			if(lnABOnYZ.isFillToFun( poiI.m_fY, poiI.m_fZ, 0)
			&& lnBCOnYZ.isFillToFun( poiI.m_fY, poiI.m_fZ, 0)
			&& lnCAOnYZ.isFillToFun( poiI.m_fY, poiI.m_fZ, 0))	{
				return true;
			}
		}
	}
	else	{
		//compose three line of points ABC, use their function to check the point inside or not
		CLine2D lnABOnXY( poiAOnXY, poiBOnXY);
		CLine2D lnBCOnXY( poiBOnXY, poiCOnXY);
		CLine2D lnCAOnXY( poiCOnXY, poiAOnXY);
		//use another point to check which side is inside the triangle
		lnABOnXY.mkFunType( poiCOnXY.m_fX, poiCOnXY.m_fY);
		lnBCOnXY.mkFunType( poiAOnXY.m_fX, poiAOnXY.m_fY);
		lnCAOnXY.mkFunType( poiBOnXY.m_fX, poiBOnXY.m_fY);
		//check whether the point follow the function
		if(lnABOnXY.isFillToFun( poiI.m_fX, poiI.m_fY, 0)
		&& lnBCOnXY.isFillToFun( poiI.m_fX, poiI.m_fY, 0)
		&& lnCAOnXY.isFillToFun( poiI.m_fX, poiI.m_fY, 0))	{
			return true;
		}
	}
	return false;
}

vector<CPoint> CTriangle::getEvePoisInside() const
{
	
	vector<CPoint> vecPoisRet = vector<CPoint>();

	CPoint poiAProj;
	CPoint poiBProj;
	CPoint poiCProj;
	
	switch(m_uProjablePln)	{
	case PLN_XY:
		poiAProj = CPoint( m_poiA.m_fX, m_poiA.m_fY, 0);
		poiBProj = CPoint( m_poiB.m_fX, m_poiB.m_fY, 0);
		poiCProj = CPoint( m_poiC.m_fX, m_poiC.m_fY, 0);
		break;
	case PLN_YZ:
		poiAProj = CPoint( m_poiA.m_fY, m_poiA.m_fZ, 0);
		poiBProj = CPoint( m_poiB.m_fY, m_poiB.m_fZ, 0);
		poiCProj = CPoint( m_poiC.m_fY, m_poiC.m_fZ, 0);
		break;
	case PLN_XZ:
		poiAProj = CPoint( m_poiA.m_fX, m_poiA.m_fZ, 0);
		poiBProj = CPoint( m_poiB.m_fX, m_poiB.m_fZ, 0);
		poiCProj = CPoint( m_poiC.m_fX, m_poiC.m_fZ, 0);
		break;
	}
	
	try{
		CLine2D lnAB( poiAProj, poiBProj);
		CLine2D lnBC( poiBProj, poiCProj);
		CLine2D lnCA( poiCProj, poiAProj);
	
		lnAB.mkFunType( poiCProj.m_fX, poiCProj.m_fY);
		lnBC.mkFunType( poiAProj.m_fX, poiAProj.m_fY);
		lnCA.mkFunType( poiBProj.m_fX, poiBProj.m_fY);
	
		float fMaxI = 0;
		float fMaxJ = 0;
		float fMinI = 0;
		float fMinJ = 0;
	
		switch(m_uProjablePln)	{
		case PLN_XY:
			fMaxI = max( m_poiA.m_fX, m_poiB.m_fX);
			fMaxI = max( fMaxI, m_poiC.m_fX);
			fMaxJ = max( m_poiA.m_fY, m_poiB.m_fY);
			fMaxJ = max( fMaxJ, m_poiC.m_fY);
		
			fMinI = min( m_poiA.m_fX, m_poiB.m_fX);
			fMinI = min( fMinI, m_poiC.m_fX);
			fMinJ = min( m_poiA.m_fY, m_poiB.m_fY);
			fMinJ = min( fMinJ, m_poiC.m_fY);
		
			break;
		case PLN_YZ:
			fMaxI = max( m_poiA.m_fY, m_poiB.m_fY);
			fMaxI = max( fMaxI, m_poiC.m_fY);
			fMaxJ = max( m_poiA.m_fZ, m_poiB.m_fZ);
			fMaxJ = max( fMaxJ, m_poiC.m_fZ);
		
			fMinI = min( m_poiA.m_fY, m_poiB.m_fY);
			fMinI = min( fMinI, m_poiC.m_fY);
			fMinJ = min( m_poiA.m_fZ, m_poiB.m_fZ);
			fMinJ = min( fMinJ, m_poiC.m_fZ);
		
			break;
		case PLN_XZ:
			fMaxI = max( m_poiA.m_fX, m_poiB.m_fX);
			fMaxI = max( fMaxI, m_poiC.m_fX);
			fMaxJ = max( m_poiA.m_fZ, m_poiB.m_fZ);
			fMaxJ = max( fMaxJ, m_poiC.m_fZ);
		
			fMinI = min( m_poiA.m_fX, m_poiB.m_fX);
			fMinI = min( fMinI, m_poiC.m_fX);
			fMinJ = min( m_poiA.m_fZ, m_poiB.m_fZ);
			fMinJ = min( fMinJ, m_poiC.m_fZ);

			break;
		}

		for( float fIndI = fMinI; fIndI < fMaxI; ++fIndI)	{
			for( float fIndJ = fMinJ; fIndJ < fMaxJ; ++fIndJ)	{
				if(lnAB.isFillToFun( fIndI, fIndJ, 0)
				&& lnBC.isFillToFun( fIndI, fIndJ, 0)
				&& lnCA.isFillToFun( fIndI, fIndJ, 0))	{
					switch(m_uProjablePln)	{
					case PLN_XY:	{
						float fZ = getZOfPln( fIndI, fIndJ);
						vecPoisRet.push_back(CPoint( fIndI, fIndJ, fZ));
									}
						break;
					case PLN_YZ:	{
						float fX = getXOfPln( fIndI, fIndJ);
						vecPoisRet.push_back(CPoint( fX, fIndI, fIndJ));
									}
						break;
					case PLN_XZ:	{
						float fY = getYOfPln( fIndI, fIndJ);
						vecPoisRet.push_back(CPoint( fIndI, fY, fIndJ));
									}
						break;
					}
				}
			}
		}
	}
	catch(std::exception e)	{
		return(vector<CPoint>());
	}
	return(vecPoisRet);
}

CPoint CTriangle::getPoiCen() const
{
	CPoint poiABCen = (m_poiA + m_poiB)/2;
	CPoint poiBCCen = (m_poiB + m_poiC)/2;
	
	CVector vecAB( m_poiA, m_poiB); vecAB.normalize();
	CVector vecBC( m_poiB, m_poiC); vecBC.normalize();
	CVector vecTglNor = m_vecNor;
	
	CVector vecRayVerToAB = vecAB.cross(vecTglNor);
	if(!vecRayVerToAB.isSameDir_VerticalIn(CVector( poiABCen, m_poiC)))	{
		vecRayVerToAB = vecRayVerToAB.getNegVec();
	}
	vecRayVerToAB.normalize();
	
	CVector vecRayVerToBC = vecBC.cross(vecTglNor);
	if(!vecRayVerToBC.isSameDir_VerticalIn(CVector( poiBCCen, m_poiA)))	{
		vecRayVerToBC = vecRayVerToBC.getNegVec();
	}
	vecRayVerToBC.normalize();
	
	CRay rayVerToAB( poiABCen, vecRayVerToAB);
	CRay rayVerToBC( poiBCCen, vecRayVerToBC);
	
	CPoint poiRet;
	try	{
		poiRet = rayVerToAB.intersectsRay(rayVerToBC);
	}
	catch(std::exception e)	{
		return(CPoint());
	}
	
	return(poiRet);
}