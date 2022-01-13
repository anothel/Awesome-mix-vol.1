// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/amvstr.h"
#include "include/b.h"

TEST(Inheriting_from_a_template, test1) {
  //
  AAA<int> aaa(1);
}

TEST(Inheriting_from_a_template, test2) {
  //
  BBB<int> bbb(2);
}

TEST(BStringTest1, test1) {
  try {
    BString bstring;
    const char* cpsz = "This is test 1";

    bstring.Append(cpsz);
    bstring.Append(cpsz, strlen(cpsz));
  } catch (...) {
  }
}

TEST(BStringTest1, test2) {
  //
  BString bstring;
}

TEST(BStringTest1, test3) {
  //
  BString bstring;
}

TEST(ByteStringTest2, RectangleSize_false) {}
