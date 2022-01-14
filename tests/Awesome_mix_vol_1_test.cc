// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "include/amvstr.h"
#include "include/b.h"

TEST(Inheriting_from_a_template, test1) { AAA<int> aaa(1); }

TEST(Inheriting_from_a_template, test2) { BBB<int> bbb(2); }

char szDest[100];
const char* szSrc = "This is source";
const char* pszTestStr = "This is test | ";
char szTestStr[] = "This is test | This is test | ";
char szTestStrDel[] = "Thistest | This is test | ";
char szTestStrAA[] = "This is test | This is test | AA";
char sztestStringAA[] = "this is test | This is test | AA";

TEST(_Func_UnitTest, BStringT) {
  try {
    BString bstring(szTestStrAA);

    ASSERT_EQ(bstring.Compare(szTestStrAA), S_OK);

    ASSERT_EQ(bstring.CompareNoCase(sztestStringAA), S_OK);

    bstring.Delete(4, 4);
    ASSERT_EQ(memcmp(bstring, szTestStrDel, strlen(szTestStrDel)), S_OK);
  } catch (...) {
  }
}

TEST(_Func_UnitTest, CSimpleStringT) {
  try {
    BString bstring;

    bstring.Append(pszTestStr);
    bstring.Append(pszTestStr, strlen(pszTestStr));
    ASSERT_EQ(memcmp(bstring, szTestStr, strlen(szTestStr)), S_OK);

    bstring.AppendChar('A');
    bstring.AppendChar('A');
    ASSERT_EQ(memcmp(bstring, szTestStrAA, strlen(szTestStrAA)), S_OK);

    bstring.CopyChars(szDest, szSrc, strlen(szSrc));
    ASSERT_EQ(memcmp(szDest, szSrc, strlen(szSrc)), S_OK);

    bstring.CopyChars(szDest, sizeof(szDest), szSrc, strlen(szSrc));
    ASSERT_EQ(memcmp(szDest, szSrc, strlen(szSrc)), S_OK);

    bstring.CopyCharsOverlapped(szDest, szSrc, strlen(szSrc));
    ASSERT_EQ(memcmp(szDest, szSrc, strlen(szSrc)), S_OK);

    bstring.Empty();
    ASSERT_EQ(bstring.GetLength(), 0);
  } catch (...) {
  }
}

TEST(BStringTest1, test3) { BString bstring; }

TEST(ByteStringTest2, RectangleSize_false) {}
