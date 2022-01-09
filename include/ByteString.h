// Copyright

#ifndef INCLUDE_BYTESTRING_H_
#define INCLUDE_BYTESTRING_H_

#include <stdint.h>

#include <string>

namespace Awesome_mix_vol_1 {

class ByteString {
 public:
  ByteString();
  ~ByteString();

 private:
  char* m_pszData;
  int64_t nDataLength;
  int64_t nAllocLength;
};

}  // namespace Awesome_mix_vol_1

#endif  // INCLUDE_BYTESTRING_H_
