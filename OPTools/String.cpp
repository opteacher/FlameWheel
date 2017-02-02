#pragma once
#include "stdafx.h"
#include "String.h"
#include "math.h"
#include "File.h"
#include "stdarg.h"

using namespace std;
using namespace OPTools;

OP_MN_API CHAR* OPTools::WCharToChar(const WCHAR* lpwStri)
{
	int nLen = wcslen(lpwStri) + 1;
	char* pstrChar = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, lpwStri, nLen, pstrChar, 2*nLen, NULL, NULL);
	return(pstrChar);
}

OP_MN_API WCHAR* OPTools::CharToWChar(const CHAR* pStri)
{
	size_t uLenStr	= strlen(pStri);
	int len = MultiByteToWideChar( CP_ACP, 0, pStri, uLenStr, NULL, 0);
	WCHAR* lpwStro	= new wchar_t[len + 1]; 
	MultiByteToWideChar( CP_ACP, 0, pStri, uLenStr, lpwStro, len);
	lpwStro[len]	= '\0';
	return(lpwStro);
}

OP_MN_API vector<string> OPTools::splitString( string strTargetI, char cSplitCharI)
{
	vector<string> vecRet	= vector<string>();
	string::size_type posFind	= 0;
	string::size_type posBeg	= 0;
	UINT uNumAcc = 0;
	while((posFind = strTargetI.find_first_of( cSplitCharI, posFind)) != string::npos)
	{
		vecRet.push_back(strTargetI.substr( posBeg, posFind - posBeg));
		posFind++;	uNumAcc++;
		posBeg	= posFind;
	}
	if(uNumAcc)	{ vecRet.push_back(strTargetI.substr( posBeg, strTargetI.length() - posBeg)); }

	return vecRet;
}

OP_MN_API vector<string> OPTools::splitString( string strTargetI, string strSplitStrI)
{
	vector<string> vecRet	= vector<string>();
	string::size_type posFind	= 0;
	string::size_type posBeg	= 0;
	UINT uNumAcc = 0;
	while((posFind = strTargetI.find_first_of( strSplitStrI, posFind)) != string::npos)
	{
		vecRet.push_back(strTargetI.substr( posBeg, posFind - posBeg));
		posFind += strSplitStrI.length();	uNumAcc++;
		posBeg	= posFind;
	}
	if(uNumAcc)	{ vecRet.push_back(strTargetI.substr( posBeg, strTargetI.length() - posBeg)); }

	return vecRet;
}

OP_MN_API string OPTools::getStrBtwnTwo( string strSouI, char cSgnAI, char cSgnBI)
{
	streamoff uLocA = strSouI.find(cSgnAI);
	streamoff uLocB = strSouI.find(cSgnBI);
	UINT uSzTake = (UINT)abs(uLocA - uLocB);
	string strRet = "";
	if(uLocA < uLocB)
	{
		strRet = strSouI.substr( (UINT)uLocA + 1, uSzTake - 1);
	}
	else
	{
		strRet = strSouI.substr( (UINT)uLocB + 1, uSzTake - 1);
	}

	return strRet;
}

OP_MN_API char* OPTools::turnToNormalStr( string strValI )
{
	char* pStrRet	= new char[strValI.length() + 2];
	_memccpy( pStrRet, strValI.c_str(), '\0', strlen(strValI.c_str()) + 1);
	pStrRet[strValI.length()] = '\0';
	return pStrRet;
}

OP_MN_API string OPTools::getFileTpFmPath(string strPathI)
{
	UINT uLocPoi = strPathI.find_last_of('.');
	if(uLocPoi != string::npos)	{ return strPathI.substr(uLocPoi + 1); }
	else						{ return ""; }
}

OP_MN_API bool OPTools::equalFileName( string strFileANamI, string strFileBNamI)
{
	int shortLength	= min( strFileANamI.length(), strFileBNamI.length());
	for( int i = strFileANamI.length() - 1, j = strFileBNamI.length() - 1; \
		shortLength > 0; --shortLength, --i, --j)
	{
			char cATmp	= toLowChar(strFileANamI[i]);
			char cBTmp	= toLowChar(strFileBNamI[j]);
			if(cATmp != cBTmp)	{ return false; }
	}
	return true;
}

