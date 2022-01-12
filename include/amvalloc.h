#include <climits>

#include "include/amvdefine.h"

namespace awesome_ATL {

template<typename T>
class AtlLimits;

template<>
class AtlLimits<int>
{
public:
	static const int _Min=INT_MIN;
	static const int _Max=INT_MAX;
};

template<>
class AtlLimits<unsigned int>
{
public:
	static const unsigned int _Min=0;
	static const unsigned int _Max=UINT_MAX;
};

template<>
class AtlLimits<long>
{
public:
	static const long _Min=LONG_MIN;
	static const long _Max=LONG_MAX;
};

template<>
class AtlLimits<unsigned long>
{
public:
	static const unsigned long _Min=0;
	static const unsigned long _Max=ULONG_MAX;
};

template<>
class AtlLimits<long long>
{
public:
	static const long long _Min=LLONG_MIN;
	static const long long _Max=LLONG_MAX;
};

template<>
class AtlLimits<unsigned long long>
{
public:
	static const unsigned long long _Min=0;
	static const unsigned long long _Max=ULLONG_MAX;
};

/* generic version */
template<typename T>
inline long AtlAdd(
	_Out_ T* ptResult,
	_In_ T tLeft,
	_In_ T tRight)
{
	if(::awesome_ATL::AtlLimits<T>::_Max-tLeft < tRight)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*ptResult= tLeft + tRight;
	return S_OK;
}

template <typename T>
inline long AtlAddThrow(
	_In_ T tLeft,
	_In_ T tRight)
{
	T tResult;
	long l;
	if( (l  = AtlAdd(&tResult, tLeft, tRight)) != S_OK)
	{
		AtlThrow(l);
	}
	return tResult;
}

/* generic but comparatively slow version */
template<typename T>
inline long AtlMultiply(
	_Out_ T* ptResult,
	_In_ T tLeft,
	_In_ T tRight)
{
	/* avoid divide 0 */
	if(tLeft==0)
	{
		*ptResult=0;
		return S_OK;
	}
	if(::awesome_ATL::AtlLimits<T>::_Max/tLeft < tRight)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*ptResult= tLeft * tRight;
	return S_OK;
}

/* fast version for 32 bit integers */
template<>
inline long AtlMultiply(
	_Out_ int *piResult,
	_In_ int iLeft,
	_In_ int iRight)
{
	long i64Result=static_cast<long>(iLeft) * static_cast<long>(iRight);
	if(i64Result>INT_MAX || i64Result < INT_MIN)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*piResult=static_cast<int>(i64Result);
	return S_OK;
}

template<>
inline long AtlMultiply(
	_Out_ unsigned int *piResult,
	_In_ unsigned int iLeft,
	_In_ unsigned int iRight)
{
	unsigned long i64Result=static_cast<unsigned long>(iLeft) * static_cast<unsigned long>(iRight);
	if(i64Result>UINT_MAX)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*piResult=static_cast<unsigned int>(i64Result);
	return S_OK;
}

template<>
inline long AtlMultiply(
	_Out_ long *piResult,
	_In_ long iLeft,
	_In_ long iRight)
{
	long i64Result=static_cast<long>(iLeft) * static_cast<long>(iRight);
	if(i64Result>LONG_MAX || i64Result < LONG_MIN)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*piResult=static_cast<long>(i64Result);
	return S_OK;
}

template<>
inline long AtlMultiply(
	_Out_ unsigned long *piResult,
	_In_ unsigned long iLeft,
	_In_ unsigned long iRight)
{
	unsigned long i64Result=static_cast<unsigned long>(iLeft) * static_cast<unsigned long>(iRight);
	if(i64Result>ULONG_MAX)
	{
		return ERROR_ARITHMETIC_OVERFLOW;
	}
	*piResult=static_cast<unsigned long>(i64Result);
	return S_OK;
}

}