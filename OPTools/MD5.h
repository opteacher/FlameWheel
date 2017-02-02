#pragma once
//Author:ArcherSC
#include "windows.h"
#include <string>
#include "OPTools.h"

namespace OPTools
{
	class OP_MN_API MD5
	{
	private:
		unsigned int m_Content[16];
		unsigned int m_ContentLen;
		unsigned int m_TotalLen;
		static unsigned int m_T[64];
		unsigned int m_A, m_B, m_C, m_D;
	private:
		inline void FF(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, int k, int i, int s);
		inline void GG(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, int k, int i, int s);
		inline void HH(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, int k, int i, int s);
		inline void II(unsigned int& a, unsigned int b, unsigned int c, unsigned int d, int k, int i, int s);
		inline unsigned int roateLeft(unsigned int a, unsigned int c);

		void transform();
	public:
		//initialize
		void beginCal();
		//no more data input, will set bit
		void endCal();
		//after initialized, input data, before the endCal, can continue input data
		void addData(const char* data, unsigned int size);
		//after calculated, get code
		std::string getCode();
		void clearMemoryHelper(char* p);
	};
}