#pragma once
#pragma warning(disable: 4244)
#include <map>
#include <vector>
#include <string>
#include "math.h"
#include "windows.h"
#include "OPTools.h"

using std::vector;
using std::string;

namespace OPTools
{
	//use the char to split the string
	//+_+: if use a char that string has not, 
	//     the function will return a array has the whole parameter string
	OP_MN_API std::vector<string> splitString( string, char);
	OP_MN_API std::vector<string> splitString( string, string);

	//get string between two identified signs
	//+_+:first happened signs' couple
	OP_MN_API string getStrBtwnTwo( string, char, char);

	//take out the class name from the name make by key word "typeid"
	string getClsNamFromTypeid(const char*);
	OP_MN_API char* turnToNormalStr(string);
	OP_MN_API string getFileTpFmPath(string strPathI);

	//from tail comparing two files' strings, allow include path
	//+_+:will not care about 'A' or 'a'
	OP_MN_API bool equalFileName( string, string);

	//Compare string ignoring the size of the write
	OP_MN_API bool cmpStrIgnSize( string, string);

	//collect custom words(a~z, A~Z, 0~9, _) from string(can define the first)
	//+_+:only one name or the first name will be return
	OP_MN_API string collectNamFromStr( string strSouI, UINT uLocBeg = 0);

	//get file name from path
	OP_MN_API string getFileNamFmPath(string strPathI);

	OP_MN_API char* WCharToChar(const WCHAR* lpwStri);
	OP_MN_API WCHAR* CharToWChar(const CHAR* pStri);
	OP_MN_API char toLowChar(char cValI);
	OP_MN_API char toUprChar(char cValI);
	OP_MN_API string toLowStr(string strValI);
	OP_MN_API string toUprStr(string strValI);
	//a~z, A~Z, 0~9, _
	OP_MN_API bool chkChar(char cValI);

	//check string is all number
	OP_MN_API bool chkNum(string strValI);

	//check two string equals(+_+: ignore case)
	OP_MN_API bool equalsStr( const string strAI, const string strBI);

	//print string, can process '\n' and '\t' not just print them
	OP_MN_API void printfEx( string strOutputI, ...);

	//clear all space in the string(+_+: only remove front and back's space)
	OP_MN_API string trimStr(string strValI);

	//get data from bracket(+_+: can't process bracket of bracket)
	OP_MN_API string getDatFmFirstBkt( string strValI, 
		std::pair<char, char> prBktI = std::pair<char, char>( '(', ')'));
	OP_MN_API string getDatFmLastBkt( string strValI, 
		std::pair<char, char> prBktI = std::pair<char, char>( '(', ')'));
	OP_MN_API std::vector<string> getDatFmBkt( string strValI,
		std::pair<char, char> prBktI = std::pair<char, char>( '(', ')'));

	//number and string's convert function
	template<typename T> TEMPLATE_API string numToStr( const T tNumI, int iFltLenI = -1)
	{
		string strRet = string();
		//zero test
		if(0 == tNumI)	{ return("0"); }
		T tNumBak = tNumI;
		//negative test
		if(tNumI < 0)
		{
			strRet += "-";
			tNumBak = -1*tNumBak;
		}
	
		//compose the integer
		DWORD dwOdd = 0;
		DWORD dwEntire = (DWORD)tNumBak;
		string strTmp = string();
		while(dwEntire)
		{
			dwOdd = dwEntire%10;
			strTmp += (char)(dwOdd + +48);
			dwEntire /= 10;
		}
	
		//swap the head and tail
		for( int i = strTmp.length() - 1; i >= 0; --i)
		{
			strRet += strTmp[i];
		}
	
		//float test
		if((DWORD)tNumBak != tNumBak)
		{
			if((DWORD)tNumBak == 0)	{ strRet += '0'; }
			T tFloat = tNumBak - (DWORD)tNumBak;
			T tEntire = tFloat;
			strTmp = "";
			int iFlt0dd = 0;
			while(tEntire)
			{
				if(iFltLenI > -1)
				{
					if(iFlt0dd < iFltLenI)	{ ++iFlt0dd; }
					else					{ break; }
				}
				tEntire *= 10;
				dwOdd = (DWORD)tEntire;
				strTmp += (char)(dwOdd + +48);
				tEntire -= (T)dwOdd;
			}
		
			//add the point
			if(iFltLenI != 0)
			{
				strRet += ".";
			}
		
			//swap the head and tail
			for( UINT i = 0; i < strTmp.length(); ++i)
			{
				strRet += strTmp[i];
			}
		}

		return strRet;
	}

	template<typename T> TEMPLATE_API T strToNum(const string strNumI)
	{
		//backup input parameter
		string strBkUp(strNumI);

		//negative test
		int iSign = 1;
		if(strNumI[0] == '-')
		{
			iSign = -1;
			strBkUp = strNumI.substr(1);
		}
	
		//float test
		string strIntPrt = "";
		string strFltPrt = "";
		UINT uLocPoi = strBkUp.find(".");
		if(string::npos != uLocPoi)
		{
			strIntPrt = strBkUp.substr( 0, uLocPoi);
			strFltPrt = strBkUp.substr(uLocPoi + 1);
		}
		else
		{
			strIntPrt = strBkUp;
		}

		//take out the integer part
		T tValRet = 0;
		for( UINT i = 0, j = strIntPrt.length()-1; i < strIntPrt.length(); ++i, --j)
		{
			tValRet += (strIntPrt[i] - 48)*(DWORD)pow( (double)10, (int)j);
		}
	
		//take out the float part
		if(!strFltPrt.empty())
		{
			int j = 1;
			for( UINT i = 0; i < strFltPrt.length(); ++i, ++j)
			{
				tValRet += (strFltPrt[i] - 48)*pow( (double)10, -j);
			}
		}

		return tValRet*iSign;
	}

};