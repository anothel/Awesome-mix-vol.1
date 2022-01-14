// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "include/amvstr.h"
#include "include/b.h"

TEST(Inheriting_from_a_template, test1) { AAA<int> aaa(1); }

TEST(Inheriting_from_a_template, test2) { BBB<int> bbb(2); }

char szDest[100];
const char* szSrc = "This is source";
const char* pszTestStr = "This is test | ";
char szTestStr[] = "This is test | This is test | ";
char szTestStrDel[] = "Thistest | This is test | ";
const char* szTestStrAA = "This is test | This is test | AA";
char sztestStringAA[] = "this is test | This is test | AA";

TEST(BStringT, construct) {}

TEST(BStringT, func) {
  try {
    std::string sTest("This is test | This is test | AA");
    BString bstring(sTest.c_str());

    ASSERT_EQ(bstring.Compare(szTestStrAA), S_OK);

    ASSERT_EQ(bstring.CompareNoCase(sztestStringAA), S_OK);

    bstring.Delete(4, 4);
    ASSERT_EQ(memcmp(bstring, szTestStrDel, strlen(szTestStrDel)), S_OK);

    bstring = sTest.c_str();
    bstring.Insert(0, "^");
    ASSERT_EQ(std::string(bstring), std::string("^" + sTest));

    bstring.Insert(0, static_cast<char>(45));
    ASSERT_EQ(std::string(bstring), std::string("-^" + sTest));

    bstring.Replace("^", "");
    ASSERT_EQ(std::string(bstring), std::string("-" + sTest));

    bstring.Replace(static_cast<char>(45), static_cast<char>(95));
    ASSERT_EQ(std::string(bstring), std::string("_" + sTest));

    bstring.Remove(static_cast<char>(95));
    ASSERT_EQ(std::string(bstring), std::string(sTest));

BString str("ABC|DEF|EEE|");
BString token;
int pos = 0;
while ((token  = str.Tokenize("|", pos)) != "")
    printf("%s\n", token);

    // bstring.Tokenize();
    // bstring.Find();
    // bstring.FindOneOf();
    // bstring.ReverseFind();
    // bstring.MakeUpper();
    // bstring.MakeLower();
    // bstring.MakeReverse();
    // bstring.TrimRight();
    // bstring.TrimLeft();
    // bstring.Trim();
    // bstring.Mid();
    // bstring.Right();
    // bstring.Left();
    // bstring.SpanIncluding();
    // bstring.SpanExcluding();
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

    // bstring.FreeExtra();
    // bstring.GetAllocLength();
    // bstring.GetAt();
    // bstring.GetBuffer();
    // bstring.GetBufferSetLength();
    // bstring.GetLength();
    // bstring.GetString();
    // bstring.IsEmpty();
    // bstring.UnlockBuffer();
    // bstring.Preallocate();
    // bstring.ReleaseBuffer();
    // bstring.ReleaseBufferSetLength();
    // bstring.Truncate();
    // bstring.SetAt();
    // bstring.SetManager();
    // bstring.SetString();
    // bstring.Concatenate();
  } catch (...) {
  }
}

TEST(BStringTest1, test3) { BString bstring; }

TEST(ByteStringTest2, RectangleSize_false) {}
