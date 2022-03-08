// Copyright 2021~2022 `anothel` All rights reserved

#ifndef CBBB_HPP_
#define CBBB_HPP_

#include "include/CAAA.hpp"

template <typename U>
class CBBB : public CAAA<U> {
 public:
  explicit CBBB(U b) throw() : CAAA<U>(b), b_(b) {
    std::cout << "Construct of CBBB \n";
  }
  ~CBBB() { std::cout << "Destruct of CBBB \n"; }

 private:
  U b_;
};

#endif  // CBBB_HPP_
