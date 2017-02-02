#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#pragma warning(disable:4800)
#include "windows.h"
#include "FWMacro.h"
#include "FWObject.h"
#include "../OPMath/Math.h"
using namespace OPMath;
#include <string>
#include <map>
using std::string;

struct FWMAIN_API SFWVertex
{
	float x, y, z;
	SFWVertex(): x(0), y(0), z(0)	{}
	SFWVertex( float xI, float yI, float zI):
		x(xI), y(yI), z(zI)	{}
	SFWVertex(OPMath::SQuate& qatI):
		x(qatI.m_fX), y(qatI.m_fY), z(qatI.m_fZ)	{}
};

class FWMAIN_API CFWCoord: public CCoord
{
public: friend class CFWAABBBox;
public:
	CFWCoord()	{}
	CFWCoord(const CMatrix& rMatI)
	{
		this->transfmByMat(rMatI);
	}
public:
	/*
	*	(mode:	"C[X" + X + "Y" + Y + "Z" + Z +"]" +
	*			"X[X" + X + "Y" + Y + "Z" + Z +"]" +
	*			"Y[X" + X + "Y" + Y + "Z" + Z +"]" +
	*			"Z[X" + X + "Y" + Y + "Z" + Z +"]")
	*/
	string convertToStr() const;
	CFWCoord& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
private:
	static std::string::size_type collectQatFromStr(
		string strI,//String source
		string::size_type& rFndOff,//From where find color
		string strQatI,//Quaternion name
		SQuate& rQatO);//Input color
};

class FWMAIN_API CFWMatrix: public OPMath::CMatrix
{
public:
	// Convert matrix to/from string
	// (mode:	"X" + x + "Y" + y + "Z" + z + "H" + h + 
	//			"X" + x + "Y" + y + "Z" + z + "H" + h + 
	//			"X" + x + "Y" + y + "Z" + z + "H" + h + 
	//			"X" + x + "Y" + y + "Z" + z + "H" + h)
	string convertToStr() const;
	CFWMatrix& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
};

struct FWMAIN_API SFWQuate: public SQuate
{
	SFWQuate()	{}
	SFWQuate(const SQuate& rQatI):
		SQuate( rQatI.m_fX, rQatI.m_fY, rQatI.m_fZ, rQatI.m_fH)	{}
	SFWQuate( float fXI, float fYI, float fZI, float fHI = 1):
		SQuate( fXI, fYI, fZI, fHI)	{}
	// Convert quaternion to/from string
	// (mode: "X" + x + "Y" + y + "Z" + z + "H" + h)
	string convertToStr() const;
	SFWQuate& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
};

class FWMAIN_API CFWAABBBox: public CBoxAABB
{
public:
	CFWAABBBox()	{}
	CFWAABBBox(const CBoxAABB& rBoxI): CBoxAABB(rBoxI)	{}
	CFWAABBBox( CPoint poiLTF, CPoint poiRBB):
		CBoxAABB( poiLTF, poiRBB)	{}
public:
	// Convert AABB box to/from string
	// (mode:	"LTF[X" + poiLT.x + "Y" + poiLT.y + "Z" + poiLT.z + "H" + poiLT.h]
	//			"RBB[X" + poiRB.x + "Y" + poiRB.y + "Z" + poiRB.z + "H" + poiRB.h])
	string convertToStr() const;
	CFWAABBBox& convertFromStr(string strI);
	static bool chkStrCanCvt(string strI);
};

// +_+: Force must be normal vector, and the length be the force
//	The special force was zero length, that means the force is unlimited
struct FWMAIN_API SFWForce: public CRay
{
	SFWForce()	{}
	SFWForce( float fXI, float fYI, float fZI)
	{
		m_fX = fXI; m_fY = fYI; m_fZ = fZI;
	}
	SFWForce(const CRay& rayI): CRay(rayI)	{}
	CVector genAccel(float fMassI) const
	{
		return CVector(*this)/fMassI;
	}
	void setForce(float fFcI)
	{
		this->m_fLength = fFcI;
	}
};