// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#ifndef AMVSIMPSTR_H_
#define AMVSIMPSTR_H_

#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>

#include "include/amvdefine.h"
#include "include/salieri.h"

namespace AMV {

struct BStringData;

__interface IAmvStringMgr {
 public:
  // Allocate a new BStringData
  virtual BStringData* Allocate(int nAllocLength, int nCharSize) throw() = 0;

  // Free an existing BStringData
  virtual void Free(BStringData * pData) throw() = 0;

  // Change the size of an existing BStringData
  virtual BStringData* Reallocate(BStringData * pData, int nAllocLength,
                                  int nCharSize) throw() = 0;

  // Get the BStringData for a Nil string
  virtual BStringData* GetNilString() throw() = 0;

  virtual IAmvStringMgr* Clone() throw() = 0;
};

struct BStringData {
  // String manager for this BStringData
  IAmvStringMgr* pStringMgr;
  // Length of currently used data in chars (not including terminating null)
  int nDataLength;
  // Length of allocated data in chars (not including terminating null)
  int nAllocLength;
  // Reference count: negative == locked
  int64_t nRefs;

  void* data() throw() { return (this + 1); }

  void AddRef() throw() {
    AMVASSERT(nRefs > 0);

    _InterlockedIncrement(&nRefs);
  }

  bool IsLocked() const throw() { return nRefs < 0; }

  bool IsShared() const throw() { return (nRefs > 1); }

  void Lock() throw() {
    AMVASSERT(nRefs <= 1);

    // Locked buffers can't be shared, so no interlocked operation necessary
    nRefs--;
    if (nRefs == 0) {
      nRefs = -1;
    }
  }

  void Release() throw() {
    AMVASSERT(nRefs != 0);

    if (_InterlockedDecrement(&nRefs) <= 0) {
      pStringMgr->Free(this);
    }
  }

  void Unlock() throw() {
    AMVASSERT(IsLocked());

    if (IsLocked()) {
      // Locked buffers can't be shared, so no interlocked operation necessary
      nRefs++;
      if (nRefs == 0) {
        nRefs = 1;
      }
    }
  }
};

class CNilStringData : public BStringData {
 public:
  CNilStringData() throw() {
    pStringMgr = NULL;
    // Never gets freed by IAmvStringMgr
    nRefs = 2;
    nDataLength = 0;
    nAllocLength = 0;
    achNil[0] = 0;
    achNil[1] = 0;
  }

  void SetManager(_In_ IAmvStringMgr* pMgr) throw() {
    AMVASSERT(pStringMgr == NULL);
    pStringMgr = pMgr;
  }

 public:
  wchar_t achNil[2];
};

template <typename TCharType>
class CStrBufT;
template <typename BaseType, const int t_nSize>
class CStaticString {
 public:
  explicit CStaticString(_In_z_ const BaseType* psz) : m_psz(psz) {}

  operator const BaseType*() const { return m_psz; }

  static int __cdecl GetLength() { return (t_nSize / sizeof(BaseType)) - 1; }

 private:
  const BaseType* m_psz;

 private:
  explicit CStaticString(_In_ const CStaticString& str) throw();
  CStaticString& operator=(_In_ const CStaticString& str) throw();
};

template <typename BaseType>
class CSimpleStringT {
 public:
  explicit CSimpleStringT(_Inout_ IAmvStringMgr* pStringMgr) {
    AMVENSURE(pStringMgr != NULL);
    BStringData* pData = pStringMgr->GetNilString();
    Attach(pData);
  }

  explicit CSimpleStringT(_In_ const CSimpleStringT& strSrc) {
    Attach(CloneData(strSrc.GetData()));
  }

  CSimpleStringT(_In_z_ const char* pszSrc, _Inout_ IAmvStringMgr* pStringMgr) {
    AMVENSURE(pStringMgr != NULL);

    int nLength = StringLength(pszSrc);
    BStringData* pData = pStringMgr->Allocate(nLength, sizeof(char));
    if (pData == NULL) {
      ThrowMemoryException();
    }
    Attach(pData);
    SetLength(nLength);
    CopyChars(m_pszData, nLength, pszSrc, nLength);
  }

