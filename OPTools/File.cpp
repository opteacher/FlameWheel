#pragma once
#include "stdafx.h"
#include "File.h"
#include <bitset>
#include <fstream>
#include "String.h"
#include "mmsystem.h"
#pragma comment( lib, "winmm.lib")

using namespace std;
using namespace OPTools;

string timeToString( STime timeI)
{
	string strRet	= string();
	strRet	= "[";
	strRet	+= numToStr<int>(timeI.m_iYear);
	strRet	+= "/";
	strRet	+= numToStr<int>(timeI.m_iMonth);
	strRet	+= "/";
	strRet	+= numToStr<int>(timeI.m_iDay);
	strRet	+= "--";
	strRet	+= numToStr<int>(timeI.m_iHour);
	strRet	+= ":";
	strRet	+= numToStr<int>(timeI.m_iMinute);
	strRet	+= ":";
	strRet	+= numToStr<int>(timeI.m_iSecond);
	strRet	+= "--";
	strRet	+= numToStr<DWORD>(timeI.m_dwMSecond);
	strRet	+= "]";

	return(string(strRet));
}

string timeToString( const SYSTEMTIME& refTimeI)
{
	string strRet	= string();
	strRet	= "[";
	strRet	+= numToStr<WORD>(refTimeI.wYear);
	strRet	+= "/";
	strRet	+= numToStr<WORD>(refTimeI.wMonth);
	strRet	+= "/";
	strRet	+= numToStr<WORD>(refTimeI.wDay);
	strRet	+= "--";
	strRet	+= numToStr<WORD>(refTimeI.wHour);
	strRet	+= ":";
	strRet	+= numToStr<WORD>(refTimeI.wMinute);
	strRet	+= ":";
	strRet	+= numToStr<WORD>(refTimeI.wSecond);
	strRet	+= "--";
	strRet	+= numToStr<WORD>(refTimeI.wMilliseconds);
	strRet	+= "]";

	return(string(strRet));
}

void OPTools::writeLog( string strFileI, string strLogI, STime TimeI )
{
	string strTime;
	if(TimeI.isUninit())
	{
		//uninitialized, get current system time
		SYSTEMTIME timeFromSys;
		::GetSystemTime(&timeFromSys);
		strTime	= timeToString(timeFromSys);
	}
	else
	{
		if(!TimeI.isLegal())
		{
			throw std::runtime_error("time isn't legal");
		}
		strTime	= timeToString(TimeI);
	}

	ofstream ofStrm( strFileI.c_str(), std::ios_base::out | std::ios_base::app);
	ofStrm<<strTime<<":"<<strLogI<<std::endl;
	ofStrm.close();
}

void OPTools::writeLog( string strFileI, string strLogI, int iHourI, int iMinuteI, int iSecondI )
{
	SYSTEMTIME timeSys;
	::GetSystemTime(&timeSys);
	STime timeTmp;
	timeTmp.m_iYear = timeSys.wYear;
	timeTmp.m_iMonth = timeSys.wMonth;
	timeTmp.m_iDay = timeSys.wDay;
	timeTmp.m_iHour = iHourI;
	timeTmp.m_iMinute = iMinuteI;
	timeTmp.m_iSecond = iSecondI;
	timeTmp.m_dwMSecond = timeSys.wMilliseconds;

	writeLog( strFileI, strLogI, timeTmp);
}

void OPTools::writeLog( string strFileI, string strLogI, int iSecondI, DWORD dwMSecondI )
{
	SYSTEMTIME timeSys;
	::GetSystemTime(&timeSys);
	STime timeTmp;
	timeTmp.m_iYear	= timeSys.wYear;
	timeTmp.m_iMonth	= timeSys.wMonth;
	timeTmp.m_iDay	= timeSys.wDay;
	timeTmp.m_iHour	= timeSys.wHour;
	timeTmp.m_iMinute	= timeSys.wMinute;
	timeTmp.m_iSecond	= iSecondI;
	timeTmp.m_dwMSecond	= dwMSecondI;

	writeLog( strFileI, strLogI, timeTmp);
}

