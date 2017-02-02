#pragma once
#include "stdafx.h"
#include "Math.h"
#include "Mmsystem.h"
#pragma comment( lib, "winmm.lib")

using namespace std;
using namespace OPMath;

extern bool g_bCheckSptSSE	= false;
extern bool g_bSupportSSE	= false;

extern const double OPMath::MAX_VAL = 9999999;
extern const double OPMath::MIN_VAL = 0.00001;
extern const double OPMath::FW_PI = 3.14159265358979323846;

bool OPMath::isOSSupportsSSE()
{
	if(g_bCheckSptSSE)	{
		return(g_bSupportSSE);
	}
	__try
	{
		_asm xorps xmm0, xmm0
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		if ( _exception_code() == STATUS_ILLEGAL_INSTRUCTION)	{
			g_bSupportSSE	= false;
		}
		else	{
			g_bSupportSSE	= false;
		}
	}
	g_bSupportSSE	= true;
	g_bCheckSptSSE	= true;
	return(g_bSupportSSE);
}

template<typename T> string numToStr(T tNumI)
{
		string strRet = string();
		//zero test
		if(0 == tNumI)	{
			return("0");
		}
		//negative test
		if(tNumI < 0)	{
			strRet += "-";
		}
	
		//compose the integer
		DWORD dwOdd = 0;
		DWORD dwEntire = (DWORD)tNumI;
		string strTmp = string();
		while(dwEntire)	{
			dwOdd = dwEntire%10;
			strTmp += (char)(dwOdd + +48);
			dwEntire /= 10;
		}
	
		//swap the head and tail
		for( int i = strTmp.length() - 1; i >= 0; --i)	{
			strRet += strTmp[i];
		}
	
		//float test
		if((DWORD)tNumI != tNumI)	{
			T tFloat = tNumI - (DWORD)tNumI;
			T tEntire = tFloat;
			strTmp = "";
			while(tEntire)	{
				tEntire *= 10;
				dwOdd = (DWORD)tEntire;
				strTmp += (char)(dwOdd + +48);
				tEntire -= (T)dwOdd;
			}
		
			//add the point
			strRet += ".";
		
			//swap the head and tail
			for( int i = strTmp.length() - 1; i >= 0; --i)	{
				strRet += strTmp[i];
			}
		}
		return(strRet);
	}

DWORD OPMath::geneHashVal( const char* strValI )
{
	//if the param is empty, use current time to switch
	string strValTmp = "";
	if(strValI == NULL)	{
		strValTmp = numToStr<DWORD>(timeGetTime());
	}
	else	{
		strValTmp = strValI;
	}
	//process the string
	size_t uLenStr = strValTmp.length();
	UINT uNumCut = geneRandVal<UINT>( 1, uLenStr/3);
	string strRes = strValTmp;
	for( UINT i = 0; i < uNumCut; ++i)	{
		string::iterator iter = strRes.begin() + geneRandVal<string::value_type>( 0, strRes.length() - 1);
		strRes.erase(iter);
	}
	//generate the hash value
	DWORD dwB = 378551;
	DWORD dwA = 63689;
	DWORD dwHash = 0;
	for( UINT i = 0; i < strRes.length(); ++i)	{
		dwHash = dwHash*dwA + strRes[i];
		dwA = dwA*dwB;
	}

	return(dwHash);
}

float OPMath::sqrtSEE( float fValI )
{
	if(!isOSSupportsSSE())	{
		return(::sqrt(fValI));
	}
	else	{
		float* pValTmp = new float(fValI);
		__asm	{
			mov esi, pValTmp;
			movups xmm0, [esi];
			rsqrtps xmm0, xmm0; reciprocal square root;
		}
		fValI = *pValTmp;
		if(pValTmp) { delete(pValTmp); }
		return(fValI);
	}
}

float OPMath::sinEx( float fAngI )
{
	//check the input value is the multiple of some special angles
	if(isZero<float>(fAngI))	{
		return(0);//sin(0)
	}
	float fAngPos = abs(fAngI);
	float fSinRet = 0;

	float fAngTst = fAngPos/(float)FW_PI;
	if(isInt<float>(fAngTst))	{
		return(0);
	}
	else	{
		fAngTst = fAngPos/(float)(FW_PI*0.5);
		if(isInt<float>(fAngTst))	{//check sin(FW_PI/2)
			UINT uNumMulti = (UINT)((fAngTst + 1)*0.5);
			if(uNumMulti%2 == 0)	{
				fSinRet = -1;
			}
			else	{
				fSinRet = 1;
			}
		}
		else	{
			fAngTst = fAngPos/(float)(FW_PI/3);
			if(isInt<float>(fAngTst))	{//check sin(FW_PI/3)
				//(1,2)(3,4)(5,6)...
				//  +1   -1   +1 ...
				UINT uNumMulti = (UINT)fAngTst;
				//turn to the number can multi 2
				if(uNumMulti%2 != 0)	{
					uNumMulti += 1;
				}
				//check able to divisible by 4(decide sign)
				if(uNumMulti%4 == 0)	{
					fSinRet = -sqrtSEE(3)*0.5f;
				}
				else	{
					fSinRet = sqrtSEE(3)*0.5f;
				}
			}
			else	{
				fAngTst = fAngPos/(float)(FW_PI/4);
				if(isInt<float>(fAngTst))	{//check sin(FW_PI/4)
					UINT uNumMulti = (UINT)((fAngTst + 1)*0.5);
					//turn to the number can multi 2
					if(uNumMulti%2 != 0)	{
						uNumMulti += 1;
					}
					//check able to divisible by 4(decide sign)
					if(uNumMulti%4 == 0)	{
						fSinRet = -sqrtSEE(2)*0.5f;
					}
					else	{
						fSinRet = sqrtSEE(2)*0.5f;
					}
				}
				else	{
					fAngTst = fAngPos/(float)(FW_PI/6);
					if(isInt<float>(fAngTst))	{//check sin(FW_PI/6)
						//the FW_PI/3 has split the 2*, 3* s' possible
						UINT uNumMulti = (UINT)((fAngTst + 1)*0.5);
						//check able to divisible by 2(decide sign)
						if(uNumMulti%2 == 0)	{
							fSinRet = -0.5f;
						}
						else	{
							fSinRet = 0.5f;
						}
					}
					else	{
						return((float)::sin(fAngI));
					}
				}
			}
		}
	}
	return((fAngI < 0) ? -fSinRet : fSinRet);
}

