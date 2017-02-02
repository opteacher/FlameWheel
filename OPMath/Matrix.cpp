#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "Math.h"

using namespace OPMath;
using namespace std;

CMatrix::CMatrix(void): m_uNumDim(DIM_3D)
{
	tunIndMat();
}

CMatrix::CMatrix(UINT uDimI)	{
	if(uDimI > DIM_3D)	{
		m_uNumDim = DIM_3D;
	}
	else	{
		m_uNumDim = uDimI;
	}
	
	tunIndMat();
}

CMatrix::CMatrix(const float pMatI[DIM_3D][DIM_3D]): m_uNumDim(DIM_3D)
{
	//memcpy( m_fVal, pMatI, DIM_3D*DIM_3D*sizeof(float));
	for( UINT i = 0; i < DIM_3D; ++i)	{
		for( UINT j = 0; j < DIM_3D; ++j)	{
			m_fVal[i][j] = pMatI[i][j];
		}
	}
}

CMatrix::CMatrix(const CMatrix& refMatI)
{
	m_uNumDim = refMatI.m_uNumDim;
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			m_fVal[i][j] = refMatI.m_fVal[i][j];
		}
	}
}

CMatrix& CMatrix::operator=(const CMatrix& refMatI)	{
	m_uNumDim = refMatI.m_uNumDim;
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			m_fVal[i][j] = refMatI.m_fVal[i][j];
		}
	}
	return(*this);
}

CMatrix CMatrix::operator+(const CMatrix& refMatI)
{
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			matRet.m_fVal[j][i] = m_fVal[j][i] + refMatI.m_fVal[j][i];
		}
	}
	return(matRet);
}

CMatrix CMatrix::operator-(const CMatrix& refMatI)
{
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			matRet.m_fVal[j][i] = m_fVal[j][i] - refMatI.m_fVal[j][i];
		}
	}
	return(matRet);
}

CMatrix CMatrix::operator*(const float& fValI)
{
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			matRet.m_fVal[j][i] = fValI * m_fVal[j][i];
		}
	}
	return(matRet);
}

CMatrix CMatrix::operator-()
{
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			m_fVal[j][i] = -m_fVal[j][i];
		}
	}
	return(*this);
}

bool CMatrix::operator==(const CMatrix& refMatI)
{
	if(m_uNumDim != refMatI.m_uNumDim)	{
		return false;
	}
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			if(!::equals<float>( m_fVal[i][j], refMatI.m_fVal[i][j]))	{
				return false;
			}
		}
	}
	return true;
}

bool CMatrix::operator!=(const CMatrix& refMatI)
{
	if(m_uNumDim != refMatI.m_uNumDim)	{
		return true;
	}
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			if(!::equals( m_fVal[i][j], refMatI.m_fVal[i][j]))	{
				return true;
			}
		}
	}
	return false;
}

CMatrix CMatrix::operator+=(const CMatrix& refMatI)
{
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			m_fVal[i][j] += refMatI.m_fVal[i][j];
		}
	}
	return(*this);
}

CMatrix CMatrix::operator-=(const CMatrix& refMatI)
{
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			m_fVal[i][j] -= refMatI.m_fVal[i][j];
		}
	}
	return(*this);
}

CMatrix CMatrix::dotLeft(const CMatrix& refMatI) const
{
	if(m_uNumDim != refMatI.m_uNumDim)	{
		throw std::runtime_error("dimension different, can't do dot calculate");
	}
	
	CMatrix matRet(m_uNumDim);
	float fValSum = 0;
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			for( UINT n = 0; n < m_uNumDim; ++n)	{
				fValSum += m_fVal[n][j] * refMatI.m_fVal[i][n];
			}
			matRet.m_fVal[i][j] = fValSum;
			fValSum	= 0.0f;
		}
	}
	return(matRet);
}

CMatrix CMatrix::dotLeftToSelf(const CMatrix& matI)
{
	*this = this->dotLeft(matI);	return *this;
}

CMatrix CMatrix::dotRight(const CMatrix& refMatI) const
{
	if(m_uNumDim != refMatI.m_uNumDim)	{
		throw std::runtime_error("dimension different, can't do dot calculate");
	}
	CMatrix matRet(m_uNumDim);
	float fValSum	= 0;
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			for( UINT n = 0; n < m_uNumDim; ++n)	{
				fValSum	+= m_fVal[i][n] * refMatI.m_fVal[n][j];
			}
			matRet.m_fVal[i][j]	= fValSum;
			fValSum	= 0.0f;
		}
	}
	return(matRet);
}

