#include <iostream>
#include <thread>

void f1(int& x) {
  x = 1;
}

void f2(int& x) {
  x = 2;
}

int main() {
  int x = 0;
  std::thread t1(f1, std::ref(x));
  std::thread t2(f2, std::ref(x));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}