float OPMath::cosEx( float fAngI )
{
	//check the input value is the multiple of some special angles
	if(isZero<float>(fAngI))	{
		return(1);//cos(0)
	}
	float fAngPos = abs(fAngI);
	float fCosRet = 0;

	float fAngTst = fAngPos/(float)FW_PI;//check cos(FW_PI)
	if(isInt<float>(fAngTst))	{
		UINT uNumMulti = (UINT)fAngTst;
		if(uNumMulti%2 == 0)	{
			fCosRet = 1;
		}
		else	{
			fCosRet = -1;
		}
	}
	else	{
		fAngTst = fAngPos/(float)(FW_PI*0.5);//check cos(FW_PI/2)
		if(isInt<float>(fAngTst))	{
			return(0);
		}
		else	{
			fAngTst = fAngPos/(float)(FW_PI/3);//check cos(FW_PI/3)
			if(isInt<float>(fAngTst))	{
				// 1,(2,3),(4,5),(6,...
				//+1  -1    +1    -1...
				UINT uNumMulti = (UINT)fAngTst - 1;
				if(0 == uNumMulti)	{
					fCosRet = 0.5;
				}
				else	{
					//turn to the number can multi 2
					if(uNumMulti%2 != 0)	{
						uNumMulti += 1;
					}
					//check able to divisible by 4(decide sign)
					if(uNumMulti%4 == 0)	{
						fCosRet = 0.5;
					}
					else	{
						fCosRet = -0.5;
					}
				}
			}
			else	{
				fAngTst = fAngPos/(float)(FW_PI/4);//check cos(FW_PI/4)
				if(isInt<float>(fAngTst))	{
					UINT uNumMulti = (UINT)fAngTst;
					if(1 == uNumMulti)	{
						fCosRet = sqrtSEE(2)*0.5f;
					}
					else	{
						//find the number of it's group
						UINT uNumFnt = uNumMulti + 1;
						UINT uNumGrp = 0;
						if(uNumFnt%4 == 0)	{
							uNumGrp = uNumMulti + 2;
						}
						else	{
							uNumGrp = uNumMulti - 2;
						}
						//calculate out the sum
						UINT uSum = uNumMulti + uNumGrp;
						//divisible by 8, to turn to 1,2,3,4....
						uSum /= 8;
						//check able to divisible by 8(decide sign)
						if(uSum%2 == 0)	{
							fCosRet = sqrtSEE(2)*0.5f;
						}
						else	{
							fCosRet = -sqrtSEE(2)*0.5f;
						}
					}
				}
				else	{
					fAngTst = fAngPos/(float)(FW_PI/6);//check cos(FW_PI/6)
					if(isInt<float>(fAngTst))	{
						//1,(5,7),(11,13)...
						//+1  -1     +1
						//the FW_PI/3 has split the 2*, 3* s' possible
						UINT uNumMulti = (UINT)((fAngTst + 1)*0.5);
						//turn to the number can multi 2
						if(uNumMulti == 1)	{
							return(sqrtSEE(3)*0.5f);
						}
						else	{
							//(3,4),(6,7),(9,10),..
							//  -1    +1    -1
							//check able to divisible by 3(decide sign)
							if(uNumMulti%3 != 0)	{
								uNumMulti -= 1;
							}
							uNumMulti /=3;
							if(uNumMulti%2 == 0)	{
								fCosRet = sqrtSEE(3)*0.5f;
							}
							else	{
								fCosRet = -sqrtSEE(3)*0.5f;
							}
						}
					}
					else	{
						return((float)::cos(fAngI));
					}
				}
			}
		}
	}

	return((fAngI < 0) ? -fCosRet : fCosRet);
}

float OPMath::disBetweenPoi( const CPoint& poiAI, const CPoint& poiBI)
{
	float fX = poiAI.m_fX - poiBI.m_fX;
	float fY = poiAI.m_fY - poiBI.m_fY;
	float fZ = poiAI.m_fZ - poiBI.m_fZ;
	float fSun = fX*fX + fY*fY + fZ*fZ;
	return (fSun > 0) ? sqrtSEE(fSun) : 0;
}