#pragma once
#pragma warning(disable:4251)
#include <vector>
#include <string>
#include "windows.h"
#include "OPTools.h"

using std::string;

namespace OPTools
{
	/*****
	*	Ex Bit Set
	*
	*/
	class OP_MN_API CBitSet
	{
	public:
		static const UINT s_uByteSz;
	protected:
		typedef std::vector<BYTE> AryByte;
		AryByte m_ayBitDat;
	public:
		CBitSet()	{ m_ayBitDat.clear(); }
		CBitSet(UINT uSetSzI)	{ m_ayBitDat.resize(uSetSzI); }
		CBitSet(std::vector<BYTE> aySetI): m_ayBitDat(aySetI)	{}
		CBitSet(string strBitI);
	public:
		virtual void set( UINT uLocI, bool bSetI = true);
		virtual bool reset(UINT uLocI);
		virtual CBitSet clsAs0Except(UINT uLocI);
		virtual CBitSet clsAs1Except(UINT uLocI);
		void clear(bool bAllSetI = false);
		UINT size() const	{ return m_ayBitDat.size(); }
		void resize(UINT uSetSzI)	{ m_ayBitDat.resize(uSetSzI); }
		bool any() const;
	public:
		bool operator[](UINT uOffI) const;
		bool operator==(const CBitSet& rBstI) const;
		bool operator!=(const CBitSet& rBstI) const
		{ return !(*this == rBstI); }
		CBitSet operator>>(UINT uOffI) const;
		CBitSet operator<<(UINT uOffI) const;
		CBitSet operator&(const CBitSet& rBstI) const;
		CBitSet& operator&=(const CBitSet& rBstI)
		{ *this = *this & rBstI; return *this; }
		CBitSet operator|(const CBitSet& rBstI) const;
		CBitSet& operator|=(const CBitSet& rBstI)
		{ *this = *this | rBstI; return *this; }
		CBitSet operator^(const CBitSet& rBstI) const;
	public:
		//L(Low) -> R(High)
		virtual string tunToStr() const;
		virtual CBitSet& reTunFmStr(string strValI);
	};
}