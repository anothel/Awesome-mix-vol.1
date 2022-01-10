#include <cassert>
#include <cstdlib>

class IAtlStringMgr {
 public:
  // Allocate a new CStringData
  virtual CStringData* Allocate(int nAllocLength, int nCharSize) throw() = 0;

  // Free an existing CStringData
  virtual void Free(CStringData* pData) throw() = 0;

  // Change the size of an existing CStringData
  virtual CStringData* Reallocate(CStringData* pData, int nAllocLength,
                                  int nCharSize) throw() = 0;

  // Get the CStringData for a Nil string
  virtual CStringData* GetNilString() throw() = 0;

  virtual IAtlStringMgr* Clone() throw() = 0;
};

struct CStringData {
  // String manager for this CStringData
  IAtlStringMgr* pStringMgr;
  // Length of currently used data in XCHARs (not including terminating null)
  int nDataLength;
  // Length of allocated data in XCHARs (not including terminating null)
  int nAllocLength;
  // Reference count: negative == locked
  long nRefs;

  void* data() throw() { return (this + 1); }

  void AddRef() throw() {
    assert(nRefs > 0);

#if defined(_WIN32)
    _InterlockedIncrement(&nRefs);
#elif defined(_LINUX)
    __sync_fetch_and_add(&nRefs, 1);
#endif
  }

  bool IsLocked() const throw() { return nRefs < 0; }

  bool IsShared() const throw() { return (nRefs > 1); }

  void Lock() throw() {
    assert(nRefs <= 1);

    // Locked buffers can't be shared, so no interlocked operation necessary
    nRefs--;
    if (nRefs == 0) {
      nRefs = -1;
    }
  }

  void Release() throw() {
    assert(nRefs != 0);

#if defined(_WIN32)
    if (_InterlockedDecrement(&nRefs) <= 0) {
      pStringMgr->Free(this);
    }
#elif defined(_LINUX)
    if (__sync_fetch_and_sub(&nRefs, 1) <= 0) {
      pStringMgr->Free(this);
    }
#endif
  }

  void Unlock() throw() {
    assert(IsLocked());

    if (IsLocked()) {
      // Locked buffers can't be shared, so no interlocked operation necessary
      nRefs++;
      if (nRefs == 0) {
        nRefs = 1;
      }
    }
  }
};


// template< typename BaseType , bool t_bMFCDLL = false>
class CSimpleStringT
{
public:
	explicit CSimpleStringT(IAtlStringMgr* pStringMgr)
	{
		assert( pStringMgr != NULL );
		CStringData* pData = pStringMgr->GetNilString();
		Attach( pData );
	}

