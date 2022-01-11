#include "include/atlsimpstr.h"
#include "include/atlalloc.h"

namespace awesome_ATL {

#ifndef _WIN32
#define CSTRING_EXPLICIT explicit
#endif  // _WIN32



template< typename _CharType = char >
class ChTraitsCRT :	public ChTraitsBase< _CharType >
{
public:

};


template <typename BaseType, class StringTraits>
class CStringT : public CSimpleStringT<BaseType> {
 public:
  typedef CSimpleStringT<BaseType> CThisSimpleString;
  typedef StringTraits StrTraits;
  typedef typename CThisSimpleString::XCHAR XCHAR;
  typedef typename CThisSimpleString::PXSTR PXSTR;
  typedef typename CThisSimpleString::PCXSTR PCXSTR;

 public:
  CStringT() throw() : CThisSimpleString(StringTraits::GetDefaultManager()) {}
  explicit CStringT(_In_ IAtlStringMgr* pStringMgr) throw()
      : CThisSimpleString(pStringMgr) {}

  static void __cdecl Construct(_In_ CStringT* pString) {
    new (pString) CStringT;
  }

  // Copy constructor
  CStringT(_In_ const CStringT& strSrc) : CThisSimpleString(strSrc) {}

  // Construct from CSimpleStringT
  operator CSimpleStringT<BaseType>&() {
    return *(
        CSimpleStringT<BaseType>*)this;
  }

  CStringT(_In_ const CSimpleStringT<BaseType>& strSrc)
      : CThisSimpleString(strSrc) {}

  CStringT(_In_opt_z_ const XCHAR* pszSrc)
      : CThisSimpleString(StringTraits::GetDefaultManager()) {
    if (!CheckImplicitLoad(pszSrc)) {
      *this = pszSrc;
    }
  }

  CStringT(_In_opt_z_ const XCHAR* pszSrc, _In_ IAtlStringMgr* pStringMgr)
      : CThisSimpleString(pStringMgr) {
    if (!CheckImplicitLoad(pszSrc)) {
      *this = pszSrc;
    }
  }

	CSTRING_EXPLICIT CStringT(_In_z_ const unsigned char* pszSrc) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		*this = reinterpret_cast< const char* >( pszSrc );
	}

	CStringT(
			_In_opt_z_ const unsigned char* pszSrc,
			_In_ IAtlStringMgr* pStringMgr) :
		CThisSimpleString( pStringMgr )
	{
		*this = reinterpret_cast< const char* >( pszSrc );
	}


#define _CSTRING_CHAR_T char

	CSTRING_EXPLICIT CStringT(
			_In_ _CSTRING_CHAR_T ch,
			_In_ int nLength = 1) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			PXSTR pszBuffer = this->GetBuffer( nLength );
			StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
			this->ReleaseBufferSetLength( nLength );
		}
	}
#undef _CSTRING_CHAR_T


