#pragma once
#include <map>
#include <string>
#include "windows.h"
#include "OPTools.h"

using std::string;

namespace OPTools
{
	template<typename T> class TEMPLATE_API CStatus
	{
	protected:
		std::map<T, bool> m_mpSttSet;
	public:
		void addStt( T tSttI, bool bEnbI = false)
		{
			if(m_mpSttSet.find(tSttI) == m_mpSttSet.end())
			{
				m_mpSttSet.insert(std::map<T, bool>::value_type( tSttI, bEnbI));
			}
			else
			{
				string strExcDsc = "the status has existed!";
				throw std::runtime_error(strExcDsc);
			}
		}

		void enable( T tSttI, bool bEnbI = true)
		{
			std::map<T, bool>::iterator iterFnd = m_mpSttSet.find(tSttI);
			if(iterFnd != m_mpSttSet.end())
			{
				iterFnd->second = bEnbI;
			}
			else
			{
				string strExcDsc = "the status was not exited!";
				throw std::runtime_error(strExcDsc);
			}
		}

		bool isEnable(T tSttI) const
		{
			std::map<T, bool>::const_iterator iterFnd = m_mpSttSet.find(tSttI);
			if(iterFnd != m_mpSttSet.end())
			{
				return iterFnd->second;
			}
			else
			{
				string strExcDsc = "the status was not exited!";
				throw std::runtime_error(strExcDsc);
			}
		}

		bool isAnyEnable() const
		{
			std::map<T, bool>::const_iterator iter = m_mpSttSet.begin();
			for( ; iter != m_mpSttSet.end(); ++iter)
			{
				if(iter->second)	{ return true; }
			}

			return false;
		}

		bool hasStt(T tIdI) const
		{
			return m_mpSttSet.find(tIdI) != m_mpSttSet.end();
		}

		bool isEmpty() const	{ return m_mpSttSet.empty(); }
	};
};