#pragma once
#include "FWMacro.h"
#include "FWNode.h"
#include "FWMisc.h"
#include "FWModel.h"
#include "FWEffIntf.h"
#include "FWTxtIntf.h"
#include "../OPTools/File.h"
using OPTools::SXmlNode;
#include <vector>
#include <string>
#include <map>
#include <set>
using std::string;

class FWMAIN_API IFWManager
{
public:
	typedef std::map<string, CFWNode*>::const_iterator CiterNode;
	typedef std::map<string, CFWNode*>::value_type VtpNode;
	typedef std::pair<UINT, UINT> PrBegEnd;
public:
	virtual CFWNode* Create(CFWParam pamI) = 0;
	virtual CFWNode* Copy(CFWNode* pNdI)
	{
		CFWNode* pNd = pNdI->Clone();
		ptd_mpNodes.insert(VtpNode( pNd->getNam(), pNd));
		return pNd;
	}
	/*
	*	Required parameters:
	*		[N1]"souNode" = Source node's name
	*		[N2]"souNodeP" = Source node's pointer
	*			+_+: Make sure the pointer is correct,
	*			copy function wont check this again
	*		"mdlAuxTyp" = Auxiliary object type
	*/
	virtual CFWNode* Copy(CFWParam pamI)
	{
		CFWNode* pNode = NULL;
		if(pamI.has("souNode"))
		{
			string strNam = pamI.getString("souNode");
			if(!CHK_EXS_STL( ptd_mpNodes, strNam))
			{
				return NULL;
			}
			pNode = ptd_mpNodes[strNam];
		}
		else if(pamI.has("souNodeP"))
		{
			pNode = (CFWNode*)pamI.getDword("souNodeP");
		}
		else	{ return NULL; }
		CFWNode::EFWAuxObj tpAuxFmr = pNode->getAuxTyp();
		if(pamI.has("mdlAuxTyp"))
		{
			pNode->setAuxTyp((CFWNode::EFWAuxObj)pamI.getUint("mdlAuxTyp"));
		}
		CFWNode* pRetNd = Copy(pNode);
		pNode->setAuxTyp(tpAuxFmr);
		return pRetNd;
	}
protected: std::map<string, CFWNode*> ptd_mpNodes;
public:
	virtual CFWNode* GetNode(string strNamI) const
	{
		CiterNode iterFnd = ptd_mpNodes.find(strNamI);
		return (iterFnd != ptd_mpNodes.end()) ? iterFnd->second : NULL;
	}
};

class FWMAIN_API CFWMdlMngr: public IFWManager
{
public: typedef std::vector<string> AryStr;
	SINGLE_DECLARE(CFWMdlMngr)
public:
	static const string s_cstrWldCod;
	static const string s_cstrLgtLoc;
	static const string s_cstrGrid;
	/*
	*	#Model_Template_Light
	*/
	static const std::set<string> s_cstrUsedNam;
	/*
	*	Required parameters:
	*		"mdlName" = Model's name
	*			(+_+: If it's file model, this name will cover exported name)
	*		[1]"spcPos" = Identified model's position(Location center)
	*		[2]"spcLoc" = Identified model's location(Coordinate system)
	*		[3]"spcID" = If want to use existed space, input the ID
	*		<!-- Cause rigid space object create -->
	*		"lock" = Whether lock space object
	*			(can't move but join the collide test)
	*		"mass" = +_+: Zero mass means cant move. If not defined, will be set as zero
	*		"speed" = Original speed
	*		"addGravity" = Add default gravity defined by space manager
	*		
	*		"wireFrame" = Whether create as wire frame(For geometry)
	*		[1N]"mdlFile" = Model's file path
	*		[2N]"plnLength" = Plane's length
	*		[2N]"plnWidth" = Plane's width
	*		[2]"plnLenSec" = Plane's length section(+_+: cant be set zero!!)
	*		[2]"plnWidSec" = Plane's width section(+_+: cant be set zero!!)
	*		[3N]"boxLTF" = box's left top front point
	*		[3N]"boxRBB" = box's right bottom back point
	*		[4N]"lnBegPoi" = line's begin point
	*		[4N]"lnEndPoi" = line's end point
	*		[5N]"tglPoiA" = Triangle
	*		[5N]"tglPoiB" = Triangle
	*		[5N]"tglPoiC" = Triangle
	*		+_+: Terrain create needs plane data, but defined "plnLenSec"
	*			and "plnWidSec" will let manager create fixed terrain(Not recommended)
	*		"trnC1" = terrain's fixed value C1
	*		"trnC2" = terrain's fixed value C2
	*		[6N]"hgtMapFile" = Terrain's height map(.raw)
	*		[6N]"hgtMapHgt" = Height map's pixel height
	*		[6N]"hgtMapWid" = Height map's pixel width
	*		[6]"hgtScale" = The value will multiply to hight from map
	*		[7N]"poiLT" = Rectangle's left top point
	*		[7N]"poiRT" = Rectangle's right top point
	*		[7N]"poiLB" = Rectangle's left bottom point
	*		[7N]"poiRB" = Rectangle's right bottom point
	*
	*		<!-- Create Mesh Parts -->
	*		"vtxCol" = Vertex color
	*
	*		<!-- Create Skin Parts -->
	*		"mtlDif" + subID = Diffuse color
	*		"mtlAmb" + subID = Ambient color
	*		"mtlSpc" + subID = Specular color
	*		"mtlEms" + subID = Emissive color
	*		"mtlPwr" + subID = material's power
	*		"difTex" + subID = Diffuse texture's file
	*		"ambTex" + subID = Ambient texture's file
	*		"spcTex" + subID = Specular texture's file
	*		"obcTex" + subID = Opacity texture's file
	*		"bmpTex" + subID = Bump texture's file
	*		"relTex" + subID = Reflection texture's file
	*		"refTex" + subID = Refraction texture's file
	*		"dspTex" + subID = Displacement texture's file
	*
	*		"mdlAuxTyp" = Whether auxiliary object
	*			If created object is a auxiliary object
	*			it should not create auxiliary object again
	*			+_+: because this creating isn't belong to user
	*				user cant use this parameter
	*/
	virtual CFWNode* Create(CFWParam);

