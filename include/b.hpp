// Copyright 2021~2022 `anothel` All rights reserved

#ifndef B_HPP_
#define B_HPP_

#include "include/a.hpp"

template <typename U>
class BBB : public AAA<U> {
 public:
  explicit BBB(U b) throw() : AAA<U>(b), b_(b) { Print(); }
  ~BBB() { std::cout << "~BBB" << std::endl; }

 protected:
  void Print() { std::cout << __FILE__ << "(" << __LINE__ << ")" << std::endl; }

 private:
  U b_;
};

#endif  // B_HPP_