void OPTools::writeExcLog( string strFileNamI, string strHpnFlI, string strLnI, string strExcIdI, string strDscbI)
{
	string strBeWrite = string();
	strBeWrite += "Local file: ";
	strBeWrite += strHpnFlI;
	strBeWrite += "; Happened line: ";
	strBeWrite += strLnI;
	strBeWrite += "; Exception ID: ";
	strBeWrite += strExcIdI;
	strBeWrite += "; Exception describe: ";
	strBeWrite += strDscbI;
	writeLog( strFileNamI, strBeWrite);
}

CCallCatch* CCallCatch::s_pInstance = NULL;

CCallCatch::CCallCatch(): m_strTstFile("FWCallLog.log")
{
	ofstream ofStrm( m_strTstFile, std::ios_base::out | std::ios_base::trunc);
	ofStrm.close();
}

void CCallCatch::funcTstBeg( string strFileI, string strFuncI)
{
	string strFlNam = OPTools::getFileNamFmPath(strFileI);
	//take out the class name from the file name
	//take away the suffix
	vector<string> vecStr = OPTools::splitString( strFlNam, '.');
	if(2 != vecStr.size())
	{
		throw std::runtime_error("check function \'OPTools::getFileNamFmPath\' and \'OPTools::splitString\'");
		//return;
	}

	SInfoFun infoFun;
	infoFun.m_strClsNam = vecStr.front();//+_+: has no prefix
	infoFun.m_strFunNam = strFuncI;

	//store the time
	infoFun.m_dwStartTime = ::timeGetTime();

	//write into test file
	SYSTEMTIME timeFromSys;
	::GetSystemTime(&timeFromSys);
	string strTime = timeToString(timeFromSys);
	ofstream ofStrm( m_strTstFile.c_str(), std::ios_base::out | std::ios_base::app);
	DWORD dwCurThdId = ::GetCurrentThreadId();
	//to show the level of calling, use '#' as the prefix
	ofStrm<<endl;
	map<DWORD, deque<SInfoFun>>::iterator iterFindDqu = m_mpCallDeque.find(dwCurThdId);
	if(m_mpCallDeque.end() != iterFindDqu && !iterFindDqu->second.empty())
	{
		UINT uDepCalling = iterFindDqu->second.size();
		for( UINT uI = 0; uI < uDepCalling; ++uI)
		{
			ofStrm<<"#";
		}
	}
	ofStrm<<"Function: "<<infoFun.m_strFunNam<<"\t belong Class: "<<infoFun.m_strClsNam<<"\t start time: "<<strTime;

	//store the location of the function calling in log
	//let the end know where to insert running time
	infoFun.m_uLocInLog = ofStrm.tellp();

	ofStrm.close();

	//push into the calling deque
	if(m_mpCallDeque.empty() || m_mpCallDeque.end() == iterFindDqu)
	{
		//map has no such thread, insert
		deque<SInfoFun> dquTmp = deque<SInfoFun>();
		dquTmp.push_back(infoFun);
		m_mpCallDeque.insert(map<DWORD, deque<SInfoFun>>::value_type( dwCurThdId, dquTmp));
	}
	else
	{
		//already existed, just do push the calling
		iterFindDqu->second.push_back(infoFun);
	}
			
	//store the current function calling 
	map<DWORD, string>::iterator iterFindStr = m_mpCurFun.find(dwCurThdId);
	if(m_mpCurFun.empty() || m_mpCurFun.end() == iterFindStr)
	{
		m_mpCurFun.insert(map<DWORD, string>::value_type( dwCurThdId, infoFun.m_strFunNam));
	}
	else
	{
		m_mpCurFun[dwCurThdId] = infoFun.m_strFunNam;
	}
}

void CCallCatch::funcTstEnd()
{
	//take out the top info from the deque
	DWORD dwCurThdId = ::GetCurrentThreadId();
	map<DWORD, deque<SInfoFun>>::iterator iterFindDqu = m_mpCallDeque.find(dwCurThdId);
	if(m_mpCallDeque.empty() || m_mpCallDeque.end() == iterFindDqu)
	{
		//map has no such thread, but function test end be called, check others thread
		throw std::runtime_error("no such calling");
	}
	else
	{
		//take out the top function calling
		SInfoFun infoFun = SInfoFun(iterFindDqu->second.back());
		iterFindDqu->second.pop_back();

		//write into the function test log file
		SYSTEMTIME timeFromSys;
		::GetSystemTime(&timeFromSys);
		string strTime = timeToString(timeFromSys);
		//calculate total time
		string strTotalTime = OPTools::numToStr<DWORD>(::timeGetTime() - infoFun.m_dwStartTime);
		string strDat = "\t end time: " + strTime + "\t total: " + strTotalTime;
		//write insert file
		writeInstFile( m_strTstFile, strDat, infoFun.m_uLocInLog);
	}
}

