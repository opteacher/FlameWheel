#pragma once
#include <vector>
#include <string>
#include <set>
#include "windows.h"
#include "String.h"
#include "File.h"

using std::string;
typedef std::vector<string>	VEC_STR;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	{ if(x) { delete(x); (x) = NULL; } }
#endif

#ifndef CHK_ITER_EXS
#define CHK_ITER_EXS( arrayI, valueI)	(arrayI.find(valueI) != arrayI.end())
#endif

namespace OPTools
{
	/*****
	*	pointer handle
	*	use: can set to every type
	*	+_+: can't test the inherit relation
	*/
	class OP_MN_API CHandle
	{
		class placeholder
		{
		public: // destructor
			virtual ~placeholder()	{} 
		public: // queries
			virtual const std::type_info & type() const = 0;
			virtual placeholder * clone() const = 0;
		};

		template<typename ValueType>
		class holder: public placeholder
		{
		public: // constructors 
			holder(const ValueType & value): held(value)	{}
		public: // queries 
			virtual const std::type_info & type() const	{ return typeid(ValueType); }
			virtual placeholder * clone() const			{ return new holder(held); }
		public: // representation 
			ValueType held;
		protected:
			holder(const holder& rHdrI)				{ held = rHdrI.held; }
			holder& operator=(const holder& rHdrI)	{ held = rHdrI.held }
		public:
			ValueType* operator->()	{ return &held; }
			ValueType& operator*()	{ return held; }
		};
	protected:
		static bool chkHdlExist(DWORD dwAddrI);
		static void insInitHdl(DWORD dwAddrI);
		static void rmvInitHdl(DWORD dwAddrI);
	public:
		CHandle(): m_pReal(NULL), m_pUintUse(NULL)	{}

		template<typename T> CHandle(T const& rI):
			m_pReal(new holder<T>(rI)),
			m_pUintUse(new UINT(1)),
			m_bAutoCtl(true)
		{
			insInitHdl((DWORD)this);
		}

		/*template<typename T> CHandle& operator=(T const& rT)
		{
			this->release();
			m_pReal = newEnumAsDword(rI);
			m_pUintUse = new UINT(1);
			m_bAutoCtl = true;
			insInitHdl((DWORD)this);

			return *this;
		}*/

		template<typename T> CHandle& initByIns( T const& rT, bool bAutoCtlI)
		{
			this->release();
			m_pReal = new holder<T>(rI);
			m_pUintUse = new UINT(1);
			m_bAutoCtl = bAutoCtlI;
			insInitHdl((DWORD)this);

			return *this;
		}

		CHandle(const CHandle& rHdlI);

		virtual ~CHandle();
	public:
		//+_+: it will delete self pointer first
		CHandle& operator=(CHandle& rHdlI);

		bool operator==(const CHandle& rHdlI) const;

		template<typename T> holder<T>& turnTo() const
		{
			if(isTypeOf<T>())
			{
				return dynamic_cast<holder<T>&>(*m_pReal);
			}
			throw std::bad_exception();
		}
	public://check function
		bool isNull() const		{ return m_pReal == NULL; }
		bool isEmpty() const	{ return !m_pReal || !m_pUintUse; }
		//check whether ex handle
		virtual bool isExHdl() const	{ return false; }

		//check the type(include all the base class of this class)
		template<typename T> bool isTypeOf() const
		{
			return m_pReal->type() == typeid(T);
		}
	public:
		DWORD getAddress()const		{ return (DWORD)m_pReal; }
		string getTypeNam() const;
	public:
		//enforce clean, use number will be ignored
		void release();
	protected:
		placeholder* m_pReal;
		UINT* m_pUintUse;
		bool m_bAutoCtl;
	};

#ifndef HDL_AVA
#define HDL_AVA( hdl, type)	(!hdl.isNull() && hdl.isTypeOf<type>())
#endif

#ifndef FUNC_GET
#define FUNC_GET( vMem, strMem, tpRet)	tpRet get##strMem##() const	{ return vMem; }
#endif

#ifndef FUNC_SET
#define FUNC_SET( vMem, strMem, tpPam) void set##strMem##(tpPam vInpI)	{ vMem = vInpI; }
#endif

#ifndef FUNC_GS
#define FUNC_GS( vMem, strMem, tpMem)	\
	FUNC_GET( vMem, strMem, tpMem)\
	FUNC_SET( vMem, strMem, tpMem)
#endif
	/*****
	*	extra handle
	*	description: compare to normal, this class add a object name(string),
	*		object id(DWORD), operation id(UINT) value for sending
	*/
	class OP_MN_API CHdlEx: public CHandle
	{
	public:
		CHdlEx(): CHandle(),
			m_strObjNam(""),
			m_dwObjId(0),
			m_uOperId(0)	{}
		CHdlEx(const CHdlEx& rHdlI);
		CHdlEx(const CHandle& rHdlI);
		CHdlEx& operator=(const CHdlEx& rHdlI);
	public:
		FUNC_GS( m_strObjNam, ObjNam, string);
		FUNC_GS( m_dwObjId, ObjID, DWORD);
		FUNC_GS( m_uOperId, OperID, UINT);
		bool hasValue()
		{
			return 0 != m_dwObjId
				|| !m_strObjNam.empty()
				|| NULL != m_pReal;
		}
	public:
		//ex handle return true
		virtual bool isExHdl() const	{ return true; }
	protected:
		string m_strObjNam;//operate object name
		DWORD m_dwObjId;//object ID
		UINT m_uOperId;//operation ID
	};
};