// Copyright 2021~2022 `anothel` All rights reserved

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "include/amvstr.hpp"

char szDest[100];
const char *szSrc = "This is source";
const char *pszTestStr = "This is test | ";
char szTestStr[] = "This is test | This is test | ";
char szTestStrDel[] = "Thistest | This is test | ";
const char *szTestStrAA = "This is test | This is test | AA";
char sztestStringAA[] = "this is test | This is test | AA";
<<<<<<< HEAD:tests/BString_test.cc

TEST(BString, construct_test) {
  BString bstring1;
=======
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:tests/Awesome_mix_vol_1_test.cc

TEST(BStringT, construct)
{
    BString bstring1;

<<<<<<< HEAD:tests/BString_test.cc
  unsigned char uszTest[] = {0x00, 0x01, 0x02, 0x02};
  BString bstring3(uszTest, sizeof(uszTest));
}

TEST(BString, func_test) {
  try {
=======
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:tests/Awesome_mix_vol_1_test.cc
    std::string sTest("This is test | This is test | AA");
    BString bstring2(sTest.c_str());
    bstring1 = bstring2;

    unsigned char uszTest[] = {0x00, 0x01, 0x02, 0x02};
    BString bstring3(uszTest, sizeof(uszTest));
}

TEST(BStringT, func)
{
    try
    {
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

        bstring += static_cast<char>(45);
        ASSERT_NE(bstring.Find(static_cast<char>(45), 0), -1);

        ASSERT_NE(bstring.Find("This", 0), -1);

        ASSERT_NE(bstring.FindOneOf("is"), -1);

        ASSERT_NE(bstring.ReverseFind(static_cast<char>(45)), -1);

        BString bstring_1 = bstring + "    a  b  b  c  d     ";
        bstring_1.TrimRight();
        ASSERT_EQ(bstring_1.GetAt(bstring_1.GetLength() - 1), 'd');

        bstring_1.TrimRight('d');
        ASSERT_EQ(bstring_1.GetAt(bstring_1.GetLength() - 1), ' ');

        bstring_1.TrimRight(" cb");
        ASSERT_EQ(bstring_1.GetAt(bstring_1.GetLength() - 1), 'a');

        bstring_1 = "         a b c d  e";
        bstring_1.TrimLeft();
        ASSERT_EQ(bstring_1.GetAt(0), 'a');

        bstring_1.TrimLeft('a');
        ASSERT_EQ(bstring_1.GetAt(0), ' ');

        bstring_1.TrimLeft(" b");
        ASSERT_EQ(bstring_1.GetAt(0), 'c');

        bstring_1 = "    a b c e d    ";
        bstring_1.Trim();
        ASSERT_EQ(bstring_1.GetAt(0), 'a');
        ASSERT_EQ(bstring_1.GetAt(bstring_1.GetLength() - 1), 'd');

<<<<<<< HEAD:tests/BString_test.cc
    bstring_1.Print();
  } catch (...) {
  }
}

TEST(BString, CSimpleStringT_test) {
  try {
    BString bstring;
=======
        bstring_1.Trim('a');
        ASSERT_EQ(bstring_1, " b c e d");
        bstring_1.Trim(" bd");
        ASSERT_EQ(bstring_1, "c e");

        bstring_1.Print();
    }
    catch (...)
    {
    }
}

TEST(_Func_UnitTest, CSimpleStringT)
{
    try
    {
        BString bstring;
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:tests/Awesome_mix_vol_1_test.cc

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

        bstring = "01234567";
        int before = bstring.GetAllocLength();
        bstring = "A";
        bstring.FreeExtra();
        ASSERT_TRUE(bstring.GetAllocLength() < before);

        ASSERT_EQ(bstring.GetAllocLength(), 7);

        ASSERT_EQ(bstring.GetAt(0), 'A');

        ASSERT_EQ(memcmp(bstring.GetBuffer(), "A", strlen("A")), S_OK);

        ASSERT_EQ(memcmp(bstring.GetBufferSetLength(3), "A", strlen("A")), S_OK);

        ASSERT_EQ(bstring.GetLength(), 3);

        ASSERT_EQ(memcmp(bstring.GetString(), "A", strlen("A")), S_OK);

        ASSERT_EQ(bstring.IsEmpty(), false);

        bstring.LockBuffer();
        bstring.UnlockBuffer();

        bstring.Preallocate(3);

        bstring.ReleaseBuffer();

        bstring.ReleaseBufferSetLength(3);

        bstring.Truncate(2);

        bstring.SetAt(0, 'a');
        ASSERT_EQ(memcmp(bstring.GetBuffer(), "a", strlen("a")), S_OK);

        // static AMV::CAmvHeap strHeap;
        // bstring.SetManager(bstring.GetManager());

<<<<<<< HEAD:tests/BString_test.cc
    bstring.SetString("CAAA");
    ASSERT_EQ(memcmp(bstring.GetBuffer(), "CAAA", strlen("CAAA")), S_OK);
  } catch (...) {
  }
=======
        bstring.SetString("AAA");
        ASSERT_EQ(memcmp(bstring.GetBuffer(), "AAA", strlen("AAA")), S_OK);
    }
    catch (...)
    {
    }
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:tests/Awesome_mix_vol_1_test.cc
}

TEST(BString, operator_plus_test) {
  BString bstring1("a");
  BString bstring2("b");
  std::cout << bstring2 + 'A' << " \n";
  std::cout << 'A' + bstring1 << " \n";
}
