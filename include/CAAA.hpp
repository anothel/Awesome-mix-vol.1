// Copyright 2021~2022 `anothel` All rights reserved

#ifndef CAAA_HPP_
#define CAAA_HPP_

#include <iostream>

template <typename T>
class CAAA {
 public:
  explicit CAAA(T a) : a_(a) { std::cout << "Construct of CAAA \n"; }

  virtual ~CAAA() { std::cout << "Destruct of CAAA \n"; }

 private:
  T a_;
};

#endif  // CAAA_HPP_