CMatrix CMatrix::dotRightToSelf(const CMatrix& matI)
{
	*this = this->dotRight(matI);	return *this;
}

CPoint CMatrix::multiPoiRight(CPoint poiI) const
{
	CPoint poiRet;
	
	if(m_uNumDim <= DIM_2D)	{
		if(poiI.m_fZ == 0.0f)	{
			poiI.m_fZ	= 1.0f;
		}//if z == 0.0f
		else if(poiI.m_fY == 0.0f)	{
			poiI.m_fY	= 1.0f;
		}//else if y == 0.0f
		
		poiRet.m_fX	= poiI.m_fX*m_fVal[0][0] + poiI.m_fY*m_fVal[0][1] + poiI.m_fH*m_fVal[0][2];
		poiRet.m_fY	= poiI.m_fX*m_fVal[1][0] + poiI.m_fY*m_fVal[1][1] + poiI.m_fH*m_fVal[1][2];
		poiRet.m_fH	= poiI.m_fX*m_fVal[2][0] + poiI.m_fY*m_fVal[2][1] + poiI.m_fH*m_fVal[2][2];
	}
	else	{
		poiRet.m_fX	= poiI.m_fX*m_fVal[0][0] + poiI.m_fY*m_fVal[0][1] + poiI.m_fZ*m_fVal[0][2] + poiI.m_fH*m_fVal[0][3];
		poiRet.m_fY	= poiI.m_fX*m_fVal[1][0] + poiI.m_fY*m_fVal[1][1] + poiI.m_fZ*m_fVal[1][2] + poiI.m_fH*m_fVal[1][3];
		poiRet.m_fZ	= poiI.m_fX*m_fVal[2][0] + poiI.m_fY*m_fVal[2][1] + poiI.m_fZ*m_fVal[2][2] + poiI.m_fH*m_fVal[2][3];
		poiRet.m_fH	= poiI.m_fX*m_fVal[3][0] + poiI.m_fY*m_fVal[3][1] + poiI.m_fZ*m_fVal[3][2] + poiI.m_fH*m_fVal[3][3];
	}

	poiRet = poiRet / poiRet.m_fH;
	
	//poiRet.regulate();

	return(poiRet);
}

CPoint CMatrix::multiPoiLeft(CPoint poiI) const
{
	CPoint poiRet;
		
	if(m_uNumDim <= DIM_2D)	{
		if(poiI.m_fZ == 0.0f)	{
			poiI.m_fZ	= 1.0f;
		}//if z == 0.0f
		else if(poiI.m_fY == 0.0f)	{
			poiI.m_fY	= 1.0f;
		}//else if y == 0.0f
		
		poiRet.m_fX	= poiI.m_fX*m_fVal[0][0] + poiI.m_fY*m_fVal[1][0] + poiI.m_fH*m_fVal[2][0];
		poiRet.m_fY	= poiI.m_fX*m_fVal[0][1] + poiI.m_fY*m_fVal[1][1] + poiI.m_fH*m_fVal[2][1];
		poiRet.m_fH	= poiI.m_fX*m_fVal[0][2] + poiI.m_fY*m_fVal[1][2] + poiI.m_fH*m_fVal[2][2];
	}
	else	{
		poiRet.m_fX	= poiI.m_fX*m_fVal[0][0] + poiI.m_fY*m_fVal[1][0] + poiI.m_fZ*m_fVal[2][0] + poiI.m_fH*m_fVal[3][0];
		poiRet.m_fY	= poiI.m_fX*m_fVal[0][1] + poiI.m_fY*m_fVal[1][1] + poiI.m_fZ*m_fVal[2][1] + poiI.m_fH*m_fVal[3][1];
		poiRet.m_fZ	= poiI.m_fX*m_fVal[0][2] + poiI.m_fY*m_fVal[1][2] + poiI.m_fZ*m_fVal[2][2] + poiI.m_fH*m_fVal[3][2];
		poiRet.m_fH	= poiI.m_fX*m_fVal[0][3] + poiI.m_fY*m_fVal[1][3] + poiI.m_fZ*m_fVal[2][3] + poiI.m_fH*m_fVal[3][3];
	}

	if(0 != poiRet.m_fH && 1 != poiRet.m_fH)	{
		poiRet = poiRet / poiRet.m_fH;
	}
	else
	{
		poiRet.m_fH = 1.0f;
	}
	
	//poiRet.regulate();

	return(poiRet);
}