map<string, MAP_STR> getDatFromIniFile(string strFileI)
{
	//use read mode to open the ini file
	ifstream ifStrm(strFileI.c_str());
	if(!ifStrm.is_open())
	{
		throw std::runtime_error("E1307"/*EXC_FILE*/);
	}

	//read every line from file
	vector<string> vecLineDat	= vector<string>();
	while(!ifStrm.eof())
	{
		string strTmp;
		getline( ifStrm, strTmp, '\n');
		if(0 < strTmp.length())
		{
			vecLineDat.push_back(strTmp);
		}
	}

	//build section<key, value> map
	map<string, MAP_STR> mpRet = map<string, MAP_STR>();
	string strSecTmp	= string();
	map<string, string> mpKeyValTmp	= map<string, string>();
	for( vector<string>::iterator iter = vecLineDat.begin(); vecLineDat.end() != iter; ++iter)
	{
		//if the line is empty, continue
		if("" == (*iter))	{ continue; }

		//the words be include by "[]", means section
		if((*iter)[0] == '[' && (*iter)[iter->size() - 1] == ']')
		{
			//take out the section
			string strTmp = iter->substr( 1, iter->length() - 2);

			//if empty, throw exception
			if(strTmp.length() == 0)
			{
				throw std::runtime_error("E1309"/*EXC_UNEXIST*/);
			}

			//update current section
			if(strSecTmp.length() == 0)
			{
				strSecTmp = strTmp;
			}
			else
			{
				//insert <section, <key, value>>
				mpRet.insert(map<string, MAP_STR>::value_type( strSecTmp, mpKeyValTmp));
				//update section
				strSecTmp	= strTmp;
				//reset key value map
				mpKeyValTmp.clear();
			}
		}
		else
		{
			//not section, means key and value set, use "=" to split
			vector<string> strSplitResu	= splitString( *iter, '=');
			if(strSplitResu.size() != 2)
			{
				throw std::runtime_error("error key and value set exist in " + strFileI);
			}

			//insert
			mpKeyValTmp.insert(map<string, string>::value_type( strSplitResu.front(), strSplitResu.back()));
		}
	}

	if(strSecTmp.length() > 0)
	{
		mpRet.insert(map<string, MAP_STR>::value_type( strSecTmp, map<string, string>()));
	}

	if(mpKeyValTmp.size() != 0)
	{
		mpRet[strSecTmp] = mpKeyValTmp;
	}

	ifStrm.close();
	return mpRet;
}

void writeIntoIni( string strFileI, map<string, MAP_STR> mpDatI, bool bCoverWriteI)
{
	ofstream ofStrm;
	//whether to enable cover mode
	if(bCoverWriteI)
	{
		ofStrm.open( strFileI.c_str(), ios_base::trunc | ios_base::out);
	}
	else
	{
		ofStrm.open( strFileI.c_str(), ios_base::out);
	}

	//convert the map data to string for writting into file
	for( map<string, MAP_STR>::iterator iter = mpDatI.begin(); iter != mpDatI.end(); ++iter)
	{
		//use [] to include the section and write
		string strTmp	= string("[");
		strTmp	+= iter->first;
		strTmp	+= "]";
		ofStrm<<strTmp<<endl;

		//key value map isn't empty, use "=" to splite and write
		if(iter->second.size() != 0)
		{
			map<string, string> mpKeyValTmp	= iter->second;
			for( map<string, string>::iterator iterA = mpKeyValTmp.begin(); iterA != mpKeyValTmp.end(); ++iterA)
			{
				if(iterA->first.length() > 0 && iterA->second.length() > 0)
				{
					ofStrm<<iterA->first<<"="<<iterA->second<<endl;
				}
			}
		}
	}

	ofStrm.close();
}