	/*
	*	Required parameter(Inherit from IFWManager):
	*		"mdlName" = Reset model's name
	*		"spcPos" = Reset model's location
	*
	*		<!-- Reset Skin Parts -->
	*		"mtlDif" = Diffuse color
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
	virtual CFWNode* Copy(CFWParam);
	virtual CFWNode* Copy(CFWNode* pNdI)
	{
		return __super::Copy(pNdI);
	}
public:
	/*
	*	Required parameters:
	*		"mdlName" = Prefix name
	*			(Real coordinate name: mdlNam + s_cstrWldCod)
	*		"axisLen" = Identify axis length, default: 50
	*/
	virtual CFWNode* createWldCod(CFWParam pamI);
	/*
	*	Required parameters:
	*		"mdlName" = Prefix name
	*			(Real coordinate name: mdlNam + s_cstrLgtLoc)
	*/
	virtual CFWNode* createLgtLoc(CFWParam pamI);

	static CFWParam GetGeoCrtePam(CFWParam);
	CFWModel* createGeoMdl(CFWParam pamI);
private:
	CFWModel* createTrnMdl(CFWParam pamI);
	void fillInMdlByPam( CFWParam pamI, CFWModel* pMdlO);
};

// Use: For action user's control and help user control
class FWMAIN_API CFWGuiMngr
{
	SINGLE_DECLARE(CFWGuiMngr)
protected:
	CFWGuiMngr():
		ptd_hMosHook(NULL),
		ptd_fMosRotSpd(10),
		ptd_fMosMovSpd(15),
		ptd_dwSttAuxRndr(-1),
		ptd_dwNumFrm(0),
		ptd_fTimePas(0),
		ptd_fFPS(0)	{}

public:
	/*
	*	Required parameters:
	*		"enableControlIO"=Enable FW GUI control the user's IO
	*		"showWorldCoordinateSys"=Whether show objects' coordinate system
	*		"showLightLocation"=Whether show a model for identified the location of lights
	*		"showFPS" = Whether show FPS(On the left top of screen)
	*/
	virtual void Initialize(CFWParam);
protected:
	HHOOK ptd_hMosHook;
	float ptd_fMosRotSpd;
	float ptd_fMosMovSpd;
	POINT ptd_poiFmrMov;
public:
	void EnableCtlIO(bool = true);
public:
	FUNC_GET( ptd_hMosHook, MouseHook, HHOOK);
	FUNC_GS( ptd_fMosRotSpd, MouseRotSpeed, float);
	FUNC_GS( ptd_fMosMovSpd, MouseMovSpeed, float);
	FUNC_GS( ptd_poiFmrMov, PoiMosFmrMov, POINT);
	
protected: DWORD ptd_dwSttAuxRndr;
public:
	void EnableAuxStt(DWORD dwSttI)	{ ptd_dwSttAuxRndr |= dwSttI; }
	void DsableAuxStt(DWORD dwSttI)	{ ptd_dwSttAuxRndr &= dwSttI; }
	FUNC_GET_I( ptd_dwSttAuxRndr, AuxRndrStt, DWORD);

protected:
	DWORD ptd_dwNumFrm;
	float ptd_fTimePas;
	float ptd_fFPS;
public:
	virtual float GetFPS(float fTimeDeltaI);
};

class FWMAIN_API CFWEffMngr
{
	SINGLE_DECLARE(CFWEffMngr)
public:
	enum
	{
		EFFECT_AMBIENT_LGT,
		EFFECT_SHOW_EDGE,
		EFFECT_ADV_BOARD,
		EFFECT_ALPHA,
		EFFECT_Z_DEPTH,
		EFFECT_UNKNOWN,
		EFFECT_SUPPORTED
	};
protected: CFWEffMngr();
protected: CFWEffIntf ptd_ayEffect[EFFECT_SUPPORTED];
public:
	CFWEffIntf& GetEffect(UINT uEffIdI);
	inline bool ChkEffAva(UINT uEffIdI) const;
	UINT CvtStrToEffID(string strEffI) const;
};

class FWMAIN_API CFWScnMngr: public IFWManager
{
public: typedef std::vector<CFWParam> AryIniScn;
	SINGLE_DECLARE(CFWScnMngr)
public:
	/*
	*	Required parameters:
	*		"scnName" = Identified the name of scene
	*		"setCurrent" = Whether set new scene as current scene
	*		[1]"spcPos" = Identified model's position(Location center)
	*		[2]"spcLoc" = Identified model's location(Coordinate system)
	*/
	virtual CFWNode* Create(CFWParam pamI);
	virtual CFWNode* CreateByXmlNode(SXmlNode* pRootI);
	virtual CFWNode* CreateByPamAry(AryIniScn ayIniI);
private: CFWNode* createObjForScn(SXmlNode* pSubI);
public:
	inline virtual bool SwitchScene(string strNamI);
	inline virtual bool SwitchScene(CFWNode* pScnI);

protected: string ptd_strCurScn;
public:
	FUNC_GET( ptd_strCurScn, CurScnNam, string);
	FUNC_GET( GetNode(ptd_strCurScn), CurScn, CFWNode*);
};