OP_MN_API bool OPTools::cmpStrIgnSize( string strAI, string strBI)
{
	if(strAI.length() != strBI.length())	{ return false; }
	for( UINT i = 0; i < strAI.length(); ++i)
	{
		char cA = toLowChar(strAI[i]);
		char cB = toLowChar(strBI[i]);
		if (cA != cB)	{ return false; }
	}
	return true;
}

OP_MN_API string OPTools::collectNamFromStr( string strSouI, UINT uLocBeg /*= 0*/ )
{
	string strRet  = "";
	string strCut = strSouI.substr(uLocBeg);
	vector<string> aStrParts = splitString( strCut, ' ');
	for( vector<string>::iterator iter = aStrParts.begin(); iter != aStrParts.end(); ++iter)
	{
		string strTmp = *iter;
		for( UINT uIndex = 0; uIndex < strTmp.length(); ++uIndex)
		{
			if((strTmp[uIndex] <= 'Z' && strTmp[uIndex] >= 'A') \
			|| (strTmp[uIndex] <= 'z' && strTmp[uIndex] >= 'a') \
			|| (strTmp[uIndex] <= '9' && strTmp[uIndex] >= '0') \
			|| (strTmp[uIndex] == '_'))
			{
				strRet += strTmp[uIndex];
			}
			else
			{ break; }
		}

		if(!strRet.empty())	{ break; }
	}
	return strRet;
}

OP_MN_API string OPTools::getFileNamFmPath(string strPathI)
{
	//first use '\\' to split the path
	vector<string> vecFolder = OPTools::splitString( strPathI, '\\');
	if(vecFolder.empty())
	{
		//if empty, use '/' to split string again
		vecFolder = OPTools::splitString( strPathI, '/');
		if(vecFolder.empty())
		{
			return strPathI;
		}
		else
		{
			//use the last string to return
			return vecFolder.back();
		}
	}
	else
	{
		return vecFolder.back();
	}
}

OP_MN_API char OPTools::toLowChar( char cValI )
{
	return (cValI >= 'A' && cValI <= 'Z') ? char(cValI + 32) : cValI;
}

OP_MN_API char OPTools::toUprChar(char cValI)
{
	return (cValI >= 'a' && cValI <= 'z') ? char(cValI - 32) : cValI;
}

OP_MN_API string OPTools::toLowStr(string strValI)
{
	string strValRet = "";
	for( UINT i = 0; i < strValI.size(); ++i)
	{ strValRet += toLowChar(strValI[i]); }

	return strValRet;
}

OP_MN_API string OPTools::toUprStr(string strValI)
{
	string strValRet = "";
	for( UINT i = 0; i < strValI.size(); ++i)
	{ strValRet += toUprChar(strValI[i]); }

	return strValRet;
}

OP_MN_API bool OPTools::chkChar(char cValI)
{
	if((cValI <= 'Z' && cValI >= 'A') \
	|| (cValI <= 'z' && cValI >= 'a') \
	|| (cValI <= '9' && cValI >= '0') \
	|| (cValI == '_'))
	{ return true; }
	else
	{ return false; }
}

OP_MN_API bool OPTools::chkNum(string strValI)
{
	UINT uNumPoi = 0;
	for( UINT i = 0; i < strValI.length(); ++i)
	{
		if(strValI[i] == '-')
		{
			if(i != 0)	{ return false; }
			else		{ continue; }
		}
		else if(strValI[i] == '.')
		{
			if(uNumPoi == 0)	{ uNumPoi = 1; }
			else 				{ return false; }
		}
		else if(strValI[i] < '0' || strValI[i] > '9')	{ return false; }
	}
	return true;
}

OP_MN_API bool OPTools::equalsStr( const string strAI, const string strBI)
{
	if(strAI.length() != strBI.length())	{ return false; }

	int iCharDif = 'a' - 'A';
	for( UINT i = 0; i < strAI.length(); ++i)
	{
		if(strAI[i] != strBI[i]
		&& strAI[i] + iCharDif != strBI[i]
		&& strAI[i] != strBI[i] + iCharDif)
		{
			return false;
		}
	}

	return true;
}