  CSimpleStringT(_In_reads_(nLength) const char* pchSrc, _In_ int nLength,
                 _Inout_ IAmvStringMgr* pStringMgr) {
    AMVENSURE(pStringMgr != NULL);

    if (pchSrc == NULL && nLength != 0) AmvThrow("Invalid arguments");

    BStringData* pData = pStringMgr->Allocate(nLength, sizeof(char));
    if (pData == NULL) {
      ThrowMemoryException();
    }
    Attach(pData);
    SetLength(nLength);
    CopyChars(m_pszData, nLength, pchSrc, nLength);
  }

  ~CSimpleStringT() throw() {
    BStringData* pData = GetData();
    pData->Release();
  }

  operator CSimpleStringT<BaseType>&() {
    return *(CSimpleStringT<BaseType>*)this;
  }

  CSimpleStringT& operator=(_In_ const CSimpleStringT& strSrc) {
    BStringData* pSrcData = strSrc.GetData();
    BStringData* pOldData = GetData();
    if (pSrcData != pOldData) {
      if (pOldData->IsLocked() ||
          pSrcData->pStringMgr != pOldData->pStringMgr) {
        SetString(strSrc.GetString(), strSrc.GetLength());
      } else {
        BStringData* pNewData = CloneData(pSrcData);
        pOldData->Release();
        Attach(pNewData);
      }
    }

    return (*this);
  }

  CSimpleStringT& operator=(_In_opt_z_ const char* pszSrc) {
    SetString(pszSrc);

    return (*this);
  }

  CSimpleStringT& operator+=(_In_ const CSimpleStringT& strSrc) {
    Append(strSrc);

    return (*this);
  }

  CSimpleStringT& operator+=(_In_z_ const char* pszSrc) {
    Append(pszSrc);

    return (*this);
  }
  template <int t_nSize>
  CSimpleStringT& operator+=(_In_ const CStaticString<char, t_nSize>& strSrc) {
    Append(static_cast<const char*>(strSrc), strSrc.GetLength());

    return (*this);
  }
  CSimpleStringT& operator+=(_In_ char ch) {
    AppendChar(static_cast<char>(ch));

    return (*this);
  }
  CSimpleStringT& operator+=(_In_ unsigned char ch) {
    AppendChar(static_cast<char>(ch));

    return (*this);
  }

  char operator[](_In_ int iChar) const {
    // Indexing the '\0' is OK
    AMVASSERT((iChar >= 0) && (iChar <= GetLength()));

    if ((iChar < 0) || (iChar > GetLength())) AmvThrow("Invalid arguments");

    return (m_pszData[iChar]);
  }

  operator const char*() const throw() { return (m_pszData); }

  void Append(_In_z_ const char* pszSrc) {
    Append(pszSrc, StringLength(pszSrc));
  }

  void Append(_In_reads_(nLength) const char* pszSrc, _In_ int nLength) {
    // See comment in SetString() about why we do this
    UINT_PTR nOffset = pszSrc - GetString();

    UINT nOldLength = GetLength();
    if (nOldLength < 0) {
      // protects from underflow
      nOldLength = 0;
    }

    // Make sure the nLength is greater than zero
    AMVENSURE_THROW(nLength >= 0, "Invalid arguments");

    // Make sure we don't read pass end of the terminating NULL
    nLength = StringLengthN(pszSrc, nLength);

    // Make sure after the string doesn't exceed INT_MAX after appending
    AMVENSURE_THROW(INT_MAX - nLength >= static_cast<int>(nOldLength),
                    "Invalid arguments");

    int nNewLength = nOldLength + nLength;
    char* pszBuffer = GetBuffer(nNewLength);
    if (nOffset <= nOldLength) {
      pszSrc = pszBuffer + nOffset;
      /* No need to call CopyCharsOverlapped, since the destination is beyond
       * the end of the original buffer*/
    }
    CopyChars(pszBuffer + nOldLength, nLength, pszSrc, nLength);
    ReleaseBufferSetLength(nNewLength);
  }

