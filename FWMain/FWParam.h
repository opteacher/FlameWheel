#pragma once
#pragma warning(disable:4251)
#include "FWMacro.h"
#include "FWMisc.h"
#include "FWMaterial.h"
#include "../OPTools/File.h"
using OPTools::SXmlNode;
#include <map>
#include <vector>
using std::string;

class FWMAIN_API CFWParam
{
public:
	typedef std::map<string, string> MapStr;
	typedef MapStr::const_iterator CitrStr;
public:
	CFWParam()	{}
	CFWParam(MapStr mpPamI): pvt_mpPam(mpPamI)	{}
	CFWParam(const CFWParam& pamI): pvt_mpPam(pamI.pvt_mpPam)	{}
	CFWParam( string strKeyI, string strValI)	{ add( strKeyI, strValI); }
private:
	MapStr pvt_mpPam;
public:
	/*
	*	Better not use this function to initialize parameter.
	*	general way should read all data from XML files first,
	*	and then change settings by needs
	*/
	inline void add( string strKeyI, string strValI);
	void upd( string KeyI, string strValI);
	inline void cls();
	inline void rmv(string strKeyI);
	inline CFWParam mrg(const CFWParam& rPamI);
	inline CFWParam mrg(const MapStr& mpPamI);
	inline bool has(string strKeyI) const;
	bool hasSuchHead(string strKeyHdI) const;
	inline bool isEmpty() const;
	std::vector<string> getKeySet() const;
	// Get parameter by basic type
	inline bool getBool(string strKeyI) const;
	inline char getChar(string strKeyI) const;
	inline int getInt(string strKeyI) const;
	inline long getLong(string strKeyI) const;
	inline float getFloat(string strKeyI) const;
	inline double getDouble(string strKeyI) const;
	inline UINT getUint(string strKeyI) const;
	inline WORD getWord(string strKeyI) const;
	inline DWORD getDword(string strKeyI) const;
	inline string getString(string strKeyI) const;
	inline SFWColor getColor(string strKeyI) const;
	inline SFWQuate getQuate(string strKeyI) const;
	inline CFWMatrix getMatrix(string strKeyI) const;
	inline SFWMaterial getMaterial(string strKeyI) const;
	inline CFWCoord getCoordinate(string strKeyI) const;
	inline CFWAABBBox getAABBBox(string strKeyI) const;
	inline SFWQuate getSphere(string strKeyI) const;
};

typedef std::vector<SXmlNode*> AryXmlNodes;

// Recommended way to build parameter
// [strFileI] Identified which XML for load
// [strNodeI] Identified which node for read(not node name but ID defined as attribute)
FWMAIN_API AryXmlNodes LoadParamFromXML( string strFileI, string strNodeI);

FWMAIN_API void ClsXmlNodeTree(SXmlNode* pRootI);