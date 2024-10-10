#include "A.hpp"
#include "B.hpp"

// To compile: g++ a.cpp b.cpp main.cpp -o main
// Note that the CPP files must be listed in the order of their dependencies.
int main() {
  A a(10);
  B b(a, 20);
  b.print();
  return 0;
}
