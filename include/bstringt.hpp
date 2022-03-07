// Copyright 2021~2022 `anothel` All rights reserved

#ifndef BSTRINGT_HPP_
#define BSTRINGT_HPP_

#include "include/amvalloc.hpp"
#include "include/amvcore.hpp"
#include "salieri-src/salieri.h"

namespace AMV {

template <typename BaseType, class StringTraits>
class BStringT : public CSimpleStringT<BaseType> {
 public:
  typedef CSimpleStringT<BaseType> CThisSimpleString;
  typedef StringTraits StrTraits;

 public:
  BStringT() throw() : CThisSimpleString(StringTraits::GetDefaultManager()) {}

  explicit BStringT(_In_ IAmvStringMgr* pStringMgr) throw()
      : CThisSimpleString(pStringMgr) {}

  // Copy constructor
  // explicit BStringT(_In_ const BStringT& strSrc) : CThisSimpleString(strSrc)
  // {}
  /* Todo(jpk, 20220118): 파라미터가 한개이기 때문에 explicit를 걸어서 암시적
   * 형변환을 막아야 한다면 이 상황에서는 처치할 수가 없음 */
  BStringT(_In_ const BStringT& strSrc) : CThisSimpleString(strSrc) {}

  // // Construct from CSimpleStringT
  // operator CSimpleStringT<BaseType>&() {
  //
  //   return *(CSimpleStringT<BaseType>*)this;
  // }

  explicit BStringT(_In_ const CSimpleStringT<BaseType>& strSrc)
      : CThisSimpleString(strSrc) {}

  explicit BStringT(_In_opt_z_ const char* pszSrc)
      : CThisSimpleString(StringTraits::GetDefaultManager()) {
    *this = pszSrc;
  }

  BStringT(_In_opt_z_ const char* pszSrc, _In_ IAmvStringMgr* pStringMgr)
      : CThisSimpleString(pStringMgr) {
    *this = pszSrc;
  }

  BStringT(_In_opt_z_ const unsigned char* pszSrc,
           _In_ IAmvStringMgr* pStringMgr)
      : CThisSimpleString(pStringMgr) {
    *this = reinterpret_cast<const char*>(pszSrc);
  }
  BStringT(_In_reads_(nLength) const unsigned char* puch, _In_ int nLength)
      : CThisSimpleString(puch, nLength, StringTraits::GetDefaultManager()) {}

  // Destructor
  ~BStringT() throw() {}

  // Assignment operators
  BStringT& operator=(_In_ const BStringT& strSrc) {
    CThisSimpleString::operator=(strSrc);

    return (*this);
  }

  BStringT& operator=(_In_ const CSimpleStringT<BaseType>& strSrc) {
    CThisSimpleString::operator=(strSrc);

    return (*this);
  }

  BStringT& operator=(_In_opt_z_ const char* pszSrc) {
    CThisSimpleString::operator=(pszSrc);

    return (*this);
  }

  BStringT& operator=(_In_ char ch) {
    char ach[2] = {ch, 0};

    return (operator=(ach));
  }

  BStringT& operator+=(_In_ const CThisSimpleString& str) {
    CThisSimpleString::operator+=(str);

    return (*this);
  }

  BStringT& operator+=(_In_z_ const char* pszSrc) {
    CThisSimpleString::operator+=(pszSrc);

    return (*this);
  }
  template <int t_nSize>
  BStringT& operator+=(_In_ const CStaticString<char, t_nSize>& strSrc) {
    CThisSimpleString::operator+=(strSrc);

    return (*this);
  }

  BStringT& operator+=(_In_ char ch) {
    CThisSimpleString::operator+=(ch);

    return (*this);
  }

  BStringT& operator+=(_In_ unsigned char ch) {
    CThisSimpleString::operator+=(ch);

    return (*this);
  }

  // Override from base class
  IAmvStringMgr* GetManager() const throw() {
    IAmvStringMgr* pStringMgr = CThisSimpleString::GetManager();
    if (pStringMgr) {
      return pStringMgr;
    }

    return StringTraits::GetDefaultManager()->Clone();
  }

