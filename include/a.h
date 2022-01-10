#include <iostream>

template <typename T>
class AAA {
 public:
  AAA(T a) : a_(a) { Print(); }
  virtual ~AAA() { std::cout << "~AAA" << std::endl; }

 protected:
  void Print() {
    std::cout << "AAAAAAAAAA" << std::endl;
    std::cout << "a_: " << a_ << std::endl;
  }

 private:
  T a_;
};