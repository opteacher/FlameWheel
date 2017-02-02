#pragma once
#include "FWMacro.h"
#include "FWIntf.h"
#include "../OPMath/Math.h"
using namespace OPMath;

#ifndef ERR_OBJ_ID
#define ERR_OBJ_ID 0
#endif

class FWMAIN_API IFWFactory
{
protected:
	typedef std::map<DWORD, CFWIntf*> MapIntf;
	typedef MapIntf::const_iterator CiterIntf;
	typedef MapIntf::iterator IterIntf;
	typedef MapIntf::value_type VtpIntf;
	typedef std::map<string, DWORD> MapCpdTbl;
	typedef MapCpdTbl::const_iterator CitrCpdTbl;
	typedef MapCpdTbl::value_type VtpCpdTbl;
public: virtual CFWIntf& Create(CFWParam& pamI) = 0;
public:
	virtual CFWIntf* GetIntfByID(DWORD dwObjIdI) const
	{
		CiterIntf iterFnd = ptd_mpIntfs.find(dwObjIdI);
		if(iterFnd == ptd_mpIntfs.end())	{ return NULL; }
		return iterFnd->second;
	}
	virtual bool HasSuchObj(DWORD dwObjIdI) const
	{
		return CHK_EXS_STL( ptd_mpIntfs, dwObjIdI);
	}
protected: MapIntf ptd_mpIntfs;
};

class FWMAIN_API CFWDevFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWDevFcty);
	/*
	*	Required parameters:
	*		"devName" = Name of device
	*		"adapter" = Graphic adapter ID
	*		"backGdCol" = Background color
	*		"devType" = Device type(HAL/REF)
	*		"width" = Back buff width
	*		"height" = Back buff height
	*		"backFmt" = Back buff color format
	*		"backCount" = Back buffs' number
	*		"multiSpl" = Multiple sample
	*		"swapEffc" = Back surface swap effect
	*		"beWindow" = Display by window or full screen
	*		"enableAutoDepth" = Use automatic depth buff
	*		"autoDepthFmt" = Auto depth buff's color format[enableAutoDepth=true]
	*		"flags" = Set as zero
	*		"fullScnRefsRate" = Rate of full screen[beWindow=false]
	*		"presInterval" = Present interval(back surfaces' swap delta)
	*		"setCurrent" = Set this device to current
	*/
public: virtual CFWIntf& Create(CFWParam& pamI);
protected:
	DWORD switchDevTypFromPam(string strPamI);
	DWORD switchVwFmtFromPam(string strPamI);
	DWORD switchSwapEffcFromPam(string strPamI);
	UINT switchPresIntervalFromPam(string strPamI);

protected: DWORD ptd_dwCurDev;
public:
	FUNC_GS( ptd_dwCurDev, CurDevID, DWORD);
	FUNC_GET( GetIntfByID(ptd_dwCurDev), CurDev, CFWIntf*);

protected: MapCpdTbl ptd_mpCpdTbl;
public:
	virtual CFWIntf* GetIntfByNam(string strNamI) const
	{
		CitrCpdTbl itrFnd = ptd_mpCpdTbl.find(strNamI);
		if(itrFnd == ptd_mpCpdTbl.end())	{ return NULL; }
		return __super::GetIntfByID(itrFnd->second);
	}
};

class FWMAIN_API CFWCamFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWCamFcty)
	/*
	*	Required parameters:
	*		"camName" = Name of the camera
	*		[N]"locEye" = Center of camera
	*		[1]"dirLook" = Camera's direction
	*		[2]"locTgt" = Where camera looking at
	*		"viewWid" = View's width
	*		"viewHgt" = View's height
	*		"clsProjPln" = Closest distance to project plane
	*		"farProjPln" = Farthest distance to project plane
	*		"setOperation" = Set this camera to operation
	*/
public: virtual CFWIntf& Create(CFWParam& pamI);

protected: DWORD ptd_dwOprCam;
public:
	FUNC_GS( ptd_dwOprCam, OprCamID, DWORD);
	FUNC_GET( GetIntfByID(ptd_dwOprCam), OprCam, CFWIntf*);

protected: MapCpdTbl ptd_mpCpdTbl;
public:
	virtual CFWIntf* GetIntfByNam(string strNamI) const
	{
		CitrCpdTbl itrFnd = ptd_mpCpdTbl.find(strNamI);
		if(itrFnd == ptd_mpCpdTbl.end())	{ return NULL; }
		return __super::GetIntfByID(itrFnd->second);
	}
};

class FWMAIN_API CFWMshFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWMshFcty)
	/*
	*	Required parameters:
	*		[N]"aryPois" = Pointer of quaternion array
	*		[N]"numPois" = Identified the number of points
	*		[N]"aryIdxs" = Pointer of indexes' array
	*		[N]"numIdxs" = Identified the number of indexes
	*		"vtxCol" = Vertexes' color
	*		"texUV" + texID = Texture UV map, first U, second V
	*			(+_+: texID start from 1, size was 2*numPois)
	*		"aryNmls" = Pointer of normal vectors' array
	*		"genNmls" = Use mesh auto normals instead of givens
	*		"weldVtx" = If vertexes are in same location, weld them together
	*		"optimize" = Optimize mesh, if needed
	*		"primTyp" = Primitive type, +_+: if add this parameter, 
	*			factory will produce primitive set instead of mesh,
	*			and will only use "aryPois", "numPois", "aryIdxs", "numIdxs", "vtxCol"
	*			(+_+: will rebuild vertex array, if index existed)
	*								
	*/
