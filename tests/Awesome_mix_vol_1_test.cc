// Copyright

// #include "include/ByteString.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/amvstr.h"
#include "include/b.h"

TEST(Inheriting_from_a_template, test1) { AAA<int> aaa(1); }

TEST(Inheriting_from_a_template, test2) { BBB<int> bbb(2); }

TEST(BStringTest1, test1) { 
  try {
  BString bstring;
  const char* cpsz = "This is test 1";

  // awesome_ATL::CSimpleStringT<char> simplestring1(::AfxGetStringManager());
  
  // a.Append(simplestring1);

  bstring.Append(cpsz);
  bstring.Append(cpsz, strlen(cpsz));
  } catch (...) {

  }
}

TEST(BStringTest1, test2) { 
  BString bstring;
  // const char* cpsz = "This is test 2";

  // awesome_ATL::CSimpleStringT<char> simplestring2(cpsz, ::AfxGetStringManager());
  // awesome_ATL::CSimpleStringT<char> simplestring3(cpsz, strlen(cpsz), ::AfxGetStringManager());
  // bstring.Append(simplestring2);
}

// TEST(BStringTest1, test3) { 
//   BString bstring;
//   // const char* cpsz = "This is test 3";

//   // awesome_ATL::CSimpleStringT<char> simplestring3(cpsz, strlen(cpsz), ::AfxGetStringManager());
//   // bstring.Append(simplestring3);
// }

// TEST(ByteStringTest2, RectangleSize_false) {}