	CSimpleStringT(const CSimpleStringT& strSrc)
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pNewData = CloneData( pSrcData );
		Attach( pNewData );
	}

	CSimpleStringT(const CSimpleStringT<BaseType, !t_bMFCDLL>& strSrc)
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pNewData = CloneData( pSrcData );
		Attach( pNewData );
	}

	CSimpleStringT(
		const char * pszSrc,
		IAtlStringMgr* pStringMgr)
	{
		assert( pStringMgr != NULL );

		int nLength = StringLength( pszSrc );
		CStringData* pData = pStringMgr->Allocate( nLength, sizeof( char ) );
		if( pData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, nLength, pszSrc, nLength );
	}
	CSimpleStringT(
		_In_reads_(nLength) const char* pchSrc,
		int nLength,
		IAtlStringMgr* pStringMgr)
	{
		assert( pStringMgr != NULL );

		if(pchSrc == NULL && nLength != 0)
			AtlThrow(E_INVALIDARG);

		CStringData* pData = pStringMgr->Allocate( nLength, sizeof( char ) );
		if( pData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pData );
		SetLength( nLength );
		CopyChars( m_pszData, nLength, pchSrc, nLength );
	}
	~CSimpleStringT() throw()
	{
		CStringData* pData = GetData();
		pData->Release();
	}

	operator CSimpleStringT<BaseType, !t_bMFCDLL>&()
	{
		return *(CSimpleStringT<BaseType, !t_bMFCDLL>*)this;
	}

	CSimpleStringT& operator=(const CSimpleStringT& strSrc)
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pOldData = GetData();
		if( pSrcData != pOldData)
		{
			if( pOldData->IsLocked() || pSrcData->pStringMgr != pOldData->pStringMgr )
			{
				SetString( strSrc.GetString(), strSrc.GetLength() );
			}
			else
			{
				CStringData* pNewData = CloneData( pSrcData );
				pOldData->Release();
				Attach( pNewData );
			}
		}

		return( *this );
	}

	CSimpleStringT& operator=(const CSimpleStringT<BaseType, !t_bMFCDLL>& strSrc)
	{
		CStringData* pSrcData = strSrc.GetData();
		CStringData* pOldData = GetData();
		if( pSrcData != pOldData)
		{
			if( pOldData->IsLocked() || pSrcData->pStringMgr != pOldData->pStringMgr )
			{
				SetString( strSrc.GetString(), strSrc.GetLength() );
			}
			else
			{
				CStringData* pNewData = CloneData( pSrcData );
				pOldData->Release();
				Attach( pNewData );
			}
		}

		return( *this );
	}

	CSimpleStringT& operator=(_In_opt_z_ const char * pszSrc)
	{
		SetString( pszSrc );

		return( *this );
	}

	CSimpleStringT& operator+=(const CSimpleStringT& strSrc)
	{
		Append( strSrc );

		return( *this );
	}
	template <bool bMFCDLL>
	CSimpleStringT& operator+=(const CSimpleStringT<BaseType, bMFCDLL>& strSrc)
	{
		Append( strSrc );

		return( *this );
	}

	CSimpleStringT& operator+=(const char * pszSrc)
	{
		Append( pszSrc );

		return( *this );
	}
	template< int t_nSize >
	CSimpleStringT& operator+=(const CStaticString< char, t_nSize >& strSrc)
	{
		Append( static_cast<const char *>(strSrc), strSrc.GetLength() );

		return( *this );
	}
	CSimpleStringT& operator+=(char ch)
	{
		AppendChar( char( ch ) );

		return( *this );
	}
	CSimpleStringT& operator+=(unsigned char ch)
	{
		AppendChar( char( ch ) );

		return( *this );
	}
	CSimpleStringT& operator+=(wchar_t ch)
	{
		AppendChar( char( ch ) );

		return( *this );
	}

	char operator[](int iChar) const
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK

		if( (iChar < 0) || (iChar > GetLength()) )
			AtlThrow(E_INVALIDARG);

		return( m_pszData[iChar] );
	}

	operator const char *() const throw()
	{
		return( m_pszData );
	}

	void Append(const char * pszSrc)
	{
		Append( pszSrc, StringLength( pszSrc ) );
	}
	void Append(
		_In_reads_(nLength) const char * pszSrc,
		int nLength)
	{
		// See comment in SetString() about why we do this
		UINT_PTR nOffset = pszSrc-GetString();

		UINT nOldLength = GetLength();
		if (nOldLength < 0)
		{
			// protects from underflow
			nOldLength = 0;
		}

		//Make sure the nLength is greater than zero
		ATLENSURE_THROW(nLength >=0, E_INVALIDARG);

		//Make sure we don't read pass end of the terminating NULL
		nLength = StringLengthN(pszSrc, nLength);

		//Make sure after the string doesn't exceed INT_MAX after appending
		ATLENSURE_THROW(INT_MAX - nLength >= static_cast<int>(nOldLength), E_INVALIDARG);

		int nNewLength = nOldLength+nLength;
		char * pszBuffer = GetBuffer( nNewLength );
		if( nOffset <= nOldLength )
		{
			pszSrc = pszBuffer+nOffset;
			// No need to call CopyCharsOverlapped, since the destination is
			// beyond the end of the original buffer
		}
		CopyChars( pszBuffer+nOldLength, nLength, pszSrc, nLength );
		ReleaseBufferSetLength( nNewLength );
	}
	void AppendChar(char ch)
	{
		UINT nOldLength = GetLength();
		int nNewLength = nOldLength+1;
		char * pszBuffer = GetBuffer( nNewLength );
		pszBuffer[nOldLength] = ch;
		ReleaseBufferSetLength( nNewLength );
	}
	void Append(const CSimpleStringT& strSrc)
	{
		Append( strSrc.GetString(), strSrc.GetLength() );
	}
	template <bool bMFCDLL>
	void Append(const CSimpleStringT<BaseType, bMFCDLL>& strSrc)
	{
		Append( strSrc.GetString(), strSrc.GetLength() );
	}
	void Empty() throw()
	{
		CStringData* pOldData = GetData();
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nDataLength == 0 )
		{
			return;
		}

		if( pOldData->IsLocked() )
		{
			// Don't reallocate a locked buffer that's shrinking
			SetLength( 0 );
		}
		else
		{
			pOldData->Release();
			CStringData* pNewData = pStringMgr->GetNilString();
			Attach( pNewData );
		}
	}
	void FreeExtra()
	{
		CStringData* pOldData = GetData();
		int nLength = pOldData->nDataLength;
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if( pOldData->nAllocLength == nLength )
		{
			return;
		}

		if( !pOldData->IsLocked() )  // Don't reallocate a locked buffer that's shrinking
		{
			CStringData* pNewData = pStringMgr->Allocate( nLength, sizeof( char ) );
			if( pNewData == NULL )
			{
				SetLength( nLength );
				return;
			}

			CopyChars( char *( pNewData->data() ), nLength,
				const char *( pOldData->data() ), nLength );

			pOldData->Release();
			Attach( pNewData );
			SetLength( nLength );
		}
	}

	int GetAllocLength() const throw()
	{
		return( GetData()->nAllocLength );
	}
	char GetAt(int iChar) const
	{
		ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );  // Indexing the '\0' is OK
		if( (iChar < 0) || (iChar > GetLength()) )
			AtlThrow(E_INVALIDARG);

		return( m_pszData[iChar] );
	}
	char * GetBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
		}

		return( m_pszData );
	}
	_Ret_notnull_ _Post_writable_size_(nMinBufferLength + 1) char * GetBuffer(int nMinBufferLength)
	{
		return( PrepareWrite( nMinBufferLength ) );
	}
	_Ret_notnull_ _Post_writable_size_(nLength + 1) char * GetBufferSetLength(int nLength)
	{
		char * pszBuffer = GetBuffer( nLength );
		SetLength( nLength );

		return( pszBuffer );
	}
	int GetLength() const throw()
	{
		return( GetData()->nDataLength );
	}
	IAtlStringMgr* GetManager() const throw()
	{
		IAtlStringMgr* pStringMgr = GetData()->pStringMgr;
		return pStringMgr ? pStringMgr->Clone() : NULL;
	}

	const char * GetString() const throw()
	{
		return( m_pszData );
	}
	bool IsEmpty() const throw()
	{
		return( GetLength() == 0 );
	}
	char * LockBuffer()
	{
		CStringData* pData = GetData();
		if( pData->IsShared() )
		{
			Fork( pData->nDataLength );
			pData = GetData();  // Do it again, because the fork might have changed it
		}
		pData->Lock();

		return( m_pszData );
	}
	void UnlockBuffer() throw()
	{
		CStringData* pData = GetData();
		pData->Unlock();
	}
	void Preallocate(int nLength)
	{
		PrepareWrite( nLength );
	}
	void ReleaseBuffer(int nNewLength = -1)
	{
		if( nNewLength == -1 )
		{
			int nAlloc = GetData()->nAllocLength;
			nNewLength = StringLengthN( m_pszData, nAlloc);
		}
		SetLength( nNewLength );
	}
	void ReleaseBufferSetLength(int nNewLength)
	{
		ATLASSERT( nNewLength >= 0 );
		SetLength( nNewLength );
	}
	void Truncate(int nNewLength)
	{
		ATLASSERT( nNewLength <= GetLength() );
		GetBuffer( nNewLength );
		ReleaseBufferSetLength( nNewLength );
	}
	void SetAt(
		int iChar,
		char ch)
	{
		ATLASSERT( (iChar >= 0) && (iChar < GetLength()) );

		if( (iChar < 0) || (iChar >= GetLength()) )
			AtlThrow(E_INVALIDARG);

		int nLength = GetLength();
		char * pszBuffer = GetBuffer();
		pszBuffer[iChar] = ch;
		ReleaseBufferSetLength( nLength );

	}
	void SetManager(IAtlStringMgr* pStringMgr)
	{
		ATLASSERT( IsEmpty() );

		CStringData* pData = GetData();
		pData->Release();
		pData = pStringMgr->GetNilString();
		Attach( pData );
	}
	void SetString(_In_opt_z_ const char * pszSrc)
	{
		SetString( pszSrc, StringLength( pszSrc ) );
	}
	void SetString(
		_In_reads_opt_(nLength) const char * pszSrc,
		int nLength)
	{
		if( nLength == 0 )
		{
			Empty();
		}
		else
		{
			// It is possible that pszSrc points to a location inside of our
			// buffer.  GetBuffer() might change m_pszData if (1) the buffer
			// is shared or (2) the buffer is too small to hold the new
			// string.  We detect this aliasing, and modify pszSrc to point
			// into the newly allocated buffer instead.

			if(pszSrc == NULL)
				AtlThrow(E_INVALIDARG);

			UINT nOldLength = GetLength();
			UINT_PTR nOffset = pszSrc-GetString();
			// If 0 <= nOffset <= nOldLength, then pszSrc points into our
			// buffer

			char * pszBuffer = GetBuffer( nLength );
			if( nOffset <= nOldLength )
			{
				CopyCharsOverlapped( pszBuffer, GetAllocLength(),
					pszBuffer+nOffset, nLength );
			}
			else
			{
				CopyChars( pszBuffer, GetAllocLength(), pszSrc, nLength );
			}
			ReleaseBufferSetLength( nLength );
		}
	}

