#include "A.hpp"

#ifndef B_HPP
#define B_HPP

class B {
  int x;
  A a;
public:
  B(const A& a, int x);
  void print();
};

#endif