  // Comparison

  int Compare(_In_z_ const char* psz) const {
    AMVENSURE(AmvIsValidString(psz));
    // AmvIsValidString guarantees that psz != NULL
    return (StringTraits::StringCompare(this->GetString(), psz));
  }

  int CompareNoCase(_In_z_ const char* psz) const {
    AMVENSURE(AmvIsValidString(psz));
    // AmvIsValidString guarantees that psz != NULL
    return (StringTraits::StringCompareIgnore(this->GetString(), psz));
  }

  // Advanced manipulation

  // Delete 'nCount' characters, starting at index 'iIndex'
  int Delete(_In_ int iIndex, _In_ int nCount = 1) {
    if (iIndex < 0) iIndex = 0;

    if (nCount < 0) nCount = 0;

    int nLength = this->GetLength();
    if ((::AMV::AmvAddThrow(nCount, iIndex)) > nLength) {
      nCount = nLength - iIndex;
    }
    if (nCount > 0) {
      int nNewLength = nLength - nCount;
      int nXCHARsToCopy = nLength - (iIndex + nCount) + 1;
      char* pszBuffer = this->GetBuffer();
      memmove_s(pszBuffer + iIndex, nXCHARsToCopy * sizeof(char),
                pszBuffer + iIndex + nCount, nXCHARsToCopy * sizeof(char));
      this->ReleaseBufferSetLength(nNewLength);
    }

    return (this->GetLength());
  }

  // Insert character 'ch' before index 'iIndex'
  int Insert(_In_ int iIndex, _In_ char ch) {
    if (iIndex < 0) iIndex = 0;

    if (iIndex > this->GetLength()) {
      iIndex = this->GetLength();
    }
    int nNewLength = this->GetLength() + 1;

    char* pszBuffer = this->GetBuffer(nNewLength);

    // move existing bytes down
    memmove_s(pszBuffer + iIndex + 1, (nNewLength - iIndex) * sizeof(char),
              pszBuffer + iIndex, (nNewLength - iIndex) * sizeof(char));
    pszBuffer[iIndex] = ch;

    this->ReleaseBufferSetLength(nNewLength);

    return (nNewLength);
  }

  // Insert string 'psz' before index 'iIndex'
  int Insert(_In_ int iIndex, _In_z_ const char* psz) {
    if (iIndex < 0) iIndex = 0;

    if (iIndex > this->GetLength()) {
      iIndex = this->GetLength();
    }

    // nInsertLength and nNewLength are in XCHARs
    int nInsertLength = StringTraits::SafeStringLen(psz);
    int nNewLength = this->GetLength();
    if (nInsertLength > 0) {
      nNewLength += nInsertLength;

      char* pszBuffer = this->GetBuffer(nNewLength);
      // move existing bytes down
      memmove_s(pszBuffer + iIndex + nInsertLength,
                (nNewLength - iIndex - nInsertLength + 1) * sizeof(char),
                pszBuffer + iIndex,
                (nNewLength - iIndex - nInsertLength + 1) * sizeof(char));
      memcpy_s(pszBuffer + iIndex, nInsertLength * sizeof(char), psz,
               nInsertLength * sizeof(char));
      this->ReleaseBufferSetLength(nNewLength);
    }

    return (nNewLength);
  }

  // Replace all occurrences of character 'chOld' with character 'chNew'
  int Replace(_In_ char chOld, _In_ char chNew) {
    int nCount = 0;

    // short-circuit the nop case
    if (chOld != chNew) {
      // otherwise modify each character that matches in the string
      bool bCopied = false;
      // We don't actually write to pszBuffer until we've called GetBuffer().
      char* pszBuffer = const_cast<char*>(this->GetString());

      int nLength = this->GetLength();
      int iChar = 0;
      while (iChar < nLength) {
        // replace instances of the specified character only
        if (pszBuffer[iChar] == chOld) {
          if (!bCopied) {
            bCopied = true;
            pszBuffer = this->GetBuffer(nLength);
          }
          pszBuffer[iChar] = chNew;
          nCount++;
        }
        iChar = static_cast<int>(StringTraits::CharNext(pszBuffer + iChar) -
                                 pszBuffer);
      }
      if (bCopied) {
        this->ReleaseBufferSetLength(nLength);
      }
    }

    return (nCount);
  }