public:

	CStringT(
			_In_reads_(nLength) const XCHAR* pch,
			_In_ int nLength) :
		CThisSimpleString( pch, nLength, StringTraits::GetDefaultManager() )
	{
	}

	CStringT(
			_In_reads_(nLength) const XCHAR* pch,
			_In_ int nLength,
			_In_ IAtlStringMgr* pStringMgr) :
		CThisSimpleString( pch, nLength, pStringMgr )
	{
	}

	// Destructor
	~CStringT() throw()
	{
printf("[jpk] ~CStringT() \n");
	}

	// Assignment operators
	CStringT& operator=(_In_ const CStringT& strSrc)
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}

	CStringT& operator=(_In_ const CSimpleStringT<BaseType>& strSrc)
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}

	CStringT& operator=(_In_opt_z_ PCXSTR pszSrc)
	{
		CThisSimpleString::operator=( pszSrc );

		return( *this );
	}



	CStringT& operator=(_In_ XCHAR ch)
	{
		XCHAR ach[2] = { ch, 0 };

		return( operator=( ach ) );
	}



	CStringT& operator+=(_In_ const CThisSimpleString& str)
	{
		CThisSimpleString::operator+=( str );

		return( *this );
	}

	CStringT& operator+=(_In_z_ PCXSTR pszSrc)
	{
		CThisSimpleString::operator+=( pszSrc );

		return( *this );
	}
	template< int t_nSize >
	CStringT& operator+=(_In_ const CStaticString< XCHAR, t_nSize >& strSrc)
	{
		CThisSimpleString::operator+=( strSrc );

		return( *this );
	}

	CStringT& operator+=(_In_ XCHAR ch)
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}


	CStringT& operator+=(_In_ unsigned char ch)
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	// Override from base class
	IAtlStringMgr* GetManager() const throw()
	{
		IAtlStringMgr* pStringMgr = CThisSimpleString::GetManager();
		if(pStringMgr) { return pStringMgr; }

		pStringMgr = StringTraits::GetDefaultManager();
		return pStringMgr->Clone();
	}

	// Comparison

	int Compare(_In_z_ PCXSTR psz) const
	{
		ATLENSURE( AtlIsValidString( psz ) );
		_Analysis_assume_(psz); // AtlIsValidString guarantees that psz != NULL
		return( StringTraits::StringCompare( this->GetString(), psz ) );
	}

	int CompareNoCase(_In_z_ PCXSTR psz) const
	{
		ATLENSURE( AtlIsValidString( psz ) );
		_Analysis_assume_(psz); // AtlIsValidString guarantees that psz != NULL
		return( StringTraits::StringCompareIgnore( this->GetString(), psz ) );
	}

	int Collate(_In_z_ PCXSTR psz) const throw()
	{
		ATLASSERT( AtlIsValidString( psz ) );
		return( StringTraits::StringCollate( this->GetString(), psz ) );
	}

	int CollateNoCase(_In_z_ PCXSTR psz) const throw()
	{
		ATLASSERT( AtlIsValidString( psz ) );
		return( StringTraits::StringCollateIgnore( this->GetString(), psz ) );
	}

	// Advanced manipulation

	// Delete 'nCount' characters, starting at index 'iIndex'
	int Delete(
		_In_ int iIndex,
		_In_ int nCount = 1)
	{
		if( iIndex < 0 )
			iIndex = 0;

		if( nCount < 0 )
			nCount = 0;

		int nLength = this->GetLength();
		if( (::awesome_ATL::AtlAddThrow(nCount, iIndex)) > nLength )
		{
			nCount = nLength-iIndex;
		}
		if( nCount > 0 )
		{
			int nNewLength = nLength-nCount;
			int nXCHARsToCopy = nLength-(iIndex+nCount)+1;
			PXSTR pszBuffer = this->GetBuffer();
			memmove_s( pszBuffer+iIndex, nXCHARsToCopy*sizeof( XCHAR ),
				pszBuffer+iIndex+nCount, nXCHARsToCopy*sizeof( XCHAR ) );
			this->ReleaseBufferSetLength( nNewLength );
		}

		return( this->GetLength() );
	}

	// Insert character 'ch' before index 'iIndex'
	int Insert(
		_In_ int iIndex,
		_In_ XCHAR ch)
	{
		if( iIndex < 0 )
			iIndex = 0;

		if( iIndex > this->GetLength() )
		{
			iIndex = this->GetLength();
		}
		int nNewLength = this->GetLength()+1;

		PXSTR pszBuffer = this->GetBuffer( nNewLength );

		// move existing bytes down
		memmove_s( pszBuffer+iIndex+1, (nNewLength-iIndex)*sizeof( XCHAR ),
			pszBuffer+iIndex, (nNewLength-iIndex)*sizeof( XCHAR ) );
		pszBuffer[iIndex] = ch;

		this->ReleaseBufferSetLength( nNewLength );

		return( nNewLength );
	}

	// Insert string 'psz' before index 'iIndex'
	int Insert(
		_In_ int iIndex,
		_In_z_ PCXSTR psz)
	{
		if( iIndex < 0 )
			iIndex = 0;

		if( iIndex > this->GetLength() )
		{
			iIndex = this->GetLength();
		}

		// nInsertLength and nNewLength are in XCHARs
		int nInsertLength = StringTraits::SafeStringLen( psz );
		int nNewLength = this->GetLength();
		if( nInsertLength > 0 )
		{
			nNewLength += nInsertLength;

			PXSTR pszBuffer = this->GetBuffer( nNewLength );
			// move existing bytes down
			memmove_s( pszBuffer+iIndex+nInsertLength, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ),
				pszBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ) );
			memcpy_s( pszBuffer+iIndex, nInsertLength*sizeof( XCHAR ),
				psz, nInsertLength*sizeof( XCHAR ) );
			this->ReleaseBufferSetLength( nNewLength );
		}

		return( nNewLength );
	}

	// Replace all occurrences of character 'chOld' with character 'chNew'
	int Replace(
		_In_ XCHAR chOld,
		_In_ XCHAR chNew)
	{
		int nCount = 0;

		// short-circuit the nop case
		if( chOld != chNew )
		{
			// otherwise modify each character that matches in the string
			bool bCopied = false;
			PXSTR pszBuffer = const_cast< PXSTR >( this->GetString() );  // We don't actually write to pszBuffer until we've called GetBuffer().

			int nLength = this->GetLength();
			int iChar = 0;
			while( iChar < nLength )
			{
				// replace instances of the specified character only
				if( pszBuffer[iChar] == chOld )
				{
					if( !bCopied )
					{
						bCopied = true;
						pszBuffer = this->GetBuffer( nLength );
					}
					pszBuffer[iChar] = chNew;
					nCount++;
				}
				iChar = int( StringTraits::CharNext( pszBuffer+iChar )-pszBuffer );
			}
			if( bCopied )
			{
				this->ReleaseBufferSetLength( nLength );
			}
		}

		return( nCount );
	}

	// Replace all occurrences of string 'pszOld' with string 'pszNew'
	int Replace(
		_In_z_ PCXSTR pszOld,
		_In_z_ PCXSTR pszNew)
	{
		// can't have empty or NULL lpszOld

		// nSourceLen is in XCHARs
		int nSourceLen = StringTraits::SafeStringLen( pszOld );
		if( nSourceLen == 0 )
			return( 0 );
		// nReplacementLen is in XCHARs
		int nReplacementLen = StringTraits::SafeStringLen( pszNew );

		// loop once to figure out the size of the result string
		int nCount = 0;
		{
			PCXSTR pszStart = this->GetString();
			PCXSTR pszEnd = pszStart + this->GetLength();
			while( pszStart < pszEnd )
			{
				PCXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL)
				{
					nCount++;
					pszStart = pszTarget+nSourceLen;
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
		}

		// if any changes were made, make them
		if( nCount > 0 )
		{
			// if the buffer is too small, just
			//   allocate a new buffer (slow but sure)
			int nOldLength = this->GetLength();
			int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;

			PXSTR pszBuffer = this->GetBuffer( __max( nNewLength, nOldLength ) );

			PXSTR pszStart = pszBuffer;
			PXSTR pszEnd = pszStart+nOldLength;

			// loop again to actually do the work
			while( pszStart < pszEnd )
			{
				PXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL )
				{
					int nBalance = nOldLength-int(pszTarget-pszBuffer+nSourceLen);
					memmove_s( pszTarget+nReplacementLen, nBalance*sizeof( XCHAR ),
						pszTarget+nSourceLen, nBalance*sizeof( XCHAR ) );
					memcpy_s( pszTarget, nReplacementLen*sizeof( XCHAR ),
						pszNew, nReplacementLen*sizeof( XCHAR ) );
					pszStart = pszTarget+nReplacementLen;
					pszTarget[nReplacementLen+nBalance] = 0;
					nOldLength += (nReplacementLen-nSourceLen);
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
			ATLASSERT( pszBuffer[nNewLength] == 0 );
			this->ReleaseBufferSetLength( nNewLength );
		}

		return( nCount );
	}

	// Remove all occurrences of character 'chRemove'
	int Remove(_In_ XCHAR chRemove)
	{
		int nLength = this->GetLength();
		PXSTR pszBuffer = this->GetBuffer( nLength );

		PXSTR pszSource = pszBuffer;
		PXSTR pszDest = pszBuffer;
		PXSTR pszEnd = pszBuffer+nLength;

		while( pszSource < pszEnd )
		{
			PXSTR pszNewSource = StringTraits::CharNext( pszSource );
			if( *pszSource != chRemove )
			{
				// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
				// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
				size_t NewSourceGap = (pszNewSource-pszSource);
				PXSTR pszNewDest = pszDest + NewSourceGap;
				size_t i = 0;
				for (i = 0;  pszDest != pszNewDest && i < NewSourceGap; i++)
				{
					*pszDest = *pszSource;
					pszSource++;
					pszDest++;
				}
			}
			pszSource = pszNewSource;
		}
		*pszDest = 0;
		int nCount = int( pszSource-pszDest );
		this->ReleaseBufferSetLength( nLength-nCount );

		return( nCount );
	}

	CStringT Tokenize(
		_In_z_ PCXSTR pszTokens,
		_Inout_ int& iStart) const
	{
		ATLASSERT( iStart >= 0 );

		if(iStart < 0)
			AtlThrow("Invalid arguments");

		if( (pszTokens == NULL) || (*pszTokens == (XCHAR)0) )
		{
			if (iStart < this->GetLength())
			{
				return( CStringT( this->GetString()+iStart, GetManager() ) );
			}
		}
		else
		{
			PCXSTR pszPlace = this->GetString()+iStart;
			PCXSTR pszEnd = this->GetString() + this->GetLength();
			if( pszPlace < pszEnd )
			{
				int nIncluding = StringTraits::StringSpanIncluding( pszPlace,
					pszTokens );

				if( (pszPlace+nIncluding) < pszEnd )
				{
					pszPlace += nIncluding;
					int nExcluding = StringTraits::StringSpanExcluding( pszPlace, pszTokens );

					int iFrom = iStart+nIncluding;
					int nUntil = nExcluding;
					iStart = iFrom+nUntil+1;

					return( Mid( iFrom, nUntil ) );
				}
			}
		}

		// return empty string, done tokenizing
		iStart = -1;

		return( CStringT( GetManager() ) );
	}

	// find routines

	// Find the first occurrence of character 'ch', starting at index 'iStart'
	int Find(
		_In_ XCHAR ch,
		_In_ int iStart = 0) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );

		// nLength is in XCHARs
		int nLength = this->GetLength();
		if( iStart < 0 || iStart >= nLength)
		{
			return( -1 );
		}

		// find first single character
		PCXSTR psz = StringTraits::StringFindChar( this->GetString()+iStart, ch );

		// return -1 if not found and index otherwise
		return( (psz == NULL) ? -1 : int( psz - this->GetString() ) );
	}

	// look for a specific sub-string

	// Find the first occurrence of string 'pszSub', starting at index 'iStart'
	int Find(
		_In_z_ PCXSTR pszSub,
		_In_ int iStart = 0) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );
		ATLASSERT( AtlIsValidString( pszSub ) );

		if(pszSub == NULL)
		{
			return( -1 );
		}
		// nLength is in XCHARs
		int nLength = this->GetLength();
		if( iStart < 0 || iStart > nLength )
		{
			return( -1 );
		}

		// find first matching substring
		PCXSTR psz = StringTraits::StringFindString( this->GetString()+iStart, pszSub );

		// return -1 for not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz - this->GetString() ) );
	}

	// Find the first occurrence of any of the characters in string 'pszCharSet'
	int FindOneOf(_In_z_ PCXSTR pszCharSet) const throw()
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		PCXSTR psz = StringTraits::StringScanSet( this->GetString(), pszCharSet );
		return( (psz == NULL) ? -1 : int( psz - this->GetString() ) );
	}

	// Find the last occurrence of character 'ch'
	int ReverseFind(_In_ XCHAR ch) const throw()
	{
		// find last single character
		PCXSTR psz = StringTraits::StringFindCharRev( this->GetString(), ch );

		// return -1 if not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz - this->GetString() ) );
	}

	// manipulation

	// Convert the string to uppercase
	CStringT& MakeUpper()
	{
		int nLength = this->GetLength();
		PXSTR pszBuffer = this->GetBuffer( nLength );
		StringTraits::StringUppercase( pszBuffer, nLength+1 );
		this->ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// Convert the string to lowercase
	CStringT& MakeLower()
	{
		int nLength = this->GetLength();
		PXSTR pszBuffer = this->GetBuffer( nLength );
		StringTraits::StringLowercase( pszBuffer, nLength+1 );
		this->ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// Reverse the string
	CStringT& MakeReverse()
	{
		int nLength = this->GetLength();
		PXSTR pszBuffer = this->GetBuffer( nLength );
		StringTraits::StringReverse( pszBuffer );
		this->ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// trimming

	// Remove all trailing whitespace
	CStringT& TrimRight()
	{
		// find beginning of trailing spaces by starting
		// at beginning (DBCS aware)

		PCXSTR psz = this->GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::IsSpace( *psz ) )
			{
				if( pszLast == NULL )
					pszLast = psz;
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at trailing space start
			int iLast = int( pszLast - this->GetString() );

			this->Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading whitespace
	CStringT& TrimLeft()
	{
		// find first non-space character

		PCXSTR psz = this->GetString();

		while( StringTraits::IsSpace( *psz ) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != this->GetString() )
		{
			// fix up data and length
			int iFirst = int( psz - this->GetString() );
			PXSTR pszBuffer = this->GetBuffer( this->GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = this->GetLength()-iFirst;
			memmove_s( pszBuffer, (this->GetLength()+1)*sizeof( XCHAR ),
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			this->ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading and trailing whitespace
	CStringT& Trim()
	{
		return( TrimRight().TrimLeft() );
	}

	// Remove all leading and trailing occurrences of character 'chTarget'
	CStringT& Trim(_In_ XCHAR chTarget)
	{
		return( TrimRight( chTarget ).TrimLeft( chTarget ) );
	}

	// Remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
	CStringT& Trim(_In_z_ PCXSTR pszTargets)
	{
		return( TrimRight( pszTargets ).TrimLeft( pszTargets ) );
	}

	// trimming anything (either side)

	// Remove all trailing occurrences of character 'chTarget'
	CStringT& TrimRight(_In_ XCHAR chTarget)
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = this->GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( *psz == chTarget )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character
			int iLast = int( pszLast - this->GetString() );
			this->Truncate( iLast );
		}

		return( *this );
	}

	// Remove all trailing occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimRight(_In_z_ PCXSTR pszTargets)
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = this->GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::StringFindChar( pszTargets, *psz ) != NULL )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character
			int iLast = int( pszLast - this->GetString() );
			this->Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading occurrences of character 'chTarget'
	CStringT& TrimLeft(_In_ XCHAR chTarget)
	{
		// find first non-matching character
		PCXSTR psz = this->GetString();

		while( chTarget == *psz )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != this->GetString() )
		{
			// fix up data and length
			int iFirst = int( psz - this->GetString() );
			PXSTR pszBuffer = this->GetBuffer( this->GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = this->GetLength()-iFirst;
			memmove_s( pszBuffer, (this->GetLength()+1)*sizeof( XCHAR ),
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			this->ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimLeft(_In_z_ PCXSTR pszTargets)
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		PCXSTR psz = this->GetString();
		while( (*psz != 0) && (StringTraits::StringFindChar( pszTargets, *psz ) != NULL) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != this->GetString() )
		{
			// fix up data and length
			int iFirst = int( psz - this->GetString() );
			PXSTR pszBuffer = this->GetBuffer( this->GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = this->GetLength()-iFirst;
			memmove_s( pszBuffer, (this->GetLength()+1)*sizeof( XCHAR ),
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			this->ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Very simple sub-string extraction

	// Return the substring starting at index 'iFirst'
	CStringT Mid(_In_ int iFirst) const
	{
		return( Mid( iFirst, this->GetLength()-iFirst ) );
	}

	// Return the substring starting at index 'iFirst', with length 'nCount'
	CStringT Mid(
		_In_ int iFirst,
		_In_ int nCount) const
	{
		// nCount is in XCHARs

		// out-of-bounds requests return sensible things
		if (iFirst < 0)
			iFirst = 0;
		if (nCount < 0)
			nCount = 0;

		if( (::awesome_ATL::AtlAddThrow(iFirst,nCount)) > this->GetLength() )
		{
			nCount = this->GetLength()-iFirst;
		}
		if( iFirst > this->GetLength() )
		{
			nCount = 0;
		}

		ATLASSERT( (nCount == 0) || ((iFirst+nCount) <= this->GetLength()) );

		// optimize case of returning entire string
		if( (iFirst == 0) && ((iFirst+nCount) == this->GetLength()) )
		{
			return( *this );
		}

		return( CStringT( this->GetString()+iFirst, nCount, GetManager() ) );
	}

	// Return the substring consisting of the rightmost 'nCount' characters
	CStringT Right(_In_ int nCount) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = this->GetLength();
		if( nCount >= nLength )
		{
			return( *this );
		}

		return( CStringT( this->GetString()+nLength-nCount, nCount, GetManager() ) );
	}

	// Return the substring consisting of the leftmost 'nCount' characters
	CStringT Left(_In_ int nCount) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = this->GetLength();
		if( nCount >= nLength )
		{
			return( *this );
		}

		return( CStringT( this->GetString(), nCount, GetManager() ) );
	}

	// Return the substring consisting of the leftmost characters in the set 'pszCharSet'
	CStringT SpanIncluding(_In_z_ PCXSTR pszCharSet) const
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		if(pszCharSet == NULL)
			AtlThrow("Invalid arguments");

		return( Left( StringTraits::StringSpanIncluding( this->GetString(), pszCharSet ) ) );
	}

	// Return the substring consisting of the leftmost characters not in the set 'pszCharSet'
	CStringT SpanExcluding(_In_z_ PCXSTR pszCharSet) const
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		if(pszCharSet == NULL)
			AtlThrow("Invalid arguments");

		return( Left( StringTraits::StringSpanExcluding( this->GetString(), pszCharSet ) ) );
	}

	// Format data using format string 'pszFormat'
	void __cdecl Format(_In_z_ PCXSTR pszFormat, ...);

	// Format data using format string loaded from resource 'nFormatID'
	void __cdecl Format(_In_ UINT nFormatID, ...);

	// Append formatted data using format string loaded from resource 'nFormatID'
	void __cdecl AppendFormat(_In_ UINT nFormatID, ...);

	// Append formatted data using format string 'pszFormat'
	void __cdecl AppendFormat(_In_z_ PCXSTR pszFormat, ...);

	void AppendFormatV(
		_In_z_ PCXSTR pszFormat,
		_In_ va_list args)
	{
		ATLASSERT( AtlIsValidString( pszFormat ) );
		if(pszFormat == NULL)
			AtlThrow("Invalid arguments");

		int nCurrentLength = this->GetLength();
		int nAppendLength = StringTraits::GetFormattedLength( pszFormat, args );

		if (nAppendLength < 0)
			AtlThrow("FAIL");

		CStringT temp(pszFormat, this->GetManager());
		PXSTR pszBuffer = this->GetBuffer( nCurrentLength+nAppendLength );
		StringTraits::Format( pszBuffer+nCurrentLength,
			nAppendLength+1, temp, args );
		this->ReleaseBufferSetLength( nCurrentLength+nAppendLength );
	}

	void FormatV(
		_In_z_ PCXSTR pszFormat,
		_In_ va_list args)
	{
		ATLASSERT( AtlIsValidString( pszFormat ) );
		if(pszFormat == NULL)
			AtlThrow("Invalid arguments");

		int nLength = StringTraits::GetFormattedLength( pszFormat, args );

		if (nLength < 0)
			AtlThrow("FAIL");

		CStringT temp(pszFormat, this->GetManager());
		PXSTR pszBuffer = this->GetBuffer( nLength );
		StringTraits::Format( pszBuffer, nLength+1, temp, args );
		this->ReleaseBufferSetLength( nLength );
	}

	friend CStringT operator+(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2)
	{
		CStringT strResult( str1.GetManager() );

		CThisSimpleString::Concatenate( strResult, str1, str1.GetLength(), str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2)
	{
		CStringT strResult( str1.GetManager() );

		CThisSimpleString::Concatenate( strResult, str1, str1.GetLength(), psz2, CThisSimpleString::StringLength( psz2 ) );

		return( strResult );
	}

	friend CStringT operator+(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2)
	{
		CStringT strResult( str2.GetManager() );

		CThisSimpleString::Concatenate( strResult, psz1, CThisSimpleString::StringLength( psz1 ), str2, str2.GetLength() );

		return( strResult );
	}


#define _CSTRING_CHAR_T char


	friend CStringT operator+(
		_In_ const CStringT& str1,
		_In_ _CSTRING_CHAR_T ch2)
	{
		CStringT strResult( str1.GetManager() );
		XCHAR chTemp = XCHAR( ch2 );

		CThisSimpleString::Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+(
		_In_ _CSTRING_CHAR_T ch1,
		_In_ const CStringT& str2)
	{
		CStringT strResult( str2.GetManager() );
		XCHAR chTemp = XCHAR( ch1 );

		CThisSimpleString::Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+(
		_In_ const CStringT& str1,
		_In_ wchar_t ch2)
	{
		CStringT strResult( str1.GetManager() );
		XCHAR chTemp = XCHAR( ch2 );

		CThisSimpleString::Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+(
		_In_ wchar_t ch1,
		_In_ const CStringT& str2)
	{
		CStringT strResult( str2.GetManager() );
		XCHAR chTemp = XCHAR( ch1 );

		CThisSimpleString::Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend bool operator==(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2) throw()
	{
		return( str1.Compare( str2 ) == 0 );
	}

	friend bool operator==(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2) throw()
	{
		return( str1.Compare( psz2 ) == 0 );
	}

	friend bool operator==(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) == 0 );
	}

	friend bool operator!=(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) != 0 );
	}

	friend bool operator!=(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2) throw()
	{
		return( str1.Compare( psz2 ) != 0 );
	}

	friend bool operator!=(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) != 0 );
	}

	friend bool operator<(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2) throw()
	{
		return( str1.Compare( str2 ) < 0 );
	}

	friend bool operator<(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) < 0 );
	}

	friend bool operator<(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) > 0 );
	}

	friend bool operator>(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2) throw()
	{
		return( str1.Compare( str2 ) > 0 );
	}

	friend bool operator>(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2) throw()
	{
		return( str1.Compare( psz2 ) > 0 );
	}

	friend bool operator>(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) < 0 );
	}

	friend bool operator<=(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2) throw()
	{
		return( str1.Compare( str2 ) <= 0 );
	}

	friend bool operator<=(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2) throw()
	{
		return( str1.Compare( psz2 ) <= 0 );
	}

	friend bool operator<=(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) >= 0 );
	}

	friend bool operator>=(
		_In_ const CStringT& str1,
		_In_ const CStringT& str2) throw()
	{
		return( str1.Compare( str2 ) >= 0 );
	}

	friend bool operator>=(
		_In_ const CStringT& str1,
		_In_z_ PCXSTR psz2) throw()
	{
		return( str1.Compare( psz2 ) >= 0 );
	}

	friend bool operator>=(
		_In_z_ PCXSTR psz1,
		_In_ const CStringT& str2) throw()
	{
		return( str2.Compare( psz1 ) <= 0 );
	}

	friend bool operator==(
		_In_ XCHAR ch1,
		_In_ const CStringT& str2) throw()
	{
		return( (str2.GetLength() == 1) && (str2[0] == ch1) );
	}

	friend bool operator==(
		_In_ const CStringT& str1,
		_In_ XCHAR ch2) throw()
	{
		return( (str1.GetLength() == 1) && (str1[0] == ch2) );
	}

	friend bool operator!=(
		_In_ XCHAR ch1,
		_In_ const CStringT& str2) throw()
	{
		return( (str2.GetLength() != 1) || (str2[0] != ch1) );
	}

	friend bool operator!=(
		_In_ const CStringT& str1,
		_In_ XCHAR ch2) throw()
	{
		return( (str1.GetLength() != 1) || (str1[0] != ch2) );
	}

private:
	bool CheckImplicitLoad(_In_opt_ const void* pv)
	{
		bool bRet = false;

		if( (pv != NULL) && IS_INTRESOURCE( pv ) )
		{
			UINT nID = LOWORD( reinterpret_cast< DWORD_PTR >( pv ) );
			(nID);

			// ATLTRACE( atlTraceString, 2, _T( "Warning: LoadString(%u) not supported under the current WINAPI_FAMILY.\n" ), nID );
		}

		return( bRet );
	}
};

}