OP_MN_API void OPTools::writeInstFile( string strFileI, string strDatI, std::streamoff uLocInsI)
{
	ofstream ofStrm( strFileI.c_str(), ios_base::in | ios_base::ate);
	ifstream ifStrm( strFileI.c_str(), ios_base::in | ios_base::binary);
	//locate the insert position
	ofStrm.seekp( uLocInsI, ios_base::beg);
	ifStrm.seekg(uLocInsI);

	UINT uIndex = 0;
	string strDatTmp = strDatI;
	bool bInEnter = false;
	while(!ifStrm.eof())
	{
		streamoff uLocTmp = ifStrm.tellg();
		//get out the char at the inserting position
		char cTmp = ifStrm.get();
		if(-1 == cTmp)
		{ break; }

		if(0x0D == cTmp)
		{
			if(!bInEnter)
			{ bInEnter = true; }

			continue;
		}
		if(bInEnter && 0x0A == cTmp)
		{ bInEnter = false; }

		//insert the char of the data string
		ofStrm<<strDatTmp[uIndex];
		//replace it with the char token from file
		strDatTmp[uIndex] = cTmp;

		++uIndex;
		if(uIndex >= strDatI.size())
		{ uIndex = 0; }
	}

	//swap the data string and insert
	ofStrm<<strDatTmp.substr(uIndex).c_str();
	ofStrm<<strDatTmp.substr( 0, uIndex).c_str();

	ofStrm.close();
	ifStrm.close();
}

OP_MN_API void OPTools::writeKVIntoIni( string strFileI, string strSecI, string strNodeI, string strNdeValI, bool bCoverWriteI)
{
	//check given node's name and value is ilegal
	if((strNodeI.length() > 0 && strNdeValI.length() == 0)
	|| (strNodeI.length() == 0 && strNdeValI.length() > 0))	{ return; }

	//read all data from the ini file
	map<string, MAP_STR> mpStrt;
	mpStrt = getDatFromIniFile(strFileI);
		
	//check this section is exist
	bool bInserted	= false;
	for( map<string, MAP_STR>::iterator iter = mpStrt.begin(); iter != mpStrt.end(); ++iter)
	{
		if(strSecI == iter->first)
		{
			bInserted	= true;
			MAP_STR& refMpKVInFl = iter->second;
			if(refMpKVInFl.end() != refMpKVInFl.find(strNodeI))
			{
				//existed, update value
				refMpKVInFl[strNodeI] = strNdeValI;
			}
			else
			{
				//unexisted, just insert the node
				refMpKVInFl.insert(MAP_STR::value_type( strNodeI, strNdeValI));
			}
		}
	}
	
	//not exist the section, create
	if(!bInserted)
	{
		map<string, string> mpTmp	= map<string, string>();
		mpTmp.insert(map<string, string>::value_type( strNodeI, strNdeValI));
		mpStrt.insert(map<string, MAP_STR>::value_type( strSecI, mpTmp));
	}
	
	//write into the ini file
	writeIntoIni( strFileI, mpStrt, bCoverWriteI);
	
	//check inserted key and value is already exist
	if(strNdeValI != readOutIniFile( strFileI, strSecI, strNodeI))	{ return; }
}

OP_MN_API void OPTools::writeKVMapIntoIni( string strFileI, string strSecI, MAP_STR mpKVI, bool bCoverWriteI )
{
	//read all data from the ini file
	map<string, MAP_STR> mpStrt;
	mpStrt = getDatFromIniFile(strFileI);
		
	//check this section is exist
	bool bInserted	= false;
	for( map<string, MAP_STR>::iterator iter = mpStrt.begin(); iter != mpStrt.end(); ++iter)
	{
		if(strSecI == iter->first)
		{
			bInserted	= true;
			//if existed, just insert the node
			MAP_STR& refMpKvInFl = iter->second;
			for( MAP_STR::iterator iterSub = mpKVI.begin(); iterSub != mpKVI.end(); ++iterSub)
			{
				if(refMpKvInFl.find(iterSub->first) != refMpKvInFl.end())
				{
					//existed, update the value with param
					refMpKvInFl[iterSub->first] = iterSub->second;
				}
				else
				{
					//unexisted, do insert
					refMpKvInFl.insert(MAP_STR::value_type( iterSub->first, iterSub->second));
				}
			}
		}
	}
	
	//not exist the section, create
	if(!bInserted)
	{
		mpStrt.insert(map<string, MAP_STR>::value_type( strSecI, mpKVI));
	}
	
	//write into the ini file
	writeIntoIni( strFileI, mpStrt, bCoverWriteI);
}