public:
	friend CSimpleStringT operator+(
		const CSimpleStringT& str1,
		const CSimpleStringT& str2)
	{
		CSimpleStringT s( str1.GetManager() );

		Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

		return( s );
	}

	friend CSimpleStringT operator+(
		const CSimpleStringT& str1,
		const char * psz2)
	{
		CSimpleStringT s( str1.GetManager() );

		Concatenate( s, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

		return( s );
	}

	friend CSimpleStringT operator+(
		const char * psz1,
		const CSimpleStringT& str2)
	{
		CSimpleStringT s( str2.GetManager() );

		Concatenate( s, psz1, StringLength( psz1 ), str2, str2.GetLength() );

		return( s );
	}

	_ATL_INSECURE_DEPRECATE("CSimpleStringT::CopyChars must be passed a buffer size")
	static void __cdecl CopyChars(
		_Out_writes_(nChars) char* pchDest,
		_In_reads_opt_(nChars) const char* pchSrc,
		int nChars) throw()
	{
		if (pchSrc != NULL)
		{
			#pragma warning (push)
			#pragma warning(disable : 4996)
			memcpy( pchDest, pchSrc, nChars*sizeof( char ) );
			#pragma warning (pop)
		}
	}
	static void __cdecl CopyChars(
		_Out_writes_to_(nDestLen,nChars) char* pchDest,
		size_t nDestLen,
		_In_reads_opt_(nChars) const char* pchSrc,
		int nChars) throw()
	{
		memcpy_s( pchDest, nDestLen*sizeof( char ),
			pchSrc, nChars*sizeof( char ) );
	}

	_ATL_INSECURE_DEPRECATE("CSimpleStringT::CopyCharsOverlapped must be passed a buffer size")
	static void __cdecl CopyCharsOverlapped(
		_Out_writes_(nChars) char* pchDest,
		_In_reads_(nChars) const char* pchSrc,
		int nChars) throw()
	{
		#pragma warning (push)
		#pragma warning(disable : 4996)
		memmove( pchDest, pchSrc, nChars*sizeof( char ) );
		#pragma warning (pop)
	}
	static void __cdecl CopyCharsOverlapped(
		_Out_writes_to_(nDestLen, nDestLen) char* pchDest,
		size_t nDestLen,
		_In_reads_(nChars) const char* pchSrc,
		int nChars) throw()
	{
		memmove_s( pchDest, nDestLen*sizeof( char ),
			pchSrc, nChars*sizeof( char ) );
	}
	static int __cdecl StringLength(_In_opt_z_ const char* psz) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strlen( psz ) ) );
	}
	static int __cdecl StringLength(_In_opt_z_ const wchar_t* psz) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcslen( psz ) ) );
	}
	static int __cdecl StringLengthN(
		_In_reads_opt_z_(sizeInXChar) const char* psz,
		size_t sizeInXChar) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strnlen( psz, sizeInXChar ) ) );
	}
	static int __cdecl StringLengthN(
		_In_reads_opt_z_(sizeInXChar) const wchar_t* psz,
		size_t sizeInXChar) throw()
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcsnlen( psz, sizeInXChar ) ) );
	}

