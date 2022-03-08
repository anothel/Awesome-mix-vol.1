// Copyright 2021~2022 `anothel` All rights reserved

#ifndef AMVALLOC_HPP_
#define AMVALLOC_HPP_

#include <climits>

#include "include/amvdefine.hpp"
#include "salieri-src/salieri.h"

namespace AMV {

template <typename T>
class AmvLimits;

template <>
class AmvLimits<int> {
 public:
  static const int _Min = INT_MIN;
  static const int _Max = INT_MAX;
};

template <>
class AmvLimits<unsigned int> {
 public:
  static const unsigned int _Min = 0;
  static const unsigned int _Max = UINT_MAX;
};

template <>
class AmvLimits<int64_t> {
 public:
  static const int64_t _Min = LONG_MIN;
  static const int64_t _Max = LONG_MAX;
};

template <>
class AmvLimits<uint64_t> {
 public:
  static const uint64_t _Min = 0;
  static const uint64_t _Max = ULONG_MAX;
};

/* generic version */
template <typename T>
inline int64_t AmvAdd(_Out_ T *ptResult, _In_ T tLeft, _In_ T tRight) {
  if (::AMV::AmvLimits<T>::_Max - tLeft < tRight) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *ptResult = tLeft + tRight;
  return S_OK;
}

template <typename T>
inline int64_t AmvAddThrow(_In_ T tLeft, _In_ T tRight) {
  T tResult;
  int64_t l;
  if ((l = AmvAdd(&tResult, tLeft, tRight)) != S_OK) {
    AmvThrow(l);
  }
  return tResult;
}

/* generic but comparatively slow version */
template <typename T>
inline int64_t AmvMultiply(_Out_ T *ptResult, _In_ T tLeft, _In_ T tRight) {
  /* avoid divide 0 */
  if (tLeft == 0) {
    *ptResult = 0;
    return S_OK;
  }
  if (::AMV::AmvLimits<T>::_Max / tLeft < tRight) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *ptResult = tLeft * tRight;
  return S_OK;
}

/* fast version for 32 bit integers */
template <>
inline int64_t AmvMultiply(_Out_ int *piResult, _In_ int iLeft,
                           _In_ int iRight) {
  int64_t i64Result =
      static_cast<int64_t>(iLeft) * static_cast<int64_t>(iRight);
  if (i64Result > INT_MAX || i64Result < INT_MIN) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *piResult = static_cast<int>(i64Result);
  return S_OK;
}

template <>
inline int64_t AmvMultiply(_Out_ unsigned int *piResult,
                           _In_ unsigned int iLeft, _In_ unsigned int iRight) {
  uint64_t i64Result =
      static_cast<uint64_t>(iLeft) * static_cast<uint64_t>(iRight);
  if (i64Result > UINT_MAX) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *piResult = static_cast<unsigned int>(i64Result);
  return S_OK;
}

template <>
inline int64_t AmvMultiply(_Out_ int64_t *piResult, _In_ int64_t iLeft,
                           _In_ int64_t iRight) {
  int64_t i64Result =
      static_cast<int64_t>(iLeft) * static_cast<int64_t>(iRight);
  if (i64Result > LONG_MAX || i64Result < LONG_MIN) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *piResult = static_cast<int64_t>(i64Result);
  return S_OK;
}

template <>
inline int64_t AmvMultiply(_Out_ uint64_t *piResult, _In_ uint64_t iLeft,
                           _In_ uint64_t iRight) {
  uint64_t i64Result =
      static_cast<uint64_t>(iLeft) * static_cast<uint64_t>(iRight);
  if (i64Result > ULONG_MAX) {
    return ERROR_ARITHMETIC_OVERFLOW;
  }
  *piResult = static_cast<uint64_t>(i64Result);
  return S_OK;
}

}  // namespace AMV

#endif  // AMVALLOC_HPP_
