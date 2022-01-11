#include <limits.h>

#include "include/awesomedefine.h"

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
inline T AtlAddThrow(
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

}