  // Replace all occurrences of string 'pszOld' with string 'pszNew'
  int Replace(_In_z_ const char* pszOld, _In_z_ const char* pszNew) {
    // can't have empty or NULL lpszOld

    // nSourceLen is in XCHARs
    int nSourceLen = StringTraits::SafeStringLen(pszOld);
    if (nSourceLen == 0) return (0);
    // nReplacementLen is in XCHARs
    int nReplacementLen = StringTraits::SafeStringLen(pszNew);

    // loop once to figure out the size of the result string
    int nCount = 0;
    {
      const char* pszStart = this->GetString();
      const char* pszEnd = pszStart + this->GetLength();
      while (pszStart < pszEnd) {
        const char* pszTarget;
        while ((pszTarget = StringTraits::StringFindString(pszStart, pszOld)) !=
               NULL) {
          nCount++;
          pszStart = pszTarget + nSourceLen;
        }
        pszStart += StringTraits::SafeStringLen(pszStart) + 1;
      }
    }

    // if any changes were made, make them
    if (nCount > 0) {
      // if the buffer is too small, just allocate a new buffer (slow but sure)
      int nOldLength = this->GetLength();
      int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;

      char* pszBuffer = this->GetBuffer(__max(nNewLength, nOldLength));

      char* pszStart = pszBuffer;
      char* pszEnd = pszStart + nOldLength;

      // loop again to actually do the work
      while (pszStart < pszEnd) {
        char* pszTarget;
        while ((pszTarget = StringTraits::StringFindString(pszStart, pszOld)) !=
               NULL) {
          int nBalance =
              nOldLength - static_cast<int>(pszTarget - pszBuffer + nSourceLen);
          memmove_s(pszTarget + nReplacementLen, nBalance * sizeof(char),
                    pszTarget + nSourceLen, nBalance * sizeof(char));
          memcpy_s(pszTarget, nReplacementLen * sizeof(char), pszNew,
                   nReplacementLen * sizeof(char));
          pszStart = pszTarget + nReplacementLen;
          pszTarget[nReplacementLen + nBalance] = 0;
          nOldLength += (nReplacementLen - nSourceLen);
        }
        pszStart += StringTraits::SafeStringLen(pszStart) + 1;
      }
      AMVASSERT(pszBuffer[nNewLength] == 0);
      this->ReleaseBufferSetLength(nNewLength);
    }

    return (nCount);
  }

  // Remove all occurrences of character 'chRemove'
  int Remove(_In_ char chRemove) {
    int nLength = this->GetLength();
    char* pszBuffer = this->GetBuffer(nLength);

    char* pszSource = pszBuffer;
    char* pszDest = pszBuffer;
    char* pszEnd = pszBuffer + nLength;

    while (pszSource < pszEnd) {
      char* pszNewSource = StringTraits::CharNext(pszSource);
      if (*pszSource != chRemove) {
        /* Copy the source to the destination.  Remember to copy all bytes of an
         * MBCS character */
        size_t NewSourceGap = (pszNewSource - pszSource);
        char* pszNewDest = pszDest + NewSourceGap;
        size_t i = 0;
        for (i = 0; pszDest != pszNewDest && i < NewSourceGap; i++) {
          *pszDest = *pszSource;
          pszSource++;
          pszDest++;
        }
      }
      pszSource = pszNewSource;
    }
    *pszDest = 0;
    int nCount = static_cast<int>(pszSource - pszDest);
    this->ReleaseBufferSetLength(nLength - nCount);

    return (nCount);
  }

  // find routines

