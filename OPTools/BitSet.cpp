#pragma once
#include "stdafx.h"
#include "BitSet.h"

using namespace OPTools;

const UINT CBitSet::s_uByteSz(sizeof(BYTE)*8);

CBitSet::CBitSet(string strBitI)
{
	reTunFmStr(strBitI);
}

void CBitSet::set( UINT uLocI, bool bSetI)
{
	//parameter check
	UINT uIndSet = uLocI/s_uByteSz;
	UINT uLocSet = uLocI%s_uByteSz;
	if(uIndSet >= m_ayBitDat.size())	{ return; }

	//get bit and set
	BYTE& rByteSet = m_ayBitDat[uIndSet];
	BYTE byteTmp = 1<<uLocSet;
	if(bSetI)	{ rByteSet |= byteTmp; }
	else		{ rByteSet &= ~byteTmp; }
}

bool CBitSet::reset(UINT uLocI)
{
	//parameter check
	UINT uIndSet = uLocI/s_uByteSz;
	UINT uLocSet = uLocI%s_uByteSz;
	if(uIndSet >= m_ayBitDat.size())	{ return false; }

	//get bit and reset
	BYTE& rByteSet = m_ayBitDat[uIndSet];
	BYTE byteTmp = rByteSet>>uLocSet&1;
	byteTmp = byteTmp<<uLocSet;
	bool bSetRet = (byteTmp != 0);
	if(byteTmp == 0)
	{
		byteTmp = 1<<uLocSet;
		rByteSet |= byteTmp;
	}
	else
	{
		byteTmp = ~(1<<uLocSet);
		rByteSet &= byteTmp;
	}

	return bSetRet;
}

CBitSet CBitSet::clsAs0Except( UINT uLocI )
{
	CBitSet bstRet = *this;
	CBitSet bstMsk(bstRet.size());
	bstMsk.set(uLocI);
	bstRet &= bstMsk;
	return bstRet;
}

CBitSet CBitSet::clsAs1Except( UINT uLocI )
{
	CBitSet bstRet = *this;
	CBitSet bstMsk(bstRet.size());
	bstMsk.clear(true);
	bstMsk.set( uLocI, false);
	bstRet |= bstMsk;
	return bstRet;
}

void CBitSet::clear(bool bAllSetI)
{
	for( UINT i = 0; i < m_ayBitDat.size(); ++i)
	{
		m_ayBitDat[i] = (bAllSetI) ? 0xff : 0x00;
	}
}

bool CBitSet::any() const
{
	for( UINT i = 0; i < m_ayBitDat.size(); ++i)
	{ if(!m_ayBitDat[i])	{ return true; } }

	return false;
}

