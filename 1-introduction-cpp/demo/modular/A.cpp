#include "A.hpp"
#include <iostream>

A::A(int x): x(x) {}
void A::print() {
  std::cout << "A: " << x << std::endl;
}