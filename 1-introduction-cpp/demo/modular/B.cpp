#include "B.hpp"
#include <iostream>

B::B(const A& a, int x): a(a), x(x) {}

void B::print() {
  a.print();
  std::cout << "B: " << x << std::endl;
}