#pragma once
#pragma warning(disable:4251)
#include <map>
#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include "windows.h"
#include "OPTools.h"
#pragma comment( lib, "../tinyxml/lib/tinyxml.lib")
#include "../tinyxml/tinyxml.h"

using std::string;

typedef std::map<string, string> MAP_STR;
typedef std::vector<string> VEC_STR;

namespace OPTools
{
	/*****
	*	time data
	*	use: FW's time struct, detail to M second
	*/
	struct STime
	{
		int m_iYear; int m_iMonth; int m_iDay;
		int m_iHour; int m_iMinute; int m_iSecond;
		DWORD m_dwMSecond;

		STime(): m_iYear(-1), m_iMonth(-1), m_iDay(-1), \
			m_iHour(-1), m_iMinute(-1), m_iSecond(-1), m_dwMSecond(0)	{}
		STime(const STime& refTimeI)
		{
			m_iYear	= refTimeI.m_iYear;
			m_iMonth	= refTimeI.m_iMonth;
			m_iDay	= refTimeI.m_iDay;
			m_iHour	= refTimeI.m_iHour;
			m_iMinute	= refTimeI.m_iMinute;
			m_iSecond	= refTimeI.m_iSecond;
			m_dwMSecond	= refTimeI.m_dwMSecond;
		}

		bool isUninit()
		{
			return(m_iYear < 0 || m_iMonth < 0 || m_iDay < 0 \
				|| m_iHour < 0 || m_iMinute < 0 || m_iSecond < 0);
		}
		bool isLegal()
		{
			bool bRet = true;
			bRet &= (m_iYear >= 1000 && m_iYear <= 3000);
			bRet &= (m_iMonth >= 1 && m_iMonth <= 13);
			bRet &= (m_iDay >= 1 && m_iDay <= 31);
			bRet &= (m_iHour >= 0 && m_iHour <= 24);
			bRet &= (m_iMinute >= 1 && m_iMinute <= 60);
			bRet &= (m_iSecond >= 1 && m_iSecond <= 60);
			return(bRet);
		}
	};

	/*****
	*	function calling struct
	*	use: log the calling stack in it
	*/
	struct SInfoFun
	{
		string m_strClsNam;
		string m_strFunNam;
		//current information
		DWORD m_dwStartTime;
		std::streamoff m_uLocInLog;//locate the position in the log
		
		SInfoFun(): m_strClsNam(""),m_strFunNam(""), 
			m_dwStartTime(0), m_uLocInLog(0)	{}
			
		SInfoFun(const SInfoFun& refTstI):
			m_strClsNam(refTstI.m_strClsNam), m_strFunNam(refTstI.m_strFunNam),
			m_dwStartTime(refTstI.m_dwStartTime), m_uLocInLog(refTstI.m_uLocInLog)	{}
			
		SInfoFun& operator=(const SInfoFun& refTstI)
		{
			m_strClsNam = refTstI.m_strClsNam;
			m_strFunNam = refTstI.m_strFunNam;
			m_dwStartTime = refTstI.m_dwStartTime;
			m_uLocInLog = refTstI.m_uLocInLog;
			return(*this);
		}
	};

	/*****
	*	function calling catch
	*	(+_+:add to the function's head and tail)
	*/
	class OP_MN_API CCallCatch
	{
	protected:
		CCallCatch();
		static CCallCatch* s_pInstance;
	public:
		~CCallCatch()	{}
		static CCallCatch* getInstance()
		{
			if(!s_pInstance)
			{
				s_pInstance = new CCallCatch;
			}
			return s_pInstance;
		}
		static void release()
		{
			if(s_pInstance)
			{
				delete s_pInstance;
				s_pInstance = NULL;
			}
		}
	protected:
		string m_strTstFile;//calling log file
		//key will be the id of the thread
		std::map<DWORD, string> m_mpCurFun;
		std::map<DWORD, std::deque<SInfoFun>> m_mpCallDeque;//function calling deque
	public:
		//+_+: file name must include path
		//+_+: no point in the file's path
		void funcTstBeg( string strFileI, string strFuncI);
		void funcTstEnd();
	};

#ifndef FUNC_BEG
#define FUNC_BEG	\
	(OPTools::CCallCatch::getInstance()->funcTstBeg( __FILE__, __FUNCTION__))
#endif

#ifndef FUNC_END
#define FUNC_END	\
	(OPTools::CCallCatch::getInstance()->funcTstEnd())
#endif

	//write log to log files
	//parameter: FW time struct/hour+minute+second/second+M second
	//+_+: if time as the parameter, ungiven data will according to current time
	OP_MN_API void writeLog( string strFileI, string strLogI, STime TimeI = STime());
	OP_MN_API void writeLog( string strFileI, string strLogI, int iHourI, int iMinuteI, int iSecondI);
	OP_MN_API void writeLog( string strFileI, string strLogI, int iSecondI, DWORD dwMSecondI);
	//parameter: input file + happened lines + exception's id(define in the FWMisc.h) + exception's described
	OP_MN_API void writeExcLog( string strFileI, string strHpnFlI, string strLnI, string strExcIdI, string strDscbI);

	//insert data type files
	OP_MN_API void writeInstFile( string strFileI, string strDatI, std::streamoff uLocInsI);

#ifndef MASK_SIZE
#define MASK_SIZE 128
#endif
	//build and write encrypted bit data into the files
	//paramter: bCover true=get out all data from current data, and compose new part to the file;
	//          bCover false=clear up all data of the current file
	OP_MN_API void writeInDatFile( string strFileI, string strDatInpI);
	//parameter: if want the mark map, use the second param, new a size 128 type BYTE array to input
	OP_MN_API string readOutDatFile( string strFileI, BYTE* pMarkMpO = NULL);

	//process INI files
	OP_MN_API void writeKVIntoIni( string strFileI,//file's name
		string strSecI,//section's name
		string strNodeI = string(),//node's name
		string strNdeValI = string(),//node's value
		bool bCoverWriteI = false);//be cover write

	OP_MN_API void writeKVMapIntoIni( string strFileI,//file's name
		string strSecI,//section's name
		MAP_STR mpKVI,//nodes' name and value map
		bool bCoverWriteI = false);//be cover write

	OP_MN_API string readOutIniFile( string strFileI, string strSecI, string strNodeI);
	OP_MN_API MAP_STR readOutAllDatFromSection( string strFileI, string strSecI);
	OP_MN_API VEC_STR readOutAllSection(string strFileI);
	OP_MN_API MAP_STR readOutAllKeyValue(string strFileI);

	//read same type files from identified path
	OP_MN_API VEC_STR readSameTypeFromPath( string strNamPathI, string strTypeFile);
#ifndef FILE_NOFOUND
#define FILE_NOFOUND	""
#endif
	//find file in identified path
	OP_MN_API string findFileFromPath( string strPathI, string strFileI, bool bSearchSubI = true);

	/*
	*	+_+: The ID and name wont insert into the attributes' map
	*/
	struct OP_MN_API SXmlNode
	{
		string m_strNodeTyp;// Type of node
		string m_strNodeID;// Node's ID, defined as id or ID, can be nonexistent
		string m_strNodeNam;// Node's name, defined as name or NAME, also can be nonexistent
		MAP_STR m_mpAttribute;// Attribute map
		string m_strText;// Write inside node, can be nonexistent(single node)
		std::vector<SXmlNode*> m_aySubNodes;
	};
	OP_MN_API SXmlNode* GetDataFromXml(TiXmlNode* pParentI);
};