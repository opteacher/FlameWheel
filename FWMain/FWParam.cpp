#pragma once
#include "stdafx.h"
#include "FWParam.h"
#include "../OPTools/String.h"
using OPTools::cmpStrIgnSize;

AryXmlNodes findIdentifiedNode( SXmlNode* pParentI, string strNodeI)
{
	AryXmlNodes ayTree;

	// If current node was what we needs,
	// add to return array and return(skip the subset)
	if(cmpStrIgnSize( pParentI->m_strNodeTyp, strNodeI))
	{
		ayTree.push_back(pParentI);	return ayTree;
	}

	typedef AryXmlNodes::iterator IterXmlNodes;
	// Review it's sub nodes for check more right nodes
	AryXmlNodes& aySubNodes = pParentI->m_aySubNodes;
	for( IterXmlNodes iter = aySubNodes.begin(); iter != aySubNodes.end(); ++iter)
	{
		AryXmlNodes ayTrTmp = findIdentifiedNode( *iter, strNodeI);
		if(!ayTrTmp.empty())
		{
			ayTree.insert( ayTree.end(), ayTrTmp.begin(), ayTrTmp.end());
		}
	}
	return ayTree;
}

FWMAIN_API AryXmlNodes LoadParamFromXML( string strFileI, string strNodeI )
{
	// Read document from XML file, and load it
	TiXmlDocument docCfg(strFileI.c_str());
	bool bSuccedLoad = docCfg.LoadFile();
	if(bSuccedLoad)
	{
		// If Succeed load XML, get all data from it
		SXmlNode* pNode = OPTools::GetDataFromXml(&docCfg);
		if(!pNode)	{ return AryXmlNodes(); }
		
		// Find specific node according to the nodes' type that parameter identified
		return findIdentifiedNode( pNode, strNodeI);
	}
	else
	{
		return AryXmlNodes();
	}
}

FWMAIN_API void ClsXmlNodeTree(SXmlNode* pRootI)
{
	if(pRootI != NULL)
	{
		AryXmlNodes& aySubNds = pRootI->m_aySubNodes;
		for( UINT i = 0; i < aySubNds.size(); ++i)
		{
			ClsXmlNodeTree(aySubNds[i]);
		}
		SAFE_DELETE(pRootI);
	}
}

void CFWParam::add( string strKeyI, string strValI )
{
	pvt_mpPam.insert(MapStr::value_type( strKeyI, strValI));
}

void CFWParam::upd( string strKeyI, string strValI )
{
	if(!CHK_EXS_STL( pvt_mpPam, strKeyI))	{ return; } 
	pvt_mpPam[strKeyI] = strValI;
}

void CFWParam::cls()
{
	pvt_mpPam.clear();
}

void CFWParam::rmv( string strKeyI )
{
	pvt_mpPam.erase(strKeyI);
}

CFWParam CFWParam::mrg(const CFWParam& rPamI)
{
	pvt_mpPam.insert( rPamI.pvt_mpPam.begin(), rPamI.pvt_mpPam.end());	return pvt_mpPam;
}

CFWParam CFWParam::mrg(const MapStr& mpPamI)
{
	pvt_mpPam.insert( mpPamI.begin(), mpPamI.end());	return pvt_mpPam;
}

bool CFWParam::has( string strKeyI ) const
{
	return pvt_mpPam.find(strKeyI) != pvt_mpPam.end();
}

#define RVW_STR_MAP_C	for( CitrStr iter = pvt_mpPam.begin(); iter != pvt_mpPam.end(); ++iter)

bool CFWParam::hasSuchHead(string strKeyHdI) const
{
	RVW_STR_MAP_C
	{
		string strHd = iter->first.substr( 0, strKeyHdI.length());
		if(strHd == strKeyHdI)	{ return true; }
	}
	return false;
}

bool CFWParam::isEmpty() const
{
	return pvt_mpPam.empty();
}

std::vector<string> CFWParam::getKeySet() const
{
	std::vector<string> ayKey;
	RVW_STR_MAP_C	{ ayKey.push_back(iter->first); }
	return ayKey;
}

#define FIND_VALUE	\
	MapStr::const_iterator iterFnd = pvt_mpPam.find(strKeyI); \
	if(iterFnd == pvt_mpPam.end()) { \
		throw std::runtime_error("Not such parameter"); \
	}

#define GET_NUM(numTyp, funcNam) \
	numTyp CFWParam::funcNam( string strKeyI ) const { \
		FIND_VALUE; \
		return OPTools::strToNum<numTyp>(iterFnd->second); \
	}

bool CFWParam::getBool( string strKeyI ) const
{
	FIND_VALUE;
	return STR_TO_BOOL(iterFnd->second);
}

char CFWParam::getChar( string strKeyI ) const
{
	FIND_VALUE;
	return iterFnd->second.front();
}

string CFWParam::getString( string strKeyI ) const
{
	FIND_VALUE;
	return iterFnd->second;
}

GET_NUM( int, getInt);
GET_NUM( long, getLong);
GET_NUM( float, getFloat);
GET_NUM( double, getDouble);
GET_NUM( UINT, getUint);
GET_NUM( WORD, getWord);
GET_NUM( DWORD, getDword);

#define GET_MISC(miscTyp, funcNam) \
	miscTyp CFWParam::funcNam( string strKeyI ) const { \
		FIND_VALUE; \
		return miscTyp().convertFromStr(iterFnd->second); \
	}

GET_MISC( SFWColor, getColor);
GET_MISC( SFWQuate, getQuate);
GET_MISC( CFWMatrix, getMatrix);
GET_MISC( SFWMaterial, getMaterial);
GET_MISC( CFWCoord, getCoordinate);
GET_MISC( CFWAABBBox, getAABBBox);
GET_MISC( SFWQuate, getSphere);