OP_MN_API string OPTools::readOutIniFile( string strFileI, string strSecI, string strNodeI )
{
	//read all data from ini file(as map style)
	map<string, MAP_STR> mpStrt	= getDatFromIniFile(strFileI);
	for( map<string, MAP_STR>::iterator iter = mpStrt.begin(); iter != mpStrt.end(); ++iter)
	{
		//find section
		if(strSecI == iter->first)
		{
			for( map<string, string>::iterator iterA = mpStrt[strSecI].begin(); iterA != mpStrt[strSecI].end(); ++iterA)
			{
				//find key
				if(strNodeI == iterA->first)
				{
					//return value
					return iterA->second;
				}
			}
		}
	}
	//unfound,  throw exception
	throw std::runtime_error("no such section or key in this ini file " + strFileI);
}

OP_MN_API MAP_STR OPTools::readOutAllDatFromSection( string strFileI, string strSecI)
{
	//read all data from ini file
	map<string, MAP_STR> mpStrt	= map<string, MAP_STR>();
	mpStrt	= getDatFromIniFile(strFileI);

	//find given section and return
	map<string, MAP_STR>::iterator iterFnd = mpStrt.find(strSecI);
	if(iterFnd != mpStrt.end())
	{
		return iterFnd->second;
	}
	else
	{
		return map<string, string>();
	}
}

OP_MN_API VEC_STR OPTools::readOutAllSection( string strFileI )
{
	VEC_STR vecRet;
	//read all data from ini file
	map<string, MAP_STR> mpStrt	= map<string, MAP_STR>();
	mpStrt	= getDatFromIniFile(strFileI);

	//build section vector
	for( map<string, MAP_STR>::iterator iter = mpStrt.begin(); \
		iter != mpStrt.end(); ++iter)
	{
		vecRet.push_back(iter->first);
	}

	return vecRet;
}

OP_MN_API MAP_STR OPTools::readOutAllKeyValue(string strFileI)
{
	MAP_STR mpRet;
	//read all data from ini file
	map<string, MAP_STR> mpStrt	= map<string, MAP_STR>();
	mpStrt	= getDatFromIniFile(strFileI);

	//build section vector
	for( map<string, MAP_STR>::iterator iter = mpStrt.begin(); \
		iter != mpStrt.end(); ++iter)
	{
		for( MAP_STR::iterator iterT = iter->second.begin(); \
			iterT != iter->second.end(); ++iterT)
		{
			mpRet.insert(*iterT);
		}
	}

	return mpRet;
}

OP_MN_API void OPTools::writeInDatFile( string strFileI, string strDatInpI)
{
	//ofen the file with defined mode
	ofstream ofstm = ofstream();
	ofstm.open( strFileI.c_str(), ofstream::out | ofstream::binary | ofstream::trunc);
	if(!ofstm.is_open())	{ return; }

	//generate mark code index
	string strIndex = "";
	UINT uSzData = MASK_SIZE * 8;//the mark map size
	for( UINT i = 0; i < uSzData; ++i)
	{
		if(rand() % 2)
		{ strIndex += "1"; }
		else
		{ strIndex += "0"; }
	}

	//build index vector
	vector<BYTE> vecIndMap = vector<BYTE>();
	for( UINT j = 0; j < uSzData; j += 8)
	{
		BYTE byIndTmp = (BYTE)std::bitset<8>( strIndex, j, 8).to_ulong();

		//insert into the map
		vecIndMap.push_back(byIndTmp);
		//write into file
		ofstm.write( (char*)&byIndTmp, sizeof(BYTE));
	}

	//compose with every char of the data
	//and input into the file
	UINT sztIndDat = 0;
	vector<BYTE>::size_type sztIndex = 0;
	while(sztIndDat < strDatInpI.size())
	{
		//use XOR to calculate every char
		char cValTmp = strDatInpI[sztIndDat] ^ vecIndMap[sztIndex];
		//write into the file
		ofstm.write( (char*)&cValTmp, sizeof(char));
		++sztIndDat;
		//review the map, if locate to the end, return to the head
		if(sztIndex < MASK_SIZE - 1)
		{ ++sztIndex; }
		else
		{ sztIndex = 0; }
	}

	ofstm.close();
}

