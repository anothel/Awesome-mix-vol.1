// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#ifndef AMVSTR_H_
#define AMVSTR_H_

#include <cctype>
#include <cstring>

#include "include/amvdefine.h"
#include "include/amvmem.h"
#include "include/amvsimpstr.h"
#include "include/bstringt.h"

namespace AMV {

class CAmvStringMgr : public IAmvStringMgr {
 public:
  explicit CAmvStringMgr(_In_opt_ IAmvMemMgr* pMemMgr = NULL) throw()
      : m_pMemMgr(pMemMgr) {
    m_nil.SetManager(this);
  }

  virtual ~CAmvStringMgr() throw() {}
  void SetMemoryManager(_In_ IAmvMemMgr* pMemMgr) throw() {
    AMVASSUME(m_pMemMgr == NULL);
    m_pMemMgr = pMemMgr;
  }

  static IAmvStringMgr* GetInstance() {
    static CAmvHeap strHeap;
    static CAmvStringMgr strMgr(&strHeap);

    return &strMgr;
  }

  // IAmvStringMgr
 public:
  virtual BStringData* Allocate(_In_ int nChars, _In_ int nCharSize) throw() {
    AMVENSURE_RETURN_VAL(nChars >= 0, NULL);

    size_t nTotalSize;
    BStringData* pData;
    size_t nDataBytes;

    if (FAILED(::AMV::AmvAdd(&nChars, nChars, 1))) {
      return NULL;
    }

    // Prevent excessive reallocation. The heap will usually round up anyway.
    int nAlignedChars = ::AMV::AmvAlignUp(nChars, 8);
    AMVENSURE_RETURN_VAL(nChars <= nAlignedChars, NULL);

    if (FAILED(::AMV::AmvMultiply(&nDataBytes,
                                  static_cast<size_t>(nAlignedChars),
                                  static_cast<size_t>(nCharSize))) ||
        FAILED(::AMV::AmvAdd(&nTotalSize,
                             static_cast<size_t>(sizeof(BStringData)),
                             nDataBytes))) {
      return NULL;
    }
    pData = static_cast<BStringData*>(m_pMemMgr->Allocate(nTotalSize));
    if (pData == NULL) {
      return (NULL);
    }
    pData->pStringMgr = this;
    pData->nRefs = 1;
    pData->nAllocLength = nAlignedChars - 1;
    pData->nDataLength = 0;

    return (pData);
  }

  virtual void Free(_In_ BStringData* pData) throw() {
    AMVASSUME(pData != NULL);
    AMVASSERT(pData->pStringMgr == this);

    m_pMemMgr->Free(pData);
  }

  virtual BStringData* Reallocate(_Inout_ BStringData* pData, _In_ int nChars,
                                  _In_ int nCharSize) throw() {
    AMVENSURE_RETURN_VAL(nChars >= 0, NULL);
    AMVASSERT(pData->pStringMgr == this);

    BStringData* pNewData;
    ULONG nTotalSize;
    ULONG nDataBytes;

    if (FAILED(::AMV::AmvAdd(&nChars, nChars, 1))) {
      return NULL;
    }

    // Prevent excessive reallocation. The heap will usually round up anyway.
    int nAlignedChars = ::AMV::AmvAlignUp(nChars, 8);
    AMVENSURE_RETURN_VAL(nChars <= nAlignedChars, NULL);

    if (FAILED(::AMV::AmvMultiply(&nDataBytes,
                                  static_cast<ULONG>(nAlignedChars),
                                  static_cast<ULONG>(nCharSize))) ||
        FAILED(::AMV::AmvAdd(&nTotalSize,
                             static_cast<ULONG>(sizeof(BStringData)),
                             nDataBytes))) {
      return NULL;
    }
    pNewData =
        static_cast<BStringData*>(m_pMemMgr->Reallocate(pData, nTotalSize));
    if (pNewData == NULL) {
      return NULL;
    }
    pNewData->nAllocLength = nAlignedChars - 1;

    return pNewData;
  }

  virtual BStringData* GetNilString() throw() {
    m_nil.AddRef();
    return &m_nil;
  }

  virtual IAmvStringMgr* Clone() throw() { return this; }

 protected:
  IAmvMemMgr* m_pMemMgr;
  CNilStringData m_nil;

 private:
};

template <class ChTraits>
inline const char* strstrT(const char* pStr, const char* pCharSet);

template <typename _CharType = char>
class ChTraitsOS {
 public:
  static int tclen(_In_z_ const char* p) throw() {
    AMVASSERT(p != NULL);
    const char* pnext = CharNext(p);
    return ((pnext - p) > 1) ? 2 : 1;
  }

  _Ret_maybenull_z_ static const char* strchr(_In_z_ const char* p,
                                              _In_ char ch) throw() {
    return AmvstrchrT(p, ch);
  }

  _Ret_maybenull_z_ static const char* strchr_db(_In_z_ const char* p,
                                                 _In_ char ch1,
                                                 _In_ char ch2) throw() {
    AMVASSERT(p != NULL);
    while (*p != 0) {
      if (*p == ch1 && *(p + 1) == ch2) {
        return p;
      }
      p = CharNext(p);
    }
    return NULL;
  }

  static int strspn(_In_z_ const _CharType* pStr,
                    _In_z_ const _CharType* pCharSet) throw() {
    AMVASSERT(pStr != NULL);
    int nRet = 0;
    _CharType* p = const_cast<_CharType*>(pStr);
    while (*p != 0) {
      _CharType* pNext = CharNext(p);
      if (pNext > p + 1) {
        if (strchr_db(pCharSet, *p, *(p + 1)) == NULL) break;
        nRet += 2;
      } else {
        if (strchr(pCharSet, *p) == NULL) break;
        nRet++;
      }
      p = pNext;
    }
    return nRet;
  }

