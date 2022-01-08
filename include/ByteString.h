// Copyright

#ifndef INCLUDE_BYTESTRING_H_
#define INCLUDE_BYTESTRING_H_

#include <stdint.h>

#include <string>

namespace Awesome_mix_vol_1 {

class ByteString {
public:
	ByteString();
	ByteString(const char *psz);
	ByteString(const ByteString& stringSrc);

	~ByteString();

	int GetLength() const;
	bool IsEmpty()const;
	void Empty();
	void SetAt(int nIndex, char ch);
	int Compare(const ByteString& s) const;
	int Find(char ch) const;
	int Find(char *pszSub) const;

	ByteString Mid(int nFirst, int nCount) const;
	ByteString Left(int nCount) const;
	ByteString Right(int nCount) const;

	int GetMem() const;
	
	ByteString& operator=(const ByteString& stringSrc);
	const ByteString& operator=(const unsigned char* psz);
	char operator [](int indx);
	bool operator ==(const ByteString& string);
	ByteString operator +(const ByteString& string);
	ByteString& operator +=(const ByteString& string);
	
	void Print();
private:
	//variables
	char* str;
	int nDataLength;
	int nAllocLength;
};

}  // namespace Awesome_mix_vol_1

#endif  // INCLUDE_BYTESTRING_H_