OP_MN_API string OPTools::readOutDatFile( string strFileI, BYTE* pMarkMpO)
{
	ifstream ifstm( strFileI.c_str(), ofstream::in | ofstream::binary);
	if(!ifstm.is_open())	{ return ""; }
	
	//read index map from the file: 128*sizeof(byte)
	vector<BYTE> aMarkTb = vector<BYTE>();
	for( UINT i = 0; i < MASK_SIZE; ++i)
	{
		BYTE byTmp = 0;
		//read every byte
		ifstm.read( (char*)&byTmp, sizeof(BYTE));
		//add to the mark map
		aMarkTb.push_back(byTmp);

		//output the mark map
		if(NULL != pMarkMpO)
		{
			pMarkMpO[i] = byTmp;
		}
	}

	//read data
	string strData = "";
	vector<BYTE>::size_type sztIndMp = 0;
	while(!ifstm.eof())
	{
		char cValTmp;
		//read every character
		ifstm.read( &cValTmp, sizeof(char));
		//build source string data
		strData += (BYTE)cValTmp ^ aMarkTb[sztIndMp];
		
		if(sztIndMp < MASK_SIZE - 1)
		{ ++sztIndMp; }
		else
		{ sztIndMp = 0; }
	}

	//cut the end word, can't understand the end word will be read
	strData = strData.substr( 0, strData.size() - 1);

	ifstm.close();
	return(strData);
}

