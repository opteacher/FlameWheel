#pragma once

//Exports macro
#ifdef FWMAIN_EXPORTS
#define FWMAIN_API __declspec(dllexport)
#else
#define FWMAIN_API __declspec(dllimport)
#endif
//template class should use this macro
#ifndef ALGORITHM_EXPORTS
#define ALGORITHM_TEMPLATE
#else //EXPORT
#define ALGORITHM_TEMPLATE __declspec(dllexport)
#endif

//Pointer safe delete
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	{ if(x) { delete(x); (x) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if (p) { p->Release(); p=NULL; } }
#endif

//Single control declare and implement
#ifndef SINGLE_DECLARE
#define SINGLE_DECLARE(strClsNam)	\
	protected: static strClsNam* s_pInstance; \
	public: static strClsNam* getInstance();
#endif
#ifndef SINGLE_IMPLEMENT
#define SINGLE_IMPLEMENT(strClsNam)	\
	strClsNam* strClsNam::s_pInstance	= NULL; \
	strClsNam* strClsNam::getInstance()	{ if(!s_pInstance) s_pInstance = new strClsNam; return(s_pInstance); }
#endif

//Get Set macro
#ifndef FUNC_GET
#define FUNC_GET( vMem, strMem, tpRet)	tpRet get##strMem##() const	{ return vMem; }
#endif
#ifndef FUNC_GET_I
#define FUNC_GET_I( vMem, strMem, tpRet)	tpRet Get##strMem##() const	{ return vMem; }
#endif
#ifndef FUNC_SET
#define FUNC_SET( vMem, strMem, tpPam) void set##strMem##(tpPam vInpI)	{ vMem = vInpI; }
#endif
#ifndef FUNC_GS
#define FUNC_GS( vMem, strMem, tpMem)	\
	FUNC_GET( vMem, strMem, tpMem)\
	FUNC_SET( vMem, strMem, tpMem)
#endif
#ifndef FUNC_IS
#define FUNC_IS( vMem, strMem)	bool is##strMem##() const	{ return vMem; }
#endif

//Turn the FW to D3D
#ifndef CVT_POI_D3D
#define CVT_POI_D3D(poiI)	(D3DXVECTOR3( poiI.m_fX, poiI.m_fY, poiI.m_fZ))
#endif
#ifndef CVT_POI_FW
#define CVT_POI_FW(poiI)	(OPMath::SQuate( poiI.x, poiI.y, poiI.z, 1))
#endif

//Turn the D3D color to DWORD
#ifndef COL_TO_DWD
#define COL_TO_DWD(colI)	(D3DCOLOR_COLORVALUE( colI.r, colI.g, colI.b, colI.a))
#endif

//String convert to bool
#ifndef STR_TO_BOOL
#define STR_TO_BOOL(strI)	((OPTools::cmpStrIgnSize( strI, "true")) ? true : false)
#endif
#ifndef BOOL_TO_STR
#define BOOL_TO_STR(bI)	((bI) ? "true" : "false")
#endif

//Check STL array existed the identified element
#ifndef CHK_EXS_STL
#define CHK_EXS_STL( aryI, keyI)	(aryI.find(keyI) != aryI.end())
#endif

//Get sub string by begin and end location
#ifndef GET_SUB_STR
#define GET_SUB_STR( locBeg, locEnd)	(locBeg), ((locEnd) - (locBeg))
#endif

//Check whether is identified class
#ifndef INI_TYPID
#define INI_TYPID(tpClsI)	const DWORD tpClsI::s_dwTypID(OPMath::geneHashVal(typeid(tpClsI).name()));
#endif
#ifndef CHK_IS_CLASS
#define CHK_IS_CLASS	\
	private: static const DWORD s_dwTypID;\
	public: static DWORD getTypID() { return s_dwTypID; }\
	public: virtual bool chkIsClass(DWORD dwTypI) const	{\
		return __super::chkIsClass(dwTypI) || dwTypI == s_dwTypID; }
#endif
#ifndef CHK_IS_CLASS_BS
#define CHK_IS_CLASS_BS	\
	private: static const DWORD s_dwTypID;\
	public: static DWORD getTypID() { return s_dwTypID; }\
	public: virtual bool chkIsClass(DWORD dwTypI) const	{ return dwTypI == s_dwTypID; }
#endif

// Initialize map
#ifndef VTP_INI_MAP
#define VTP_INI_MAP( tpKey, tpVal)	typedef std::map<tpKey, tpVal>::value_type VtpKeyVal;
#endif
#ifndef ITR_RVW_MAP
#define ITR_RVW_MAP( tpKey, tpVal)	typedef std::map<tpKey, tpVal>::iterator ItrKeyVal;
#endif
#ifndef BEG_INI_MAP
#define BEG_INI_MAP( tpKey, tpVal, uNum, ayTmp)	std::map<tpKey, tpVal>::value_type ayTmp[uNum] = {
#endif
#ifndef END_INI_MAP
#define END_INI_MAP( tpKey, tpVal, uNum, ayTmp, mpNam)	}; std::map<tpKey, tpVal> mpNam( ayTmp, ayTmp + uNum);
#endif