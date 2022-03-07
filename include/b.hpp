// Copyright 2021~2022 `anothel` All rights reserved

#ifndef B_HPP_
#define B_HPP_

#include "include/a.hpp"

template <typename U>
class CBBB : public CAAA<U> {
 public:
  explicit CBBB(U b) throw() : CAAA<U>(b), b_(b) { Print(); }
  ~CBBB() { std::cout << "~CBBB" << std::endl; }

 protected:
  void Print() { std::cout << __FILE__ << "(" << __LINE__ << ")" << std::endl; }

 private:
  U b_;
};

#endif  // B_HPP_