  // Find the first occurrence of character 'ch', starting at index 'iStart'
  int Find(_In_ char ch, _In_ int iStart = 0) const throw() {
    // iStart is in XCHARs
    AMVASSERT(iStart >= 0);

    // nLength is in XCHARs
    int nLength = this->GetLength();
    if (iStart < 0 || iStart >= nLength) {
      return (-1);
    }

    // find first single character
    const char* psz =
        StringTraits::StringFindChar(this->GetString() + iStart, ch);

    // return -1 if not found and index otherwise
    return ((psz == NULL) ? -1 : static_cast<int>(psz - this->GetString()));
  }

  // look for a specific sub-string

  // Find the first occurrence of string 'pszSub', starting at index 'iStart'
  int Find(_In_z_ const char* pszSub, _In_ int iStart = 0) const throw() {
    // iStart is in XCHARs
    AMVASSERT(iStart >= 0);
    AMVASSERT(AmvIsValidString(pszSub));

    if (pszSub == NULL) {
      return (-1);
    }
    // nLength is in XCHARs
    int nLength = this->GetLength();
    if (iStart < 0 || iStart > nLength) {
      return (-1);
    }

    // find first matching substring
    const char* psz =
        StringTraits::StringFindString(this->GetString() + iStart, pszSub);

    // return -1 for not found, distance from beginning otherwise
    return ((psz == NULL) ? -1 : static_cast<int>(psz - this->GetString()));
  }

  // Find the first occurrence of any of the characters in string 'pszCharSet'
  int FindOneOf(_In_z_ const char* pszCharSet) const throw() {
    AMVASSERT(AmvIsValidString(pszCharSet));
    const char* psz =
        StringTraits::StringScanSet(this->GetString(), pszCharSet);
    return ((psz == NULL) ? -1 : static_cast<int>(psz - this->GetString()));
  }

  // Find the last occurrence of character 'ch'
  int ReverseFind(_In_ char ch) const throw() {
    // find last single character
    const char* psz = StringTraits::StringFindCharRev(this->GetString(), ch);

    // return -1 if not found, distance from beginning otherwise
    return ((psz == NULL) ? -1 : static_cast<int>(psz - this->GetString()));
  }

  // trimming

  // Remove all trailing whitespace
  BStringT& TrimRight() {
    // find beginning of trailing spaces by starting at beginning (DBCS aware)
    const char* psz = this->GetString();
    const char* pszLast = NULL;

    while (*psz != 0) {
      if (StringTraits::IsSpace(*psz)) {
        if (pszLast == NULL) pszLast = psz;
      } else {
        pszLast = NULL;
      }
      psz = StringTraits::CharNext(psz);
    }

    if (pszLast != NULL) {
      // truncate at trailing space start
      int iLast = static_cast<int>(pszLast - this->GetString());

      this->Truncate(iLast);
    }

    return (*this);
  }

  // Remove all leading whitespace
  BStringT& TrimLeft() {
    // find first non-space character

    const char* psz = this->GetString();

    while (StringTraits::IsSpace(*psz)) {
      psz = StringTraits::CharNext(psz);
    }

    if (psz != this->GetString()) {
      // fix up data and length
      int iFirst = static_cast<int>(psz - this->GetString());
      char* pszBuffer = this->GetBuffer(this->GetLength());
      psz = pszBuffer + iFirst;
      int nDataLength = this->GetLength() - iFirst;
      memmove_s(pszBuffer, (this->GetLength() + 1) * sizeof(char), psz,
                (nDataLength + 1) * sizeof(char));
      this->ReleaseBufferSetLength(nDataLength);
    }

    return (*this);
  }

  // Remove all leading and trailing whitespace
  BStringT& Trim() { return (TrimRight().TrimLeft()); }

  // Remove all leading and trailing occurrences of character 'chTarget'
  BStringT& Trim(_In_ char chTarget) {
    return (TrimRight(chTarget).TrimLeft(chTarget));
  }

  /* Remove all leading and trailing occurrences of any of the characters in the
   * string 'pszTargets' */
  BStringT& Trim(_In_z_ const char* pszTargets) {
    return (TrimRight(pszTargets).TrimLeft(pszTargets));
  }