  void AppendChar(_In_ char ch) {
    UINT nOldLength = GetLength();
    int nNewLength = nOldLength + 1;
    char* pszBuffer = GetBuffer(nNewLength);
    pszBuffer[nOldLength] = ch;
    ReleaseBufferSetLength(nNewLength);
  }

  void Append(_In_ const CSimpleStringT& strSrc) {
    Append(strSrc.GetString(), strSrc.GetLength());
  }

  _AMV_INSECURE_DEPRECATE(
      "CSimpleStringT::CopyChars must be passed a buffer size")
  static void __cdecl CopyChars(_Out_writes_(nChars) char* pchDest,
                                _In_reads_opt_(nChars) const char* pchSrc,
                                _In_ int nChars) throw() {
    if (pchSrc != NULL) {
      memcpy(pchDest, pchSrc, nChars * sizeof(char));
    }
  }

  static void __cdecl CopyChars(_Out_writes_to_(nDestLen, nChars) char* pchDest,
                                _In_ size_t nDestLen,
                                _In_reads_opt_(nChars) const char* pchSrc,
                                _In_ int nChars) throw() {
    memcpy_s(pchDest, nDestLen * sizeof(char), pchSrc, nChars * sizeof(char));
  }

  _AMV_INSECURE_DEPRECATE(
      "CSimpleStringT::CopyCharsOverlapped must be passed a buffer size")
  static void __cdecl CopyCharsOverlapped(_Out_writes_(nChars) char* pchDest,
                                          _In_reads_(nChars) const char* pchSrc,
                                          _In_ int nChars) throw() {
    memmove(pchDest, pchSrc, nChars * sizeof(char));
  }

  static void __cdecl CopyCharsOverlapped(
      _Out_writes_to_(nDestLen, nDestLen) char* pchDest, _In_ size_t nDestLen,
      _In_reads_(nChars) const char* pchSrc, _In_ int nChars) throw() {
    memmove_s(pchDest, nDestLen * sizeof(char), pchSrc, nChars * sizeof(char));
  }

  void Empty() throw() {
    BStringData* pOldData = GetData();
    if (pOldData->nDataLength == 0) {
      return;
    }

    if (pOldData->IsLocked()) {
      // Don't reallocate a locked buffer that's shrinking
      SetLength(0);
    } else {
      pOldData->Release();
      Attach(pOldData->pStringMgr->GetNilString());
    }
  }

  void FreeExtra() {
    BStringData* pOldData = GetData();
    int nLength = pOldData->nDataLength;
    IAmvStringMgr* pStringMgr = pOldData->pStringMgr;
    if (pOldData->nAllocLength == nLength) {
      return;
    }

    // Don't reallocate a locked buffer that's shrinking
    if (!pOldData->IsLocked()) {
      BStringData* pNewData = pStringMgr->Allocate(nLength, sizeof(char));
      if (pNewData == NULL) {
        SetLength(nLength);
        return;
      }

      CopyChars(static_cast<char*>(pNewData->data()), nLength,
                static_cast<const char*>(pOldData->data()), nLength);

      pOldData->Release();
      Attach(pNewData);
      SetLength(nLength);
    }
  }

  int GetAllocLength() const throw() { return (GetData()->nAllocLength); }

  char GetAt(_In_ int iChar) const {
    // Indexing the '\0' is OK
    AMVASSERT((iChar >= 0) && (iChar <= GetLength()));
    if ((iChar < 0) || (iChar > GetLength())) AmvThrow("Invalid arguments");

    return (m_pszData[iChar]);
  }

  char* GetBuffer() {
    BStringData* pData = GetData();
    if (pData->IsShared()) {
      Fork(pData->nDataLength);
    }

    return (m_pszData);
  }

  char* GetBuffer(_In_ int nMinBufferLength) {
    return (PrepareWrite(nMinBufferLength));
  }