  static int strcspn(_In_z_ const _CharType* pStr,
                     _In_z_ const _CharType* pCharSet) throw() {
    AMVASSERT(pStr != NULL);
    int nRet = 0;
    _CharType* p = const_cast<_CharType*>(pStr);
    while (*p != 0) {
      _CharType* pNext = CharNext(p);
      if (pNext > p + 1) {
        if (strchr_db(pCharSet, *p, *(p + 1)) != NULL) break;
        nRet += 2;
      } else {
        if (strchr(pCharSet, *p) != NULL) break;
        nRet++;
      }
      p = pNext;
    }
    return nRet;
  }

  static const char* strpbrk(_In_z_ const char* p,
                             _In_z_ const char* lpszCharSet) throw() {
    int nRet = 0;
    nRet = strcspn(p, lpszCharSet);
    if (p[nRet]) {
      p += nRet;
      return p;
    }
    return NULL;
  }

  static _CharType* CharNext(_In_ const _CharType* p) throw() {
    return AmvCharNext(p);
  }

  static int IsDigit(_In_ _CharType ch) throw() { return isdigit(ch); }

  static int IsSpace(_In_ _CharType ch) throw() { return isspace(ch); }

  static int StringCompare(_In_z_ const _CharType* pstrOne,
                           _In_z_ const _CharType* pstrOther) throw() {
    return strcmp((const char*)pstrOne, (const char*)pstrOther);
  }

  static int StringCompareIgnore(_In_z_ const _CharType* pstrOne,
                                 _In_z_ const _CharType* pstrOther) throw() {
    return strcasecmp((const char*)pstrOne, (const char*)pstrOther);
  }

  static const char* StringFindString(_In_z_ const char* pstrBlock,
                                      _In_z_ const char* pstrMatch) throw() {
    return strstr(pstrBlock, pstrMatch);
  }

  static LPSTR StringFindString(_In_z_ LPSTR pszBlock,
                                _In_z_ const char* pszMatch) throw() {
    return (const_cast<LPSTR>(
        StringFindString(const_cast<const char*>(pszBlock), pszMatch)));
  }

  static const char* StringFindChar(_In_z_ const char* pszBlock,
                                    _In_ char chMatch) throw() {
    return strchr(pszBlock, chMatch);
  }

  static const char* StringFindCharRev(_In_z_ const char* psz,
                                       _In_ char ch) throw() {
    return strrchr(psz, ch);
  }

  static const char* StringScanSet(_In_z_ const char* pszBlock,
                                   _In_z_ const char* pszMatch) throw() {
    return strpbrk(pszBlock, pszMatch);
  }

  static int StringSpanIncluding(_In_z_ const _CharType* pstrBlock,
                                 _In_z_ const _CharType* pstrSet) throw() {
    return strspn(pstrBlock, pstrSet);
  }

  static int StringSpanExcluding(_In_z_ const _CharType* pstrBlock,
                                 _In_z_ const _CharType* pstrSet) throw() {
    return strcspn(pstrBlock, pstrSet);
  }


  static int GetBaseTypeLength(_In_z_ const char* pszSrc) throw() {
    // Returns required buffer length in XCHARs
    if (pszSrc == NULL) return 0;
    return static_cast<int>(::strlen(pszSrc));
  }

  static int GetBaseTypeLength(_In_z_ const char* pszSrc,
                               _In_ int nLength) throw() {
    (void)pszSrc;
    // Returns required buffer length in XCHARs
    return nLength;
  }

  static void ConvertToBaseType(_Out_writes_(nDestLength) _CharType* pszDest,
                                _In_ int nDestLength,
                                _In_reads_(nSrcLength) const char* pszSrc,
                                _In_ int nSrcLength = -1) throw() {
    if (nSrcLength == -1) {
      nSrcLength = 1 + GetBaseTypeLength(pszSrc);
    }
    // nLen is in chars
    memcpy(pszDest, pszSrc, nSrcLength * sizeof(_CharType));
  }

  static int SafeStringLen(_In_opt_z_ const char* psz) throw() {
    // returns length in bytes
    return (psz != NULL) ? static_cast<int>(strlen(psz)) : 0;
  }
};

template <class ChTraits>
inline const char* strstrT(_In_ const char* pStr, _In_ const char* pCharSet) {
  log1;
  AMVASSERT(pStr != NULL);
  size_t nCharSetLen = ChTraits::GetBaseTypeLength(pCharSet);
  if (nCharSetLen == 0) return pStr;

  // strlen returns length in bytes, not chars.
  size_t nStrLen = ChTraits::GetBaseTypeLength(pStr);
  const char* pStrEnd = pStr + nStrLen;
  const char* pMatch;
  const char* pStart = pStr;
  while ((pMatch = ChTraits::strchr(pStart, *pCharSet)) != NULL) {
    size_t nCharsLeftInStr = pStrEnd - pMatch;
    if (nCharsLeftInStr < nCharSetLen) {
      break;
    }
    if (memcmp(pMatch, pCharSet, nCharSetLen * sizeof(char)) == 0) {
      return pMatch;
    }
    pStart = ChTraits::CharNext(pMatch);
  }

  return NULL;
}

template <typename _BaseType = char,
          class StringIterator = ChTraitsOS<_BaseType> >
class StrTraitAMV : public StringIterator {
 public:
  static IAmvStringMgr* GetDefaultManager() throw() {
    return CAmvStringMgr::GetInstance();
  }
};

typedef BStringT<char, StrTraitAMV<char> > CAmvString;

}  // namespace AMV
typedef AMV::CAmvString BString;

#endif  // AMVSTR_H_
