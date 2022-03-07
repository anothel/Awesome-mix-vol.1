// Copyright 2021~2022 `anothel` All rights reserved

#ifndef A_HPP_
#define A_HPP_

#include <iostream>

template <typename T>
class CAAA {
 public:
  explicit CAAA(T a) : a_(a) { Print(); }

  virtual ~CAAA() { std::cout << "~CAAA" << std::endl; }

 protected:
  void Print() { std::cout << __FILE__ << "(" << __LINE__ << ")" << std::endl; }

 private:
  T a_;
};

#endif  // A_HPP_
