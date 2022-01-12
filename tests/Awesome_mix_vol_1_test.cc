// Copyright

// #include "include/ByteString.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/atlstr.h"
#include "include/b.h"

TEST(Inheriting_from_a_template, test1) { AAA<int> aaa(1); }

TEST(Inheriting_from_a_template, test2) { BBB<int> bbb(2); }

TEST(CStringTest1, test1) { 
  try {
  CString a;
  const char* cpsz = "This is test 1";

  // awesome_ATL::CSimpleStringT<char> simplestring1(::AfxGetStringManager());
  
  // a.Append(simplestring1);

  a.Append(cpsz);
  a.Append(cpsz, strlen(cpsz));
  printf("[jpk] a: %s \n", a.GetBuffer());
  } catch (int i) {
printf("[jpk] i: %d \n", i);
  } catch (char * s) {
printf("[jpk] %s(%d) \n", __FILE__, __LINE__);
  } catch (const char * s) {
printf("[jpk] %s(%d) \n", __FILE__, __LINE__);
  }
}

TEST(CStringTest1, test2) { 
  CString a;
  // const char* cpsz = "This is test 2";

  // awesome_ATL::CSimpleStringT<char> simplestring2(cpsz, ::AfxGetStringManager());
  // awesome_ATL::CSimpleStringT<char> simplestring3(cpsz, strlen(cpsz), ::AfxGetStringManager());
  // a.Append(simplestring2);
}

// TEST(CStringTest1, test3) { 
//   CString a;
//   // const char* cpsz = "This is test 3";

//   // awesome_ATL::CSimpleStringT<char> simplestring3(cpsz, strlen(cpsz), ::AfxGetStringManager());
//   // a.Append(simplestring3);
// }

// TEST(ByteStringTest2, RectangleSize_false) {}