#ifndef CTL_LOOP_BEG
#define CTL_LOOP_BEG(condition, numLoop)	\
	UINT uIndAcc = 0; while((condition) && uIndAcc < numLoop)	{
#endif

#ifndef CTL_LOOP_END
#define CTL_LOOP_END	++uIndAcc; }
#endif

bool CBitSet::operator==(const CBitSet& rBstI) const
{
	AryByte ayRefISet = rBstI.m_ayBitDat;
	UINT uThisSz = m_ayBitDat.size();
	UINT uRefISz = ayRefISet.size();
	if(uThisSz != uRefISz)	{ return false; }
	for( UINT i = 0; i < uThisSz; ++i)
	{
		if(m_ayBitDat[i] != ayRefISet[i])	{ return false; }
	}
	return true;
}

bool CBitSet::operator[](UINT uOffI) const
{
	UINT uIndByte = uOffI/s_uByteSz;
	UINT uIndLeft = uOffI%s_uByteSz;
	if(uIndByte >= m_ayBitDat.size())
	{
		return false;//exception
	}
	BYTE byteTmp = m_ayBitDat[uIndByte];
	BYTE byteMsk = 0;
	byteMsk |= 1<<uIndLeft;

	return (byteTmp & byteMsk) != 0;
}

CBitSet CBitSet::operator>>(UINT uOffI) const
{
	//if offset bigger than byte size, set to zero
	UINT uNumByte = uOffI/s_uByteSz;
	UINT uNumLft = uOffI%s_uByteSz;
	UINT uLftOff = s_uByteSz - uNumLft;
	AryByte ayBstTmp = m_ayBitDat;
	if(uNumByte != 0)
	{
		typedef AryByte::iterator IterByte;
		IterByte iter = ayBstTmp.begin();
		CTL_LOOP_BEG( iter != ayBstTmp.end(), uNumByte)
			iter = ayBstTmp.erase(iter);
			ayBstTmp.push_back(0);
		CTL_LOOP_END
	}

	//do right
	CBitSet bstRet(m_ayBitDat.size());
	BYTE byteOff = 0;
	for( UINT i = 0; i< m_ayBitDat.size(); ++i)
	{
		BYTE byteTmp = ayBstTmp[i];
		byteOff = byteTmp<<uLftOff;
		bstRet.m_ayBitDat[i] = byteTmp>>uNumLft;
		if(i == 0)	{  continue; }
		bstRet.m_ayBitDat[i - 1] |= byteOff;
	}

	return bstRet;
}

CBitSet CBitSet::operator<<(UINT uOffI) const
{
	//if offset bigger than byte size, set to zero
	UINT uNumByte = uOffI/s_uByteSz;
	UINT uNumLft = uOffI%s_uByteSz;
	UINT uLftOff = s_uByteSz - uNumLft;
	AryByte ayBstTmp = m_ayBitDat;
	if(uNumByte != 0)
	{
		for( UINT i = 0, j = 0; i < m_ayBitDat.size(); ++i)
		{
			if(i < uNumByte)	{ ayBstTmp[0]; }
			else                { ayBstTmp[i] = m_ayBitDat[j]; ++j; }
		}
	}

	//do left
	CBitSet bstRet(m_ayBitDat.size());
	BYTE byteOff = 0;
	for( UINT i = 0; i< m_ayBitDat.size(); ++i)
	{
		BYTE byteTmp = ayBstTmp[i];
		bstRet.m_ayBitDat[i] = byteTmp<<uNumLft;
		if(i != 0)
		{
			bstRet.m_ayBitDat[i] |= byteOff;
		}
		byteOff = byteTmp>>uLftOff;
	}

	return bstRet;
}

#ifndef BSET_OPERATOR
#define BSET_OPERATOR(sign)	\
	CBitSet CBitSet::operator sign(const CBitSet& rBstI) const	{\
		/*resize to larger one*/\
		AryByte ayByteTs = m_ayBitDat;\
		AryByte ayByteIp = rBstI.m_ayBitDat;\
		UINT uSetSz = ayByteTs.size();\
		if(ayByteTs.size() > ayByteIp.size())	{\
			uSetSz = ayByteTs.size();\
			ayByteIp.resize(uSetSz);\
		}\
		else if(ayByteTs.size() < ayByteIp.size())	{\
			uSetSz = ayByteIp.size();\
			ayByteTs.resize(uSetSz);\
		}\
		/*equals will not process*/\
		CBitSet bstRet;\
		for( UINT i = 0; i < uSetSz; ++i)\
		{\
			bstRet.m_ayBitDat.push_back(ayByteTs[i] sign ayByteIp[i]);\
		}\
		return bstRet;\
	}
#endif

BSET_OPERATOR(&)
BSET_OPERATOR(|)
BSET_OPERATOR(^)

string CBitSet::tunToStr() const
{
	string strRet = "";
	for( UINT i = 0; i < m_ayBitDat.size(); ++i)
	{
		BYTE byteTmp = m_ayBitDat[i];
		for( UINT j = 0; j < s_uByteSz; ++j)
		{
			BYTE byteOut = byteTmp>>j&1;
			if(byteOut != 0)	{ strRet += '1'; }
			else				{ strRet += '0'; }
		}
	}

	return strRet;
}

CBitSet& CBitSet::reTunFmStr(string strValI)
{
	BYTE byteColc = 0;
	for( UINT i = 0, j = 0; i < strValI.size(); ++i, ++j)
	{
		//check whether a BYTE was be full filled
		if(j >= s_uByteSz)
		{
			m_ayBitDat.push_back(byteColc);
					
			//data reset
			byteColc = 0;
			j = 0;
		}

		if(strValI[i] != '0' && strValI[i] != ' ')
		{
			//all not '0' and space will be set as '1'
			BYTE byteTmp = 0;
			byteTmp = 1;
			byteTmp = byteTmp<<j;
			byteColc |= byteTmp;
		}
	}

	m_ayBitDat.push_back(byteColc);

	return *this;
}