CVector CMatrix::multiVecLeft(CVector vecI) const
{
	return(CVector(multiPoiLeft(CPoint(vecI))));
}

CVector CMatrix::multiVecRight(CVector vecI) const
{
	return(CVector(multiPoiRight(CPoint(vecI))));
}

float CMatrix::getDetVal()
{
	//compose positive number
	float fPosVal	= 0;
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		float fValMulti = 1;
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			UINT iValTemp = i + j;
			if(iValTemp >= m_uNumDim)	{
				iValTemp -= m_uNumDim;
			}
			float fValTmp = m_fVal[j][iValTemp];
			fValMulti *= fValTmp;
		}//end for 2
		fPosVal += fValMulti;
	}
	//compose negative number
	float fNegVal	= 0;
	for( UINT i = m_uNumDim - 1; i >= 0; --i)	{
		float fValMulti = 1;
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			UINT iValTemp = i - j;
			if(iValTemp < 0)	{
				iValTemp += m_uNumDim;
			}
			float fValTmp = m_fVal[j][iValTemp];
			fValMulti *= fValTmp;
		}//end for 2
		fNegVal += fValMulti;
	}
	return(fPosVal - fNegVal);
}

CMatrix CMatrix::getDecip()
{
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			if(0 != m_fVal[j][i])	{
				matRet.m_fVal[j][i] = 1.0f / m_fVal[j][i];
			}
		}
	}
	return(matRet);
}

CMatrix CMatrix::getAdjMat()	{
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)
		for( UINT j = 0; j < m_uNumDim; ++j)	{
				
			CMatrix matTmp(m_uNumDim - 1);
			for( UINT iT = 0, iIndRow = 0; iT < m_uNumDim && iIndRow < m_uNumDim - 1; ++iT)	{
				for( UINT jT = 0, iIndCol = 0; jT < m_uNumDim && iIndCol < m_uNumDim - 1; ++jT)	{
					if(iT != i && jT != j)	{
						matTmp.m_fVal[iIndCol][iIndRow] = this->m_fVal[jT][iT];
						++iIndCol;
					}
				}
				if(iT != i)	{
					++iIndRow;
				}
			}
				
			float fResDet = matTmp.getDetVal();
			matRet.m_fVal[j][i] = pow( (float)-1, (int)(j+i)) * fResDet;
		}
	return(matRet);
}

CMatrix CMatrix::tunIndMat()
{
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			if(i == j)	{
				m_fVal[j][i] = 1.0f;
			}
			else	{
				m_fVal[j][i] = 0.0f;
			}
		}
	}
	return(*this);
}

CMatrix CMatrix::getNegMat()
{
	float fDetRes = getDetVal();
	if(0 == fDetRes)	{
		throw std::runtime_error("the matrix's det is zero");
	}

	CMatrix matAdj = getAdjMat();
	CMatrix matRet(m_uNumDim);
	for( UINT i = 0; i < m_uNumDim; ++i)	{
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			matRet.m_fVal[j][i]	= matAdj.m_fVal[j][i] / fDetRes;
		}
	}
	return(matRet);
}

CMatrix CMatrix::getTrspMat()
{
	CMatrix matRet(m_uNumDim);
	UINT t = 0;
	for( UINT i = 0; i < m_uNumDim; ++i, ++t)	{
		for( UINT j = t; j < m_uNumDim; ++j)	{
			if(i != j)	{
				matRet.m_fVal[i][j] = m_fVal[j][i];
				matRet.m_fVal[j][i] = m_fVal[i][j];
			}
		}
	}
	return(matRet);
}

bool CMatrix::isIndMat()
{
	for( UINT i = 0; i < m_uNumDim; ++i)
		for( UINT j = 0; j < m_uNumDim; ++j)	{
			if(i == j)	{
				if(m_fVal[i][j] != 1.0f)
					return false;
			}
			else	{
				if(m_fVal[i][j] != 0.0f)
					return false;
			}
		}
	return true;
}

CMatrix CMatrix::rotate2D(float fAngI)
{
	CMatrix matRet(DIM_2D);
	//turn to radian
	float fAng	= ((float)FW_PI/180.0f)*fAngI;
	//set matrix value
	matRet.m_fVal[0][0] = cos(fAng);
	matRet.m_fVal[1][0] = -sin(fAng);
	matRet.m_fVal[0][1] = -matRet.m_fVal[1][0];
	matRet.m_fVal[1][1] = matRet.m_fVal[0][0];
	
	return(matRet);
}