  void Print() {
    printf("%s\n", this->GetString());
    return;
  }

  // trimming anything (either side)

  // Remove all trailing occurrences of character 'chTarget'
  BStringT& TrimRight(_In_ char chTarget) {
    // find beginning of trailing matches
    // by starting at beginning (DBCS aware)

    const char* psz = this->GetString();
    const char* pszLast = NULL;

    while (*psz != 0) {
      if (*psz == chTarget) {
        if (pszLast == NULL) {
          pszLast = psz;
        }
      } else {
        pszLast = NULL;
      }
      psz = StringTraits::CharNext(psz);
    }

    if (pszLast != NULL) {
      // truncate at left-most matching character
      int iLast = static_cast<int>(pszLast - this->GetString());
      this->Truncate(iLast);
    }

    return (*this);
  }

  /* Remove all trailing occurrences of any of the characters in string
   * 'pszTargets' */
  BStringT& TrimRight(_In_z_ const char* pszTargets) {
    // if we're not trimming anything, we're not doing any work
    if ((pszTargets == NULL) || (*pszTargets == 0)) {
      return (*this);
    }

    // find beginning of trailing matches
    // by starting at beginning (DBCS aware)

    const char* psz = this->GetString();
    const char* pszLast = NULL;

    while (*psz != 0) {
      if (StringTraits::StringFindChar(pszTargets, *psz) != NULL) {
        if (pszLast == NULL) {
          pszLast = psz;
        }
      } else {
        pszLast = NULL;
      }
      psz = StringTraits::CharNext(psz);
    }

    if (pszLast != NULL) {
      // truncate at left-most matching character
      int iLast = static_cast<int>(pszLast - this->GetString());
      this->Truncate(iLast);
    }

    return (*this);
  }

  // Remove all leading occurrences of character 'chTarget'
  BStringT& TrimLeft(_In_ char chTarget) {
    // find first non-matching character
    const char* psz = this->GetString();

    while (chTarget == *psz) {
      psz = StringTraits::CharNext(psz);
    }

    if (psz != this->GetString()) {
      // fix up data and length
      int iFirst = static_cast<int>(psz - this->GetString());
      char* pszBuffer = this->GetBuffer(this->GetLength());
      psz = pszBuffer + iFirst;
      int nDataLength = this->GetLength() - iFirst;
      memmove_s(pszBuffer, (this->GetLength() + 1) * sizeof(char), psz,
                (nDataLength + 1) * sizeof(char));
      this->ReleaseBufferSetLength(nDataLength);
    }

    return (*this);
  }

  /* Remove all leading occurrences of any of the characters in string
   * 'pszTargets' */
  BStringT& TrimLeft(_In_z_ const char* pszTargets) {
    // if we're not trimming anything, we're not doing any work
    if ((pszTargets == NULL) || (*pszTargets == 0)) {
      return (*this);
    }

    const char* psz = this->GetString();
    while ((*psz != 0) &&
           (StringTraits::StringFindChar(pszTargets, *psz) != NULL)) {
      psz = StringTraits::CharNext(psz);
    }

    if (psz != this->GetString()) {
      // fix up data and length
      int iFirst = static_cast<int>(psz - this->GetString());
      char* pszBuffer = this->GetBuffer(this->GetLength());
      psz = pszBuffer + iFirst;
      int nDataLength = this->GetLength() - iFirst;
      memmove_s(pszBuffer, (this->GetLength() + 1) * sizeof(char), psz,
                (nDataLength + 1) * sizeof(char));
      this->ReleaseBufferSetLength(nDataLength);
    }

    return (*this);
  }

  friend BStringT operator+(_In_ const BStringT& str1,
                            _In_ const BStringT& str2) {
    BStringT strResult(str1.GetManager());

    CThisSimpleString::Concatenate(strResult, str1, str1.GetLength(), str2,
                                   str2.GetLength());

    return (strResult);
  }

