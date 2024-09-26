#include <iostream>
#include <thread>
#include <atomic>

// Atomics, but the code is still mostly sequentialized :)
void inc(std::atomic<int>& x) {
  for(int i = 0; i != 100000; ++i) {
    ++x; // or `x.fetch_add(1);`
  }
}

int main() {
  std::atomic<int> x = 0;
  std::thread t1(inc, std::ref(x));
  std::thread t2(inc, std::ref(x));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
