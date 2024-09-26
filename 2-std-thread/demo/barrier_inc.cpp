#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <barrier>

void inc(std::atomic<int>& x, std::barrier<>& b) {
  while(x.load() < 500000) {
    for(int i = 0; i != 100000; ++i) {
      ++x;
    }
    b.arrive_and_wait();
  }
}

int main() {
  std::atomic<int> x = 0;
  std::barrier<> b(2);
  std::thread t1(inc, std::ref(x), std::ref(b));
  std::thread t2(inc, std::ref(x), std::ref(b));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