OP_MN_API vector<string> OPTools::readSameTypeFromPath( string strNamPathI, string strTypeFile )
{
	if(strNamPathI.substr( strNamPathI.length() - 1, strNamPathI.length() - 1) != "\\")
		strNamPathI	+= "\\";

	//search files
	string strAllFile = strNamPathI + "*." + strTypeFile;
	WIN32_FIND_DATA datFindFile;
	HANDLE hResult = ::FindFirstFile( CharToWChar( strAllFile.c_str()), \
		(LPWIN32_FIND_DATAW)&datFindFile);
	vector<string> vecNamFile;
	do
	{
		if(hResult != INVALID_HANDLE_VALUE)
		{
			if(!(datFindFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//not dir
				string strNamFile(WCharToChar(datFindFile.cFileName));
				strNamFile	= strNamPathI + strNamFile;
				vecNamFile.push_back(strNamFile);
			}
		}
	}
	while(::FindNextFile( hResult, (LPWIN32_FIND_DATAW)&datFindFile));
	::FindClose(hResult);
	//search dir
	struct
	{
		unsigned    attrib;
		__time64_t  time_create;    // -1 for FAT file systems 
		__time64_t  time_access;    // -1 for FAT file systems 
		__time64_t  time_write;
		_fsize_t    size;
		wchar_t     name[260];
	} datFindDir;
	string strAllDir	= strNamPathI + "*";
	WCHAR* pWStrTmp	= CharToWChar(strAllDir.c_str());
	long fHdlRes = ::_wfindfirst( pWStrTmp, (_wfinddata64i32_t*)&datFindDir);
	if (fHdlRes == -1L)
	{ return(vecNamFile); }
	else
	{
		do
		{
			if (datFindDir.attrib & FILE_ATTRIBUTE_DIRECTORY)
			{
				//must check twice, from internet
				if((wcscmp( datFindDir.name, L".") != 0 ) && (wcscmp( datFindDir.name, L"..") != 0))
				{
					string strNamDir(WCharToChar(datFindDir.name));
					strNamDir = strNamPathI + strNamDir + "\\";
					vector<string> vecSubDir = readSameTypeFromPath( strNamDir, strTypeFile);
					for( vector<string>::size_type i = 0; i < vecSubDir.size(); ++i)
					{ vecNamFile.push_back(vecSubDir[i]); }
				}
			}
		}
		while (_wfindnext( fHdlRes, (_wfinddata64i32_t*)&datFindDir) == 0);
	}

	return vecNamFile;
}

OP_MN_API string OPTools::findFileFromPath( string strPathI, string strFileI, bool bSearchSubI)
{
	if(strPathI.back() != '\\')	{ strPathI += '\\'; }

	//search files
	string strWhlFile = strPathI + strFileI;
	WIN32_FIND_DATA datFindFile;
	HANDLE hResult = ::FindFirstFile( CharToWChar( strWhlFile.c_str()), (LPWIN32_FIND_DATAW)&datFindFile);

	if(hResult != INVALID_HANDLE_VALUE)
	{
		if(!(datFindFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//not dir, return the whole path about the file
			::FindClose(hResult);
			return strWhlFile;
		}
	}

	//dont search the sub, return empty string
	if(!bSearchSubI)
	{
		::FindClose(hResult);
		return FILE_NOFOUND;
	}

	//search dir
	struct
	{
		unsigned    attrib;
		__time64_t  time_create;    // -1 for FAT file systems 
		__time64_t  time_access;    // -1 for FAT file systems 
		__time64_t  time_write;
		_fsize_t    size;
		wchar_t     name[260];
	} datFindDir;
	string strAllDir = strPathI + "*";
	WCHAR* pWStrTmp	= CharToWChar(strAllDir.c_str());
	long fHdlRes = ::_wfindfirst( pWStrTmp, (_wfinddata64i32_t*)&datFindDir);
	if (fHdlRes == -1L)
	{
		return FILE_NOFOUND;
	}
	else
	{
		do
		{
			if (datFindDir.attrib & FILE_ATTRIBUTE_DIRECTORY)
			{
				//must check twice, from internet
				if((wcscmp( datFindDir.name, L".") != 0 ) && (wcscmp( datFindDir.name, L"..") != 0))
				{
					string strDir(WCharToChar(datFindDir.name));
					if(strDir == "FW_140805")
					{
						strDir = strDir;
					}
					strDir = strPathI + strDir + '\\';
					string strFileFnd = findFileFromPath( strDir, strFileI, bSearchSubI);
					if(strFileFnd != FILE_NOFOUND)
					{
						return strFileFnd;
					}
				}
			}
		}
		while (_wfindnext( fHdlRes, (_wfinddata64i32_t*)&datFindDir) == 0);
		
		return FILE_NOFOUND;
	}
}

void getAttrFromElement( TiXmlElement* pXmlEleI, SXmlNode* pXmlNdI)
{
	TiXmlAttribute* pAttr = pXmlEleI->FirstAttribute();
	MAP_STR mpRet;
	while(pAttr)
	{
		string strNam = pAttr->Name();
		if(OPTools::cmpStrIgnSize( strNam, "id"))
		{
			pXmlNdI->m_strNodeID = pAttr->Value();
		}
		else if(OPTools::cmpStrIgnSize( strNam, "name"))
		{
			pXmlNdI->m_strNodeNam = pAttr->Value();
		}
		else
		{
			mpRet.insert(MAP_STR::value_type( pAttr->Name(), pAttr->Value()));
		}
		pAttr = pAttr->Next();
	}
	pXmlNdI->m_mpAttribute = mpRet;
}

OP_MN_API SXmlNode* OPTools::GetDataFromXml(TiXmlNode* pParentI)
{
	// Node was not existed, return Null 
	if(!pParentI)	{ return NULL; }

	// Switch process according to the nodes' type
	string strNodeTyp = "";
	SXmlNode* pCurNode = NULL;
	switch(pParentI->Type())
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		pCurNode = new SXmlNode();
		pCurNode->m_strNodeTyp = "<DOC>";
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		pCurNode = new SXmlNode();
		strNodeTyp = pParentI->Value();
		pCurNode->m_strNodeTyp = strNodeTyp;
		getAttrFromElement( pParentI->ToElement(), pCurNode);
		break;

	case TiXmlNode::TINYXML_TEXT:
		pCurNode = new SXmlNode();
		pCurNode->m_strText = pParentI->ToText()->Value();
		break;

	default:
		break;
	}

	// Reviews the sub nodes and append references to current
	for ( TiXmlNode* pChild = pParentI->FirstChild(); \
		pChild != NULL; pChild = pChild->NextSibling()) 
	{
		SXmlNode* pSubNode = GetDataFromXml(pChild);
		if(pSubNode == NULL)	{ continue; }
		// When the sub node was text, copy it's text to current
		// text member, and delete this sub node's pointer
		else if(!pSubNode->m_strText.empty())
		{
			pCurNode->m_strText = pSubNode->m_strText;
			delete(pSubNode);
		}
		else
		{
			pCurNode->m_aySubNodes.push_back(pSubNode);
		}
	}
	return pCurNode;
}