  char* GetBufferSetLength(_In_ int nLength) {
    char* pszBuffer = GetBuffer(nLength);
    SetLength(nLength);

    return (pszBuffer);
  }

  int GetLength() const throw() { return (GetData()->nDataLength); }
  IAmvStringMgr* GetManager() const throw() {
    IAmvStringMgr* pStringMgr = GetData()->pStringMgr;
    return pStringMgr ? pStringMgr->Clone() : NULL;
  }

  const char* GetString() const throw() { return (m_pszData); }

  bool IsEmpty() const throw() { return (GetLength() == 0); }
  char* LockBuffer() {
    BStringData* pData = GetData();
    if (pData->IsShared()) {
      Fork(pData->nDataLength);
      // Do it again, because the fork might have changed it
      pData = GetData();
    }
    pData->Lock();

    return (m_pszData);
  }

  void UnlockBuffer() throw() {
    BStringData* pData = GetData();
    pData->Unlock();
  }

  void Preallocate(_In_ int nLength) { PrepareWrite(nLength); }

  void ReleaseBuffer(_In_ int nNewLength = -1) {
    if (nNewLength == -1) {
      int nAlloc = GetData()->nAllocLength;
      nNewLength = StringLengthN(m_pszData, nAlloc);
    }
    SetLength(nNewLength);
  }

  void ReleaseBufferSetLength(_In_ int nNewLength) {
    AMVASSERT(nNewLength >= 0);
    SetLength(nNewLength);
  }

  void Truncate(_In_ int nNewLength) {
    AMVASSERT(nNewLength <= GetLength());
    GetBuffer(nNewLength);
    ReleaseBufferSetLength(nNewLength);
  }

  void SetAt(_In_ int iChar, _In_ char ch) {
    AMVASSERT((iChar >= 0) && (iChar < GetLength()));

    if ((iChar < 0) || (iChar >= GetLength())) AmvThrow("Invalid arguments");

    int nLength = GetLength();
    char* pszBuffer = GetBuffer();
    pszBuffer[iChar] = ch;
    ReleaseBufferSetLength(nLength);
  }

  void SetManager(_Inout_ IAmvStringMgr* pStringMgr) {
    AMVASSERT(IsEmpty());

    BStringData* pData = GetData();
    pData->Release();
    pData = pStringMgr->GetNilString();
    Attach(pData);
  }

  void SetString(_In_opt_z_ const char* pszSrc) {
    SetString(pszSrc, StringLength(pszSrc));
  }

  void SetString(_In_reads_opt_(nLength) const char* pszSrc, _In_ int nLength) {
    if (nLength == 0) {
      Empty();
    } else {
      /* It is possible that pszSrc points to a location inside of our buffer.
       * GetBuffer() might change m_pszData if (1) the buffer is shared or (2)
       * the buffer is too small to hold the new string.  We detect this
       * aliasing, and modify pszSrc to point into the newly allocated buffer
       * instead.*/

      if (pszSrc == NULL) AmvThrow("Invalid arguments");

      UINT nOldLength = GetLength();
      UINT_PTR nOffset = pszSrc - GetString();
      // If 0 <= nOffset <= nOldLength, then pszSrc points into our
      // buffer

      char* pszBuffer = GetBuffer(nLength);
      if (nOffset <= nOldLength) {
        CopyCharsOverlapped(pszBuffer, GetAllocLength(), pszBuffer + nOffset,
                            nLength);
      } else {
        CopyChars(pszBuffer, GetAllocLength(), pszSrc, nLength);
      }
      ReleaseBufferSetLength(nLength);
    }
  }

 public:
  friend CSimpleStringT operator+(_In_ const CSimpleStringT& str1,
                                  _In_ const CSimpleStringT& str2) {
    CSimpleStringT s(str1.GetManager());

    Concatenate(s, str1, str1.GetLength(), str2, str2.GetLength());

    return (s);
  }

