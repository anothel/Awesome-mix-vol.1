// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#ifndef A_H_
#define A_H_

#include <iostream>

template <typename T>
class AAA {
 public:
  explicit AAA(T a) : a_(a) {
    //
    Print();
  }

  virtual ~AAA() {
    //
    std::cout << "~AAA" << std::endl;
  }

 protected:
  void Print() {
    std::cout << "AAAAAAAAAA" << std::endl;
    std::cout << "a_: " << a_ << std::endl;
  }

 private:
  T a_;
};

#endif  // A_H_
