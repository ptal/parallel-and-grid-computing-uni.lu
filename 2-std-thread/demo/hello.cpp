#include <iostream>
#include <thread>

void print_hello() {
  std::cout << "hello";
}

int main() {
  // the constructor starts the thread asynchronously.
  std::thread t1(print_hello);
  t1.join(); // wait for thread completion.
  std::cout << " world" << std::endl;
}
