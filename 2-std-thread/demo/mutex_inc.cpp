#include <iostream>
#include <thread>
#include <mutex>

// Great, we just sequentialized our parallel code :)
void inc(int& x, std::mutex& m) {
  std::lock_guard<std::mutex> guard(m);
  for(int i = 0; i != 100000; ++i) {
    x = x + 1;
  }
}

int main() {
  int x = 0;
  std::mutex m;
  std::thread t1(inc, std::ref(x), std::ref(m));
  std::thread t2(inc, std::ref(x), std::ref(m));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