CMatrix CMatrix::rotate2D( float fAngI, CPoint poiCtI)
{
	//1.create two matrix to transfer and reset
	CMatrix matTransA = transfer2D( poiCtI.m_fX, poiCtI.m_fY);
	CMatrix matTransB = transfer2D( -poiCtI.m_fX, -poiCtI.m_fY);
	//2.create rotate matrix
	CMatrix matRet = rotate2D(fAngI);
	//3.compose multiply: matTransA * matRet * matTransB
	matRet = matRet.dotLeft(matTransA);
	matRet = matRet.dotRight(matTransB);
	
	return(matRet);
}

CMatrix CMatrix::transfer2D( float fXOffI, float fYOffI)
{
	CMatrix matRet(DIM_2D);
	matRet.m_fVal[2][0]	= fXOffI;
	matRet.m_fVal[2][1]	= fYOffI;

	return(matRet);
}

CMatrix CMatrix::scale2D( float fXDiffI, float fYDiffI)
{
	CMatrix matRet(DIM_2D);
	matRet.m_fVal[0][0]	= fXDiffI;
	matRet.m_fVal[1][1]	= fYDiffI;
	
	return(matRet);
}

CMatrix CMatrix::scale2D( float fXDiffI, float fYDiffI, CPoint poiCtI)
{
	//1.create two matrix to transfer and reset
	CMatrix matTransA = transfer2D( -poiCtI.m_fX, -poiCtI.m_fY);
	CMatrix matTransB = transfer2D( poiCtI.m_fX, poiCtI.m_fY);
	//2.create scale matrix
	CMatrix matRet = scale2D( fXDiffI, fYDiffI);
	//3.compose multiply: matTransA * matRet * matTransB
	matRet = matRet.dotLeft(matTransA);
	matRet = matRet.dotRight(matTransB);
	
	return(matRet);
}

CMatrix CMatrix::transfer3D( float fXOffI, float fYOffI, float fZOffI)
{
	CMatrix matRet(DIM_3D);
	matRet.m_fVal[3][0] = fXOffI;
	matRet.m_fVal[3][1] = fYOffI;
	matRet.m_fVal[3][2] = fZOffI;
	
	return(matRet);
}