OP_MN_API void OPTools::printfEx( string strOutputI, ...)
{
	if(strOutputI.empty())	{ return; }

	UINT uLocFnt = 0;
	UINT uLength = strOutputI.length();
	va_list pArg = NULL;
	va_start( pArg, strOutputI);
	for( UINT i = 0; i < uLength; ++i)
	{
		char cTmp = strOutputI[i];
		if(strOutputI[i] == '\\' && i != uLength - 1)
		{
			string strTmp = strOutputI.substr( uLocFnt, i - uLocFnt);
			::printf(strTmp.c_str());
			char cSpc = strOutputI[i + 1];
			switch(cSpc)
			{
			case 'n':
			case 'N':
				::printf("\n");
				break;
			case 't':
			case 'T':
				::printf("\t");
				break;
			case '\"':
				::printf("\"");
				break;
			case '\'':
				::printf("\'");
				break;
			default:
				strTmp = '\\' + cSpc;
				::printf(strTmp.c_str());
			}
			i += 1;
			uLocFnt = i + 1;
		}
		else if(strOutputI[i] == '%' && i != uLength - 1)
		{
			string strTmp = strOutputI.substr( uLocFnt, i - uLocFnt);
			::printf(strTmp.c_str());
			char cSpc = strOutputI[i + 1];
			switch(cSpc)
			{
			case 's':
			case 'S':
				{
					string strPam = va_arg( pArg, char*);
					::printf(strPam.c_str());
					break;
				}
			default:
				strTmp = '\\' + cSpc;
				::printf(strTmp.c_str());
			}
			i += 1;
			uLocFnt = i + 1;
		}
	}

	::printf(strOutputI.substr(uLocFnt).c_str());
}

OP_MN_API string OPTools::trimStr(string strValI)
{
	if(strValI.empty())	{ return strValI; }

	string strRet = strValI;
	while(strRet.front() == ' ')
	{
		strRet = strRet.substr(1);
	}

	while(strRet.back() == ' ')
	{
		strRet = strRet.substr( 0, strRet.length() - 1);
	}

	return strRet;
}

OP_MN_API string OPTools::getDatFmFirstBkt( string strValI, std::pair<char, char> prBktI)
{
	UINT uLocFst = strValI.find_first_of(prBktI.first);
	UINT uLocScd = strValI.find_first_of(prBktI.second);

	if(uLocFst != string::npos
	&& uLocScd != string::npos
	&& uLocFst < uLocScd)
	{
		UINT uLocBeg = uLocFst + 1;
		UINT uSzCut = uLocScd - uLocBeg;
		return strValI.substr( uLocBeg, uSzCut);
	}
	else if(uLocFst != string::npos)
	{
		return strValI.substr(uLocFst + 1);
	}
	else
	{
		return "";
	}
}

OP_MN_API string OPTools::getDatFmLastBkt( string strValI, std::pair<char, char> prBktI)
{
	UINT uLocFst = strValI.find_last_of(prBktI.first);
	UINT uLocScd = strValI.find_last_of(prBktI.second);

	if(uLocFst != string::npos
	&& uLocScd != string::npos
	&& uLocFst < uLocScd)
	{
		UINT uLocBeg = uLocFst + 1;
		UINT uSzCut = uLocScd - uLocBeg;
		return strValI.substr( uLocBeg, uSzCut);
	}
	else if(uLocFst != string::npos)
	{
		return strValI.substr(uLocFst + 1);
	}
	else
	{
		return "";
	}
}

OP_MN_API vector<string> OPTools::getDatFmBkt( string strValI, std::pair<char, char> prBktI)
{
	string strTmpI = strValI;
	UINT uLocLft = strTmpI.find_first_of(prBktI.first);
	UINT uLocRgt = strTmpI.find_first_of(prBktI.second);
	vector<string> ayDatRet;
	while(uLocLft != string::npos)
	{
		if(uLocRgt != string::npos)
		{
			UINT uLocBeg = uLocLft + 1;
			UINT uSzCut = uLocRgt - uLocBeg;
			string strInst = strTmpI.substr( uLocBeg, uSzCut);
			ayDatRet.push_back(strInst);
			//start from the next location of the right
			strTmpI = strTmpI.substr(uLocRgt + 1);
		}
		else
		{
			//has the first but no second, cut all of the rest
			//and return
			string strInst = strTmpI.substr(uLocLft + 1);
			ayDatRet.push_back(strInst);
			return ayDatRet;
		}

		uLocLft = strTmpI.find_first_of(prBktI.first);
		uLocRgt = strTmpI.find_first_of(prBktI.second);
	}

	return ayDatRet;
}
