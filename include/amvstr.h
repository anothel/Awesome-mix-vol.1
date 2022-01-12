#include <cctype>
#include <cstring>

#include "include/amvmem.h"
#include "include/amvsimpstr.h"
#include "include/amvdefine.h"
#include "include/bstringt.h"

namespace awesome_ATL {

class CAtlStringMgr :
	public IAtlStringMgr
{
public:
	CAtlStringMgr(_In_opt_ IAtlMemMgr* pMemMgr = NULL) throw() :
		m_pMemMgr( pMemMgr )
	{
		m_nil.SetManager( this );
	}
	virtual ~CAtlStringMgr() throw()
	{
	}
	void SetMemoryManager(_In_ IAtlMemMgr* pMemMgr) throw()
	{
		ATLASSUME( m_pMemMgr == NULL );
		m_pMemMgr = pMemMgr;
	}

	static IAtlStringMgr* GetInstance()
	{
		static CWin32Heap strHeap;
		static CAtlStringMgr strMgr(&strHeap);

		return &strMgr;
	}
// IAtlStringMgr
public:
	virtual BStringData* Allocate(
		_In_ int nChars,
		_In_ int nCharSize) throw()
	{
		ATLENSURE_RETURN_VAL( nChars>=0, NULL );

		size_t nTotalSize;
		BStringData* pData;
		size_t nDataBytes;

		if( FAILED(::awesome_ATL::AtlAdd(&nChars, nChars, 1)) )
		{
			return NULL;
		}

		int nAlignedChars = ::awesome_ATL::AtlAlignUp( nChars, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.
		ATLENSURE_RETURN_VAL( nChars<=nAlignedChars, NULL );

		if(	FAILED(::awesome_ATL::AtlMultiply(&nDataBytes, static_cast<size_t>(nAlignedChars), static_cast<size_t>(nCharSize))) ||
			FAILED(::awesome_ATL::AtlAdd(&nTotalSize, static_cast<size_t>(sizeof( BStringData )), nDataBytes)))
		{
			return NULL;
		}
		pData = static_cast< BStringData* >( m_pMemMgr->Allocate( nTotalSize ) );
		if( pData == NULL )
		{
			return( NULL );
		}
		pData->pStringMgr = this;
		pData->nRefs = 1;
		pData->nAllocLength = nAlignedChars - 1;
		pData->nDataLength = 0;

		return( pData );
	}
	virtual void Free(_In_ BStringData* pData) throw()
	{
		ATLASSUME(pData != NULL);
		ATLASSERT(pData->pStringMgr == this);

		m_pMemMgr->Free( pData );
	}
	virtual BStringData* Reallocate(
		_Inout_ BStringData* pData,
		_In_ int nChars,
		_In_ int nCharSize) throw()
	{
		ATLENSURE_RETURN_VAL( nChars>=0, NULL );
		ATLASSERT( pData->pStringMgr == this );

		BStringData* pNewData;
		ULONG nTotalSize;
		ULONG nDataBytes;

		if( FAILED(::awesome_ATL::AtlAdd(&nChars, nChars, 1)) )
		{
			return NULL;
		}

		int nAlignedChars = ::awesome_ATL::AtlAlignUp( nChars, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.
		ATLENSURE_RETURN_VAL( nChars<=nAlignedChars, NULL );

		if(	FAILED(::awesome_ATL::AtlMultiply(&nDataBytes, static_cast<ULONG>(nAlignedChars), static_cast<ULONG>(nCharSize))) ||
			FAILED(::awesome_ATL::AtlAdd(&nTotalSize, static_cast<ULONG>(sizeof( BStringData )), nDataBytes)))
		{
			return NULL;
		}
		pNewData = static_cast< BStringData* >( m_pMemMgr->Reallocate( pData, nTotalSize ) );
		if( pNewData == NULL )
		{
			return NULL;
		}
		pNewData->nAllocLength = nAlignedChars - 1;

		return pNewData;
	}
	virtual BStringData* GetNilString() throw()
	{
		m_nil.AddRef();
		return &m_nil;
	}
	virtual IAtlStringMgr* Clone() throw()
	{
		return this;
	}

protected:
	IAtlMemMgr* m_pMemMgr;
	CNilStringData m_nil;
private:
};

template <class ChTraits>
inline typename ChTraits::PCXSTR strstrT(typename ChTraits::PCXSTR pStr,typename ChTraits::PCXSTR pCharSet);

template< typename _CharType = char >
class ChTraitsOS :
	public ChTraitsBase< _CharType >
{
public:
	static int tclen(_In_z_ LPCSTR p) throw()
	{
		ATLASSERT(p != NULL);
		LPCSTR pnext = CharNext(p);
		return ((pnext-p)>1) ? 2 : 1;
	}
	static LPCSTR strstr(
		_In_z_ LPCSTR pStr,
		_In_z_ LPCSTR pCharSet) throw()
	{
		return strstrT< ChTraitsOS<typename ChTraitsBase<_CharType>::XCHAR> >(pStr,pCharSet);
	}
	static int strspn(
		_In_z_ const _CharType* pStr,
		_In_z_ const _CharType* pCharSet) throw()
	{
		ATLASSERT(pStr != NULL);
		int nRet = 0;
		_CharType* p = const_cast<_CharType*>(pStr);
		while (*p != 0)
		{
			_CharType* pNext = CharNext(p);
			if(pNext > p + 1)
			{
				if(strchr_db(pCharSet, *p, *(p+1)) == NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(strchr(pCharSet, *p) == NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}
	static int strcspn(
		_In_z_ const _CharType* pStr,
		_In_z_ const _CharType* pCharSet) throw()
	{
		ATLASSERT(pStr != NULL);
		int nRet = 0;
		_CharType* p = const_cast<_CharType*>(pStr);
		while (*p != 0)
		{
			_CharType* pNext = CharNext(p);
			if(pNext > p + 1)
			{
				if(strchr_db(pCharSet, *p, *(p+1)) != NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(strchr(pCharSet, *p) != NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}
	static LPCSTR strpbrk(
		_In_z_ LPCSTR p,
		_In_z_ LPCSTR lpszCharSet) throw()
	{
		int nRet=0;
		nRet=strcspn(p,lpszCharSet);
		if (p[nRet]){
			p+=nRet;
			return p;
		}
		return NULL;
	}

	static _CharType* CharNext(_In_ const _CharType* p) throw()
	{
		return AtlCharNext(p);
	}

	static int IsDigit(_In_ _CharType ch) throw()
	{
		return isdigit(ch);
	}

	static int IsSpace(_In_ _CharType ch) throw()
	{
		return isspace(ch);
	}

	static int StringCompare(
		_In_z_ const _CharType* pstrOne,
		_In_z_ const _CharType* pstrOther) throw()
	{
		return strcmp((LPCSTR) pstrOne, (LPCSTR) pstrOther);
	}

	static int StringCompareIgnore(
		_In_z_ const _CharType* pstrOne,
		_In_z_ const _CharType* pstrOther) throw()
	{
		return strcasecmp((LPCSTR) pstrOne, (LPCSTR) pstrOther);
	}

	static LPCSTR StringFindString(
		_In_z_ LPCSTR pstrBlock,
		_In_z_ LPCSTR pstrMatch) throw()
	{
		return strstr(pstrBlock, pstrMatch);
	}

	static LPSTR StringFindString(
		_In_z_ LPSTR pszBlock,
		_In_z_ LPCSTR pszMatch) throw()
	{
		return( const_cast< LPSTR >( StringFindString( const_cast< LPCSTR >( pszBlock ), pszMatch ) ) );
	}

	static LPCSTR StringFindChar(
		_In_z_ LPCSTR pszBlock,
		_In_ char chMatch) throw()
	{
		return strchr(pszBlock, chMatch);
	}

	static LPCSTR StringFindCharRev(
		_In_z_ LPCSTR psz,
		_In_ char ch) throw()
	{
		return strrchr(psz, ch);
	}

	static LPCSTR StringScanSet(
		_In_z_ LPCSTR pszBlock,
		_In_z_ LPCSTR pszMatch) throw()
	{
		return strpbrk(pszBlock, pszMatch);
	}

	static int StringSpanIncluding(
		_In_z_ const _CharType* pstrBlock,
		_In_z_ const _CharType* pstrSet) throw()
	{
		return strspn(pstrBlock, pstrSet);
	}

	static int StringSpanExcluding(
		_In_z_ const _CharType* pstrBlock,
		_In_z_ const _CharType* pstrSet) throw()
	{
		return strcspn(pstrBlock, pstrSet);
	}

	_ATL_INSECURE_DEPRECATE("ChTraitsOS::StringUppercase must be passed a buffer size")
	static _CharType* StringUppercase(_Inout_z_ _CharType* psz) throw()
	{
		return CharUpperA( psz );
	}

	_ATL_INSECURE_DEPRECATE("ChTraitsOS::StringLowercase must be passed a buffer size")
	static _CharType* StringLowercase(_Inout_z_ _CharType* psz) throw()
	{
		return CharLowerA( psz );
	}

	static _CharType* StringUppercase(
		_Inout_updates_z_(size) _CharType* psz,
		_In_ size_t size) throw()
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			AtlThrow("Invalid arguments");
		}
		DWORD dwSize=static_cast<DWORD>(size);
		CharUpperBuffA( psz, dwSize );
		return psz;
	}

	static _CharType* StringLowercase(
		_Inout_updates_z_(size) _CharType* psz,
		_In_ size_t size) throw()
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			AtlThrow("Invalid arguments");
		}
		DWORD dwSize=static_cast<DWORD>(size);
		CharLowerBuffA( psz, dwSize );
		return psz;
	}

	static _CharType* StringReverse(_Inout_z_ _CharType* psz) throw()
	{
		return _strrev( psz );
	}

	static int GetBaseTypeLength(_In_z_ const char* pszSrc) throw()
	{
		// Returns required buffer length in XCHARs
    if (pszSrc == NULL) return 0;
    return static_cast<int>(::strlen(pszSrc));
	}

	static int GetBaseTypeLength(
		_In_z_ const char* pszSrc,
		_In_ int nLength) throw()
	{
		(void)pszSrc;
		// Returns required buffer length in XCHARs
		return nLength;
	}

	static void ConvertToBaseType(
		_Out_writes_(nDestLength) _CharType* pszDest,
		_In_ int nDestLength,
		_In_reads_(nSrcLength) const char* pszSrc,
		_In_ int nSrcLength = -1) throw()
	{
		if (nSrcLength == -1) { nSrcLength=1 + GetBaseTypeLength(pszSrc); }
		// nLen is in chars
		memcpy(pszDest, pszSrc, nSrcLength*sizeof(_CharType));
	}

	static int SafeStringLen(_In_opt_z_ const char* psz) throw()
	{
		// returns length in bytes
		return (psz != NULL) ? static_cast<int>(strlen(psz)) : 0;
	}

};

template <class ChTraits>
inline typename ChTraits::PCXSTR strstrT(
	_In_ typename ChTraits::PCXSTR pStr,
	_In_ typename ChTraits::PCXSTR pCharSet)
{
	ATLASSERT(pStr != NULL);
	size_t nCharSetLen = ChTraits::GetBaseTypeLength(pCharSet);
	if (nCharSetLen == 0)
		return pStr;
	//strlen returns length in bytes, not chars.
	size_t nStrLen = ChTraits::GetBaseTypeLength(pStr);
	typename ChTraits::PCXSTR pStrEnd=pStr + nStrLen;
	const typename ChTraits::XCHAR* pMatch;
	const typename ChTraits::XCHAR* pStart = pStr;
	while ((pMatch = ChTraits::strchr(pStart, *pCharSet)) != NULL)
	{
		size_t nCharsLeftInStr=pStrEnd - pMatch;
		if (nCharsLeftInStr < nCharSetLen)
		{
			break;
		}
		if (memcmp(pMatch, pCharSet, nCharSetLen*sizeof(typename ChTraits::XCHAR)) == 0)
		{
			return pMatch;
		}
		pStart = ChTraits::CharNext(pMatch);
	}

	return NULL;
}

template< typename _BaseType = char, class StringIterator = ChTraitsOS< _BaseType > >
class StrTraitATL :	public StringIterator
{
public:
	static IAtlStringMgr* GetDefaultManager() throw()
	{
		return CAtlStringMgr::GetInstance();
	}
};

typedef BStringT< char, StrTraitATL< char > > CAtlString;


}
typedef awesome_ATL::CAtlString BString;
