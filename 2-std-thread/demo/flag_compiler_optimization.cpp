#include <iostream>
#include <thread>

void wait_bool(bool& b, int& x) {
  while(b) {
    // do stuff.
    x++;
  }
}

void stop_bool(bool& b, int& x) {
  while(x < 10) {
  }
  b = false;
}

int main() {
  int x = 0;
  bool b = true;
  std::thread t1(wait_bool, std::ref(b), std::ref(x));
  std::thread t2(stop_bool, std::ref(b), std::ref(x));
  t1.join(); // wait for thread completion.
  t2.join(); // wait for thread completion.
  std::cout << x << std::endl;
}
