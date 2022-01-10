#include "include/a.h"

template <typename U>
class BBB : public AAA<U> {
 public:
  BBB(U b) throw() : AAA<U>(b), b_(b) { Print(); }
  ~BBB() { std::cout << "~BBB" << std::endl; }

 protected:
  void Print() {
    std::cout << "BBBBBBBBBB" << std::endl;
    std::cout << "b_: " << b_ << std::endl;
  }

 private:
  U b_;
};
