#include <iostream>
#include <thread>

void f1(int& x, int& y) {
  int r1 = x;
  y = 1;
  std::cout << "r1 = " << r1 << std::endl;
}

void f2(int& x, int& y) {
  int r2 = y;
  x = 1;
  std::cout << "r2 = " << r2 << std::endl;
}

int main() {
  int x = 0;
  int y = 0;
  std::jthread t1(f1, std::ref(x), std::ref(y));
  std::jthread t2(f2, std::ref(x), std::ref(y));
}