public: virtual CFWIntf& Create(CFWParam&);
public:// Geometry mesh generation algorithm
	static CFWParam GenPlnInitPam(
		float fLenI, float fWidI,
		UINT uLenSecI, UINT uWidSecI,
		bool bGrteWFI = false);
	// +_+: Use point LTF RTF LBB RBB to build rectangle
	static CFWParam GenRectInitPam(
		CPoint poiLTI, CPoint poiRTI,
		CPoint poiLBI, CPoint poiRBI,
		bool bGrteWFI = false);
	static CFWParam GenBoxInitPam( CBoxAABB boxI, bool bGrteWFI = false);
	static CFWParam GenLineInitPam( CPoint poiBegI, CPoint poiEndI);
	static CFWParam GenTriangleInitPam(
		CPoint poiAI, CPoint poiBI, CPoint poiCI, bool bGrteWFI = false);
public:
	/*
	*	Clean up parameters: "aryPois", "aryIdxs", "texUV" + texID, "aryNmls"
	*/
	static void ClsCrtePam(CFWParam pamI);
	static CFWParam MrgPam( CFWParam& pamBasIO, CFWParam pamSubI, UINT uSubIdI);

protected: DWORD ptd_dwDrwMsh;// Current drawing mesh
public:
	FUNC_GS( ptd_dwDrwMsh, DrwMshID, DWORD);
	FUNC_GET( GetIntfByID(ptd_dwDrwMsh), DrwMsh, CFWIntf*);
};

class FWMAIN_API CFWSknFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWSknFcty)
public:
	/*
	*	Required parameters:
	*		[N]"mtlDif" = Diffuse color
	*		"mtlAmb" = Ambient color
	*		"mtlSpc" = Specular color
	*		"mtlEms" = Emissive color
	*		"mtlPwr" = material's power
	*		"difTex" = Diffuse texture's file
	*		"ambTex" = Ambient texture's file
	*		"spcTex" = Specular texture's file
	*		"obcTex" = Opacity texture's file
	*		"bmpTex" = Bump texture's file
	*		"relTex" = Reflection texture's file
	*		"refTex" = Refraction texture's file
	*		"dspTex" = Displacement texture's file
	*/
	virtual CFWIntf& Create(CFWParam&);
public:
	virtual CFWIntf* ChkExs(
		const SFWMaterial&,
		std::map<UINT, string>) const;
	static CFWParam MrgPam( CFWParam& pamBasIO, CFWParam pamSubI, bool bCvrBasI = true);

protected: DWORD ptd_dwActSkn;
public:
	FUNC_GS( ptd_dwActSkn, ActSknID, DWORD);
	FUNC_GET( GetIntfByID(ptd_dwActSkn), ActSkn, CFWIntf*);
};

class FWMAIN_API CFWLgtFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWLgtFcty)
	/*
	*	Required parameters:
	*		"lgtName" = Name of light
	*		"lgtTyp" = Point/Spot/Direction
	*		"lgtDif" = Diffuse color
	*		"lgtAmb" = Ambient color
	*		"lgtSpc" = Specular color
	*		"lgtPos" = Light's location(+_+: For Point and Spot)
	*		"lgtDir" = Light's direction(+_+: For Spot and Direction)
	*		"lgtRng" = Light's range(+_+: For Spot and Point)
	*		"lgtAtt0" = Control light's power(1/(A0 + A1*D + A2*D^2))
	*		"lgtAtt1" = Control light's power
	*		"lgtAtt2" = Control light's power
	*		"lgtID" = Where set the light
	*		"enable" = defined whether enable the light(required "lgtID")
	*			If has not defined "lgtID" the light will use ID:0 for enable
	*/
public: virtual CFWIntf& Create(CFWParam& pamI);

protected: MapCpdTbl ptd_mpCpdTbl;
public:
	virtual CFWIntf* GetIntfByNam(string strNamI) const
	{
		CitrCpdTbl itrFnd = ptd_mpCpdTbl.find(strNamI);
		if(itrFnd == ptd_mpCpdTbl.end())	{ return NULL; }
		return __super::GetIntfByID(itrFnd->second);
	}
};

class FWMAIN_API CFWSpcFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWSpcFcty)
public: CFWSpcFcty(): ptd_fcGravity( 0, -10, 0)	{}
public:
	/*
	*	Required parameters:
	*		[1]"spcPos" = Location in the space(CFWQuateEx)
	*		[2]"spcLoc" = Coordinate system of the space
	*		=========== Physics space ===========
	*		"lock" = Whether lock space object
	*			(can't move but join the collide test)
	*		"mass" = Object's mass
	*		[1]"outSrdBox" = Out surround box
	*		[2]"outSrdSph" = Out surround sphere
	*		"rgdVol" = Simple mesh for colliding test
	*			If no rigid volume, it will take out
	*			surround object as rigid volume
	*/
	virtual CFWIntf& Create(CFWParam& pamIO);
	virtual CFWIntf& Copy(DWORD dwSpcIdI);
public:
	FUNC_GET( ptd_mpIntfs, AllSpc, MapIntf);
public: bool checkCollide(CFWIntf*);

protected: SFWForce ptd_fcGravity;
public: FUNC_GET( ptd_fcGravity, Gravity, SFWForce);
};

class FWMAIN_API CFWTxtFcty: public IFWFactory
{
	SINGLE_DECLARE(CFWTxtFcty)
public: CFWTxtFcty(): ptd_dwDftTxt(ERR_OBJ_ID)	{}
public:
	/*
	*	Required parameters:
	*		[N]"3DText" = Identified whether 3D text
	*/
	virtual CFWIntf& Create(CFWParam&);

protected: DWORD ptd_dwDftTxt;
public:
	FUNC_GS( ptd_dwDftTxt, DftTxtID, DWORD);
	virtual CFWIntf* getDftTxt();
};