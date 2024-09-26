#include <iostream>
#include <thread>

// `x` is a shared variable.
void inc(int& x) {
  for(int i = 0; i != 100000; ++i) {
    x = x + 1;
  }
}

int main() {
  int x = 0;
  std::thread t1(inc, std::ref(x));
  std::thread t2(inc, std::ref(x));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
