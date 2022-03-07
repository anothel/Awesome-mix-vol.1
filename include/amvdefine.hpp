// Copyright 2021~2022 `anothel` All rights reserved

#ifndef AMVDEFINE_HPP_
#define AMVDEFINE_HPP_

#ifdef __GNUC__
#define __interface class
#define AMVASSERT(x) assert(x)
#define AMVENSURE(x) assert(x)
#define AMVASSUME(x) assert(x)
#define AMVENSURE_RETURN_VAL(x, y) assert(x)
#define FAILED(hr) (hr < 0)
#define AMVENSURE_THROW(x, s) assert(x)
#define _InterlockedIncrement(x) __sync_fetch_and_add((x), 1)
#define _InterlockedDecrement(x) __sync_fetch_and_sub((x), 1)
#define __cdecl
#define _AMV_INSECURE_DEPRECATE(s)
#define AmvThrow throw
#define __declspec(s)
#endif  // __GNUC__

#ifdef _AMV_DISABLE_NOINLINE
#define AMV_NOINLINE
#else
#define AMV_NOINLINE __declspec(noinline)
#endif

#ifndef _WIN32
#define CSTRING_EXPLICIT explicit
#endif  // _WIN32

typedef uint16_t WORD;

typedef unsigned int UINT;
typedef int64_t INT_PTR, *PINT_PTR;
typedef uint64_t UINT_PTR, *PUINT_PTR;
typedef uint64_t DWORD;

typedef char *LPSTR;
typedef uint64_t ULONG;

typedef const void *LPCVOID;

#define memmove_s(d, dl, s, sl) memmove(d, s, sl);
#define memcpy_s(d, dl, s, sl) memcpy(d, s, sl);

#define BOOL int

typedef uint64_t ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

#define S_OK ((int64_t)0L)
#define S_FALSE ((int64_t)1L)

#define ERROR_ARITHMETIC_OVERFLOW 534L

// Minimum and maximum macros
#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

#define IS_INTRESOURCE(_r) ((((ULONG_PTR)(_r)) >> 16) == 0)

#ifndef LOWORD
#define LOWORD(_dw) ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#endif

#ifndef _WIN32
#define CSTRING_EXPLICIT explicit
#endif  // _WIN32

#define log1 printf("[jpk] %s(%d), %s \n", __FILE__, __LINE__, __func__);

#endif  // AMVDEFINE_HPP_