CMatrix CMatrix::rotate3D( float fAngI, CVector vecAxisI, bool bUseQatI)
{
	//1.turn angle to radian
	float fAngRad = (float)FW_PI*fAngI*0.005555556f;

	CMatrix matRet(DIM_3D);
	vecAxisI.normalize();
	if(bUseQatI)
	{
		//2.compose the rotate Quaternion
		CVector poiQuate;
		try	{
			poiQuate = vecAxisI * sinEx(fAngRad*.5f);
			poiQuate.m_fH = cosEx(fAngRad*.5f);
		}
		catch(std::exception e)	{
			return(CMatrix());
		}

		//3.compose the matrix
		float fXX = poiQuate.m_fX*poiQuate.m_fX;
		float fYY = poiQuate.m_fY*poiQuate.m_fY;
		float fZZ = poiQuate.m_fZ*poiQuate.m_fZ;
		float fXY = poiQuate.m_fX*poiQuate.m_fY;
		float fYZ = poiQuate.m_fY*poiQuate.m_fZ;
		float fXZ = poiQuate.m_fX*poiQuate.m_fZ;
		float fHX = poiQuate.m_fH*poiQuate.m_fX;
		float fHY = poiQuate.m_fH*poiQuate.m_fY;
		float fHZ = poiQuate.m_fH*poiQuate.m_fZ;

		matRet.m_fVal[0][0]	= 1 - 2*fYY - 2*fZZ;
		matRet.m_fVal[0][1]	=     2*fXY + 2*fHZ;
		matRet.m_fVal[0][2]	=     2*fXZ - 2*fHY;

		matRet.m_fVal[1][0]	=     2*fXY - 2*fHZ;
		matRet.m_fVal[1][1]	= 1 - 2*fXX - 2*fZZ;
		matRet.m_fVal[1][2]	=     2*fYZ + 2*fHX;

		matRet.m_fVal[2][0]	=     2*fXZ + 2*fHY;
		matRet.m_fVal[2][1]	=     2*fYZ - 2*fHX;
		matRet.m_fVal[2][2]	= 1 - 2*fXX - 2*fYY;
	}
	else
	{
		float fCos = cosEx(fAngRad);
		float fSin = sinEx(fAngRad);

		//3.compose the matrix
		float fX = vecAxisI.m_fX;
		float fY = vecAxisI.m_fY;
		float fZ = vecAxisI.m_fZ;
		float fXX = fX*fX;
		float fYY = fY*fY;
		float fZZ = fZ*fZ;
		float fXY = fX*fY;
		float fYZ = fY*fZ;
		float fXZ = fX*fZ;

		matRet.m_fVal[0][0]	=          fCos  + fXX*(1 - fCos);
		matRet.m_fVal[0][1]	= fXY*(1 - fCos) + fZ *     fSin;
		matRet.m_fVal[0][2]	= fXZ*(1 - fCos) - fY *     fSin;

		matRet.m_fVal[1][0]	= fXY*(1 - fCos) - fZ *     fSin;
		matRet.m_fVal[1][1]	=          fCos  + fYY*(1 - fCos);
		matRet.m_fVal[1][2]	= fYZ*(1 - fCos) + fX *     fSin;

		matRet.m_fVal[2][0]	= fXZ*(1 - fCos) + fY *     fSin;
		matRet.m_fVal[2][1]	= fYZ*(1 - fCos) - fX *     fSin;
		matRet.m_fVal[2][2]	=          fCos  + fZZ*(1 - fCos);
	}

	//ofstream ofStm( "test_me.txt", ios_base::out | ios_base::app);
	//ofStm<<"Angel:"<<fAngI<<"  "<<endl;
	//for( UINT i = 0; i < DIM_3D; ++i)
	//{
	//	for( UINT j = 0; j < DIM_3D; ++j)
	//	{
	//		ofStm<<matRet.m_fVal[i][j];
	//		if(j != DIM_3D - 1)	{ ofStm<<','; }
	//	}
	//	ofStm<<endl;
	//}
	//ofStm<<endl<<endl;
	//ofStm.close();
	
	return(matRet);
}

CMatrix CMatrix::rotate3D( float fAngI, CRay rayAxisI, bool bUseQatI)	{
	CVector vecAxis = (CVector)rayAxisI;
	CMatrix matRot = rotate3D( fAngI, vecAxis, bUseQatI);
	CPoint poiDisTrsf = rayAxisI.getPoiBeg();
	CMatrix matNegTrans = transfer3D( -poiDisTrsf.m_fX, -poiDisTrsf.m_fY, -poiDisTrsf.m_fZ);
	CMatrix matPosTrans = transfer3D( poiDisTrsf.m_fX, poiDisTrsf.m_fY, poiDisTrsf.m_fZ);
	
	CMatrix matRet(DIM_3D);
	matRet = matRet.dotRight(matNegTrans);
	matRet = matRet.dotRight(matRot);
	matRet = matRet.dotRight(matPosTrans);
	
	return(matRet);
}

CMatrix CMatrix::rotate3D( CVector vecBegI, CVector vecEndI)
{
	vecBegI.normalize();	vecEndI.normalize();
	CVector vecRotAxis = vecBegI.cross(vecEndI);
	//the same vector, return Identity Matrix
	if(vecRotAxis.isZeroVec())	{ return(CMatrix()); }
	float fAngTmp = (float)vecEndI.angWithVec( vecBegI, false);
	CMatrix matRot = rotate3D( fAngTmp, vecRotAxis, true);
	return matRot;
}

CMatrix CMatrix::scale3D( float fSxi, float fSyi, float fSzi, CPoint poiCenI)
{
	CMatrix matRet(DIM_3D);
	//ÉèÖÃËõ·ÅÖµ
	matRet.m_fVal[0][0]	= fSxi;
	matRet.m_fVal[1][1]	= fSyi;
	matRet.m_fVal[2][2]	= fSzi;

	CMatrix matTransFront(DIM_3D);
	CMatrix matTransBack(DIM_3D);

	matTransFront.transfer3D( poiCenI.m_fX, poiCenI.m_fY, poiCenI.m_fZ);
	matTransBack.transfer3D( -poiCenI.m_fX, -poiCenI.m_fY, -poiCenI.m_fZ);

	matRet = matRet.dotLeft(matTransBack);
	matRet = matRet.dotRight(matTransFront);
	return(matRet);
}