  friend BStringT operator+(_In_ const BStringT& str1,
                            _In_z_ const char* psz2) {
    BStringT strResult(str1.GetManager());

    CThisSimpleString::Concatenate(strResult, str1, str1.GetLength(), psz2,
                                   CThisSimpleString::StringLength(psz2));

    return (strResult);
  }

  friend BStringT operator+(_In_z_ const char* psz1,
                            _In_ const BStringT& str2) {
    BStringT strResult(str2.GetManager());

    CThisSimpleString::Concatenate(strResult, psz1,
                                   CThisSimpleString::StringLength(psz1), str2,
                                   str2.GetLength());

    return (strResult);
  }

#define _CSTRING_CHAR_T char

  friend BStringT operator+(_In_ const BStringT& str1,
                            _In_ _CSTRING_CHAR_T ch2) {
    BStringT strResult(str1.GetManager());
    char chTemp = char(ch2);

    CThisSimpleString::Concatenate(strResult, str1, str1.GetLength(), &chTemp,
                                   1);

    return (strResult);
  }

  friend BStringT operator+(_In_ _CSTRING_CHAR_T ch1,
                            _In_ const BStringT& str2) {
    BStringT strResult(str2.GetManager());
    char chTemp = char(ch1);

    CThisSimpleString::Concatenate(strResult, &chTemp, 1, str2,
                                   str2.GetLength());

    return (strResult);
  }

  friend bool operator==(_In_ const BStringT& str1,
                         _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) == 0);
  }

  friend bool operator==(_In_ const BStringT& str1,
                         _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) == 0);
  }

  friend bool operator==(_In_z_ const char* psz1,
                         _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) == 0);
  }

  friend bool operator!=(_In_ const BStringT& str1,
                         _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) != 0);
  }

  friend bool operator!=(_In_ const BStringT& str1,
                         _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) != 0);
  }

  friend bool operator!=(_In_z_ const char* psz1,
                         _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) != 0);
  }

  friend bool operator<(_In_ const BStringT& str1,
                        _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) < 0);
  }

  friend bool operator<(_In_ const BStringT& str1,
                        _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) < 0);
  }

  friend bool operator<(_In_z_ const char* psz1,
                        _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) > 0);
  }

  friend bool operator>(_In_ const BStringT& str1,
                        _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) > 0);
  }

  friend bool operator>(_In_ const BStringT& str1,
                        _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) > 0);
  }

  friend bool operator>(_In_z_ const char* psz1,
                        _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) < 0);
  }

  friend bool operator<=(_In_ const BStringT& str1,
                         _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) <= 0);
  }

  friend bool operator<=(_In_ const BStringT& str1,
                         _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) <= 0);
  }

  friend bool operator<=(_In_z_ const char* psz1,
                         _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) >= 0);
  }

  friend bool operator>=(_In_ const BStringT& str1,
                         _In_ const BStringT& str2) throw() {
    return (str1.Compare(str2) >= 0);
  }

  friend bool operator>=(_In_ const BStringT& str1,
                         _In_z_ const char* psz2) throw() {
    return (str1.Compare(psz2) >= 0);
  }

  friend bool operator>=(_In_z_ const char* psz1,
                         _In_ const BStringT& str2) throw() {
    return (str2.Compare(psz1) <= 0);
  }

  friend bool operator==(_In_ char ch1, _In_ const BStringT& str2) throw() {
    return ((str2.GetLength() == 1) && (str2[0] == ch1));
  }

  friend bool operator==(_In_ const BStringT& str1, _In_ char ch2) throw() {
    return ((str1.GetLength() == 1) && (str1[0] == ch2));
  }

  friend bool operator!=(_In_ char ch1, _In_ const BStringT& str2) throw() {
    return ((str2.GetLength() != 1) || (str2[0] != ch1));
  }

  friend bool operator!=(_In_ const BStringT& str1, _In_ char ch2) throw() {
    return ((str1.GetLength() != 1) || (str1[0] != ch2));
  }
};

}  // namespace AMV

#endif  // BSTRINGT_HPP_