  friend CSimpleStringT operator+(_In_ const CSimpleStringT& str1,
                                  _In_z_ const char* psz2) {
    CSimpleStringT s(str1.GetManager());

    Concatenate(s, str1, str1.GetLength(), psz2, StringLength(psz2));

    return (s);
  }

  friend CSimpleStringT operator+(_In_z_ const char* psz1,
                                  _In_ const CSimpleStringT& str2) {
    CSimpleStringT s(str2.GetManager());

    Concatenate(s, psz1, StringLength(psz1), str2, str2.GetLength());

    return (s);
  }

  static int __cdecl StringLength(_In_opt_z_ const char* psz) throw() {
    if (psz == NULL) {
      return (0);
    }
    return (static_cast<int>(strlen(psz)));
  }

  static int __cdecl StringLengthN(_In_reads_opt_z_(sizeInXChar)
                                       const char* psz,
                                   _In_ size_t sizeInXChar) throw() {
    if (psz == NULL) {
      return (0);
    }
    return (static_cast<int>(strnlen(psz, sizeInXChar)));
  }

 protected:
  static void __cdecl Concatenate(_Inout_ const CSimpleStringT& strResult,
                                  _In_reads_(nLength1) const char* psz1,
                                  _In_ int nLength1,
                                  _In_reads_(nLength2) const char* psz2,
                                  _In_ int nLength2) {
    int nNewLength = nLength1 + nLength2;
    char* pszBuffer = strResult.GetBuffer(nNewLength);
    CopyChars(pszBuffer, nLength1, psz1, nLength1);
    CopyChars(pszBuffer + nLength1, nLength2, psz2, nLength2);
    strResult.ReleaseBufferSetLength(nNewLength);
  }

  AMV_NOINLINE __declspec(noreturn) static void __cdecl ThrowMemoryException() {
    AmvThrow("Out of memory");
  }

  // Implementation
 private:
  void Attach(_Inout_ BStringData* pData) throw() {
    m_pszData = static_cast<char*>(pData->data());
  }

  AMV_NOINLINE void Fork(_In_ int nLength) {
    BStringData* pOldData = GetData();
    int nOldLength = pOldData->nDataLength;
    BStringData* pNewData =
        pOldData->pStringMgr->Clone()->Allocate(nLength, sizeof(char));
    if (pNewData == NULL) {
      ThrowMemoryException();
    }
    // Copy '\0'
    int nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength) + 1;
    CopyChars(static_cast<char*>(pNewData->data()), nCharsToCopy,
              static_cast<const char*>(pOldData->data()), nCharsToCopy);
    pNewData->nDataLength = nOldLength;
    pOldData->Release();
    Attach(pNewData);
  }

  BStringData* GetData() const throw() {
    return (reinterpret_cast<BStringData*>(m_pszData) - 1);
  }

  char* PrepareWrite(_In_ int nLength) {
    if (nLength < 0) AmvThrow("Invalid arguments");

    BStringData* pOldData = GetData();
    // nShared < 0 means true, >= 0 means false
    int nShared = 1 - pOldData->nRefs;
    // nTooShort < 0 means true, >= 0 means false
    int nTooShort = pOldData->nAllocLength - nLength;
    /* If either sign bit is set (i.e. either is less than zero), we need to
     * copy data */
    if ((nShared | nTooShort) < 0) {
      PrepareWrite2(nLength);
    }

    return (m_pszData);
  }

  AMV_NOINLINE void PrepareWrite2(_In_ int nLength) {
    BStringData* pOldData = GetData();
    if (pOldData->nDataLength > nLength) {
      nLength = pOldData->nDataLength;
    }
    if (pOldData->IsShared()) {
      Fork(nLength);
    } else if (pOldData->nAllocLength < nLength) {
      // Grow exponentially, until we hit 1G, then by 1M thereafter.
      int nNewLength = pOldData->nAllocLength;
      if (nNewLength > 1024 * 1024 * 1024) {
        nNewLength += 1024 * 1024;
      } else {
        // Exponential growth factor is 1.5.
        nNewLength = nNewLength + nNewLength / 2;
      }
      if (nNewLength < nLength) {
        nNewLength = nLength;
      }
      Reallocate(nNewLength);
    }
  }

