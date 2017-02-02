#pragma once
#include <iostream>
#include "windows.h"
#include "OPTools.h"

namespace OPTools
{
	template<typename T> class TEMPLATE_API CAry2Dim
	{
	protected:
		T** m_pData;
		DWORD m_dwRow;
		DWORD m_dwCol;
	public:
		DWORD getNumRow() const	{ return m_dwRow; }
		DWORD getNumCol() const	{ return m_dwCol; }
	public:
		CAry2Dim():
		m_pData(NULL),
		m_dwRow(0),
		m_dwCol(0)	{}

		CAry2Dim( UINT uRowI, UINT uColI):
		m_dwRow(uRowI), m_dwCol(uColI)
		{
			m_pData = new T*[uRowI];
			memset( m_pData, 0, uRowI*sizeof(T*));

			UINT uSzT = sizeof(T);
			for( UINT i = 0; i < uRowI; ++i)
			{
				m_pData[i] = new T[uColI];
				memset( m_pData[i], 0, uSzT*uColI);
			}
		}

		CAry2Dim(const CAry2Dim& rAryI)
		{
			m_dwRow = rAryI.m_dwRow;
			m_dwCol = rAryI.m_dwCol;

			m_pData = new T*[m_dwRow];
			memset( m_pData, 0, m_dwRow*sizeof(T*));

			UINT uSzT = sizeof(T);
			for( UINT i = 0; i < m_dwRow; ++i)
			{
				m_pData[i] = new T[m_dwCol];
				memset( m_pData[i], 0, m_dwCol*uSzT);
				for( UINT j = 0; j < m_dwCol; ++j)
				{
					m_pData[i][j] = rAryI.m_pData[i][j];
				}
			}
		}

		virtual ~CAry2Dim()	{ release(); }

		void release()
		{
			if(m_pData)
			{
				for( UINT i = 0; i < m_dwRow; ++i)
				{
					if(m_pData[i])	{ delete[] m_pData[i]; }
				}
				delete[] m_pData;
			}
		}

		CAry2Dim& operator=(const CAry2Dim& rAryI)
		{
			release();
			
			m_dwRow = rAryI.m_dwRow;
			m_dwCol = rAryI.m_dwCol;

			m_pData = new T*[m_dwRow];
			memset( m_pData, 0, m_dwRow*sizeof(T*));

			UINT uSzT = sizeof(T);
			for( UINT i = 0; i < m_dwRow; ++i)
			{
				m_pData[i] = new T[m_dwCol];
				memset( m_pData[i], 0, m_dwCol*uSzT);
				for( UINT j = 0; j < m_dwCol; ++j)
				{
					m_pData[i][j] = rAryI.m_pData[i][j];
				}
			}

			return *this;
		}
	public:
		T get( DWORD dwRowI, DWORD dwColI) const
		{
			if(dwRowI >= m_dwRow || m_dwCol >= m_dwCol)
			{
				throw std::runtime_error("given row and col out of array");
			}
			return m_pData[dwRowI][dwColI];
		}

		void set( DWORD dwRowI, DWORD dwColI, T tValI)
		{
			if(dwRowI >= m_dwRow || m_dwCol >= m_dwCol)
			{
				throw std::runtime_error("given row and col out of array");
			}
			m_pData[dwRowI][dwColI] = tValI;
		}

		T* operator[](DWORD dwRowI)	{ return m_pData[dwRowI]; }
	};
}