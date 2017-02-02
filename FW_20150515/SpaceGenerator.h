#pragma once
#include "windows.h"
#pragma comment( lib, "../Debug/FWMain.lib")
#include "../FWMain/FWParam.h"
#include "../FWMain/FWMisc.h"
#include "../FWMain/FWNode.h"
using OPMath::CPoint;
#include <vector>

struct SSpaceBox
{
	SSpaceBox(): m_tpBox(BTP_UNKNOWN),
		m_pFront(NULL), m_pBack(NULL),
		m_pLeft(NULL), m_pRight(NULL),
		m_pTop(NULL), m_pBottom(NULL)	{}
	SSpaceBox(const SSpaceBox& rBoxI):
		m_tpBox(BTP_UNKNOWN),
		m_pFront(NULL), m_pBack(NULL),
		m_pLeft(NULL), m_pRight(NULL),
		m_pTop(NULL), m_pBottom(NULL)	{}
	enum EBoxTyp
	{
		BTP_UNKNOWN,
		BTP_EARTH_CORE,
		BTP_BED_ROCK,
		BTP_GRASS
	};
	enum EBoxRect
	{
		BRT_FRONT = 1,
		BRT_BACK,
		BRT_LEFT,
		BRT_RIGHT,
		BRT_TOP,
		BRT_BOTTOM
	};
	EBoxTyp m_tpBox;
	SFWQuate m_poiLTF;
	SFWQuate m_poiRBB;
	SFWQuate m_poiCen;
	SSpaceBox* m_pFront;
	SSpaceBox* m_pBack;
	SSpaceBox* m_pLeft;
	SSpaceBox* m_pRight;
	SSpaceBox* m_pTop;
	SSpaceBox* m_pBottom;
};

class CSpaceGenerator
{
public:
	typedef CFWParam (*ProcBox)(SSpaceBox*);
	typedef std::vector<CFWParam> AryIniScn;
public:
	CSpaceGenerator():
		ptd_pBoxLTF(NULL),
		ptd_pBoxRBB(NULL),
		ptd_fBoxSz(10),
		ptd_fEdgeLen(0)	{}
	virtual void Destroy();
protected:
	SSpaceBox* ptd_pBoxLTF;
	SSpaceBox* ptd_pBoxRBB;
	float ptd_fBoxSz;
	float ptd_fEdgeLen;
public:
	SSpaceBox* BuildSpace( float, UINT, UINT);
	AryIniScn GeneScnIniInfo();
private:
	void geneRow( SSpaceBox*, UINT);
	void geneCol(SSpaceBox*);
	void geneHgt(SSpaceBox*);
	static CFWParam geneCrtePam(SSpaceBox*);
	// The second parameter identified which rect of box to generate
	static CFWParam geneRectCrtePam( SSpaceBox*, SSpaceBox::EBoxRect);
protected:
	void setBoxTypFmLT( SSpaceBox::EBoxTyp, SSpaceBox*);
	void setBoxTypFmRB( SSpaceBox::EBoxTyp, SSpaceBox*);
	AryIniScn rvwSpaceToCrtePam();// From LTF
	AryIniScn rvwRowLR( SSpaceBox*, ProcBox);// From left->right
	AryIniScn rvwRowRL( SSpaceBox*, ProcBox);// From right->left
	AryIniScn rvwColFB( SSpaceBox*, ProcBox);// From front->back
	AryIniScn rvwColBF( SSpaceBox*, ProcBox);// From back->front
	AryIniScn rvwPlnFrmLF( SSpaceBox*, ProcBox);
	AryIniScn rvwPlnFrmRB( SSpaceBox*, ProcBox);
};