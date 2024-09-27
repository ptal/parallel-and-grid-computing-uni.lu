#include <iostream>
#include <thread>
#include <atomic>

void wait_bool(std::atomic<bool>& b, std::atomic<int>& x) {
  while(b.load()) {
    // do stuff.
    x++;
  }
}

void stop_bool(std::atomic<bool>& b, std::atomic<int>& x) {
  while(x.load() < 10) {
  }
  b.store(false);
}

int main() {
  std::atomic<int> x = 0;
  std::atomic<bool> b = true;
  std::thread t1(wait_bool, std::ref(b), std::ref(x));
  std::thread t2(stop_bool, std::ref(b), std::ref(x));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
