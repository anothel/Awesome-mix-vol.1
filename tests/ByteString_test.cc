// Copyright

#include "include/ByteString.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/b.h"
// #include "include/cstringt.h"

namespace Awesome_mix_vol_1 {

TEST(Inheriting_from_a_template, test1) { AAA<int> aaa(1); }

TEST(Inheriting_from_a_template, test2) { BBB<int> bbb(2); }

TEST(CStringTest1, test1) {}

TEST(ByteStringTest2, RectangleSize_false) {}

}  // namespace Awesome_mix_vol_1