  AMV_NOINLINE void Reallocate(_In_ int nLength) {
    BStringData* pOldData = GetData();
    AMVASSERT(pOldData->nAllocLength < nLength);
    IAmvStringMgr* pStringMgr = pOldData->pStringMgr;
    if (pOldData->nAllocLength >= nLength || nLength <= 0) {
      ThrowMemoryException();
      return;
    }
    BStringData* pNewData =
        pStringMgr->Reallocate(pOldData, nLength, sizeof(char));
    if (pNewData == NULL) {
      ThrowMemoryException();
    }
    Attach(pNewData);
  }

  void SetLength(_In_ int nLength) {
    AMVASSERT(nLength >= 0);
    AMVASSERT(nLength <= GetData()->nAllocLength);

    if (nLength < 0 || nLength > GetData()->nAllocLength)
      AmvThrow("Invalid arguments");

    GetData()->nDataLength = nLength;
    m_pszData[nLength] = 0;
  }

  static BStringData* __cdecl CloneData(_Inout_ BStringData* pData) {
    BStringData* pNewData = NULL;

    IAmvStringMgr* pNewStringMgr = pData->pStringMgr->Clone();
    if (!pData->IsLocked() && (pNewStringMgr == pData->pStringMgr)) {
      pNewData = pData;
      pNewData->AddRef();
    } else {
      pNewData = pNewStringMgr->Allocate(pData->nDataLength, sizeof(char));
      if (pNewData == NULL) {
        ThrowMemoryException();
      }
      pNewData->nDataLength = pData->nDataLength;
      // Copy '\0'
      CopyChars(static_cast<char*>(pNewData->data()), pData->nDataLength + 1,
                static_cast<const char*>(pData->data()),
                pData->nDataLength + 1);
    }

    return (pNewData);
  }

 public:
  typedef CStrBufT<BaseType> CStrBuf;

 private:
  char* m_pszData;
};

template <typename TCharType>
class CStrBufT {
 public:
  typedef CSimpleStringT<TCharType> StringType;

  /* Automatically determine the new length of the string at release. The string
   * must be null-terminated. */
  static const DWORD AUTO_LENGTH = 0x01;
  // Set the length of the string object at GetBuffer time
  static const DWORD SET_LENGTH = 0x02;

 public:
  explicit CStrBufT(_In_ const StringType& str)
      : m_str(str), m_pszBuffer(NULL), m_nLength(str.GetLength()) {
    m_pszBuffer = m_str.GetBuffer();
  }

  CStrBufT(_In_ const StringType& str, _In_ int nMinLength,
           _In_ DWORD dwFlags = AUTO_LENGTH)
      : m_str(str),
        m_pszBuffer(NULL),
        m_nLength((dwFlags & AUTO_LENGTH) ? -1 : nMinLength) {
    if (dwFlags & SET_LENGTH) {
      m_pszBuffer = m_str.GetBufferSetLength(nMinLength);
    } else {
      m_pszBuffer = m_str.GetBuffer(nMinLength);
    }
  }

  ~CStrBufT() { m_str.ReleaseBuffer(m_nLength); }

  operator char*() throw() { return (m_pszBuffer); }
  operator const char*() const throw() { return (m_pszBuffer); }

  void SetLength(_In_ int nLength) {
    AMVASSERT(nLength >= 0);

    if (nLength < 0) AmvThrow("Invalid arguments");

    m_nLength = nLength;
  }

  // Implementation
 private:
  StringType& m_str;
  char* m_pszBuffer;
  int m_nLength;

  /* Private copy constructor and copy assignment operator to prevent
   * accidental use*/
 private:
  explicit CStrBufT(_In_ const CStrBufT&) throw();
  CStrBufT& operator=(_In_ const CStrBufT&) throw();
};

}  // namespace AMV

#endif  // AMVSIMPSTR_H_
