// Copyright ⓒ2022 AWESOME-MIX-VOL.1 Co.,Ltd. All rights reserved

#ifndef B_H_
#define B_H_

#include "include/a.h"

template <typename U>
class BBB : public AAA<U>
{
public:
    BBB(U b)
    throw() : AAA<U>(b), b_(b) { Print(); }
    ~BBB() { std::cout << "~BBB" << std::endl; }

protected:
    void Print() { std::cout << __FILE__ << "(" << __LINE__ << ")" << std::endl; }

private:
    U b_;
};

#endif // B_H_
