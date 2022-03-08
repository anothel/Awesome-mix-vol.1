// Copyright 2021~2022 `anothel` All rights reserved

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/CBBB.hpp"

TEST(Inheriting_from_a_template, test1) {
  std::cout << "Start Inheriting_from_a_template.test1 \n";
  CAAA<int> aaa(1);
  std::cout << "End Inheriting_from_a_template.test1 \n\n";
}

TEST(Inheriting_from_a_template, test2) {
  std::cout << "Start Inheriting_from_a_template.test2 \n";
  CBBB<int> bbb(2);
  std::cout << "End Inheriting_from_a_template.test2 \n\n";
}
