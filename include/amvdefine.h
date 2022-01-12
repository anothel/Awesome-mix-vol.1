#ifdef __GNUC__
#define __interface class
#define ATLASSERT(x) assert(x)
#define ATLENSURE(x) assert(x)
#define ATLASSUME(x) assert(x)
#define ATLENSURE_RETURN_VAL(x, y) assert(x)
#define FAILED(hr) (hr < 0)
#define ATLENSURE_THROW(x, s) assert(x)
#define _InterlockedIncrement(x) __sync_fetch_and_add((x), 1)
#define _InterlockedDecrement(x) __sync_fetch_and_sub((x), 1)
#define __cdecl
#define _ATL_INSECURE_DEPRECATE(s)
#define AtlThrow throw
#define __declspec(s)
#endif  // __GNUC__

#ifdef _ATL_DISABLE_NOINLINE
#define ATL_NOINLINE
#else
#define ATL_NOINLINE __declspec(noinline)
#endif

typedef unsigned short      WORD;

typedef unsigned int UINT;
typedef int64_t INT_PTR, *PINT_PTR;
typedef uint64_t UINT_PTR, *PUINT_PTR;
typedef unsigned long DWORD;

typedef char CHAR;
typedef CHAR *LPSTR;
typedef const CHAR *LPCSTR;
typedef unsigned long ULONG;

typedef const void *LPCVOID;

#define BOOL int

typedef uint64_t ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

#define S_OK ((long)0L)
#define S_FALSE ((long)1L)

#define ERROR_ARITHMETIC_OVERFLOW 534L

// Minimum and maximum macros
#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

#define IS_INTRESOURCE(_r) ((((ULONG_PTR)(_r)) >> 16) == 0)

#ifndef LOWORD
#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#endif

#ifndef _WIN32
#define CSTRING_EXPLICIT explicit
#endif  // _WIN32

#define log1 printf("[jpk] %s(%d), %s \n", __FILE__, __LINE__, __func__);