protected:
	static void __cdecl Concatenate(
		CSimpleStringT& strResult,
		_In_reads_(nLength1) const char * psz1,
		int nLength1,
		_In_reads_(nLength2) const char * psz2,
		int nLength2)
	{
		int nNewLength = nLength1+nLength2;
		char * pszBuffer = strResult.GetBuffer( nNewLength );
		CopyChars( pszBuffer, nLength1, psz1, nLength1 );
		CopyChars( pszBuffer+nLength1, nLength2, psz2, nLength2 );
		strResult.ReleaseBufferSetLength( nNewLength );
	}
	ATL_NOINLINE __declspec( noreturn ) static void __cdecl ThrowMemoryException()
	{
		AtlThrow( E_OUTOFMEMORY );
	}

// Implementation
private:
	void Attach(CStringData* pData) throw()
	{
		m_pszData = static_cast< char * >( pData->data() );
	}
	ATL_NOINLINE void Fork(int nLength)
	{
		CStringData* pOldData = GetData();
		int nOldLength = pOldData->nDataLength;
		CStringData* pNewData = pOldData->pStringMgr->Clone()->Allocate( nLength, sizeof( char ) );
		if( pNewData == NULL )
		{
			ThrowMemoryException();
		}
		int nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;  // Copy '\0'
		CopyChars( char *( pNewData->data() ), nCharsToCopy,
			const char *( pOldData->data() ), nCharsToCopy );
		pNewData->nDataLength = nOldLength;
		pOldData->Release();
		Attach( pNewData );
	}
	CStringData* GetData() const throw()
	{
		return( reinterpret_cast< CStringData* >( m_pszData )-1 );
	}
	char * PrepareWrite(int nLength)
	{
		if (nLength < 0)
			AtlThrow(E_INVALIDARG);

		CStringData* pOldData = GetData();
		int nShared = 1-pOldData->nRefs;  // nShared < 0 means true, >= 0 means false
		int nTooShort = pOldData->nAllocLength-nLength;  // nTooShort < 0 means true, >= 0 means false
		if( (nShared|nTooShort) < 0 )  // If either sign bit is set (i.e. either is less than zero), we need to copy data
		{
			PrepareWrite2( nLength );
		}

		return( m_pszData );
	}
	ATL_NOINLINE void PrepareWrite2(int nLength)
	{
		CStringData* pOldData = GetData();
		if( pOldData->nDataLength > nLength )
		{
			nLength = pOldData->nDataLength;
		}
		if( pOldData->IsShared() )
		{
			Fork( nLength );
		}
		else if( pOldData->nAllocLength < nLength )
		{
			// Grow exponentially, until we hit 1G, then by 1M thereafter.
			int nNewLength = pOldData->nAllocLength;
			if( nNewLength > 1024 * 1024 * 1024 )
			{
				nNewLength += 1024 * 1024;
			}
			else
			{
				// Exponential growth factor is 1.5.
				nNewLength = nNewLength + nNewLength / 2;
			}
			if( nNewLength < nLength )
			{
				nNewLength = nLength;
			}
			Reallocate( nNewLength );
		}
	}
	ATL_NOINLINE void Reallocate(int nLength)
	{
		CStringData* pOldData = GetData();
		ATLASSERT( pOldData->nAllocLength < nLength );
		IAtlStringMgr* pStringMgr = pOldData->pStringMgr;
		if ( pOldData->nAllocLength >= nLength || nLength <= 0)
		{
			ThrowMemoryException();
			return;
		}
		CStringData* pNewData = pStringMgr->Reallocate( pOldData, nLength, sizeof( char ) );
		if( pNewData == NULL )
		{
			ThrowMemoryException();
		}
		Attach( pNewData );
	}

	void SetLength(int nLength)
	{
		ATLASSERT( nLength >= 0 );
		ATLASSERT( nLength <= GetData()->nAllocLength );

		if( nLength < 0 || nLength > GetData()->nAllocLength)
			AtlThrow(E_INVALIDARG);

		GetData()->nDataLength = nLength;
		m_pszData[nLength] = 0;
	}

	static CStringData* __cdecl CloneData(CStringData* pData)
	{
		CStringData* pNewData = NULL;

		IAtlStringMgr* pNewStringMgr = pData->pStringMgr->Clone();
		if( !pData->IsLocked() && (pNewStringMgr == pData->pStringMgr) )
		{
			pNewData = pData;
			pNewData->AddRef();
		}
		else
		{
			pNewData = pNewStringMgr->Allocate( pData->nDataLength, sizeof( char ) );
			if( pNewData == NULL )
			{
				ThrowMemoryException();
			}
			pNewData->nDataLength = pData->nDataLength;
			CopyChars( char *( pNewData->data() ), pData->nDataLength+1,
				const char *( pData->data() ), pData->nDataLength+1 );  // Copy '\0'
		}

		return( pNewData );
	}

public :
	typedef CStrBufT<BaseType, t_bMFCDLL> CStrBuf;
private:
	char * m_pszData;

	friend class CSimpleStringT<BaseType, !t_bMFCDLL>;
};
