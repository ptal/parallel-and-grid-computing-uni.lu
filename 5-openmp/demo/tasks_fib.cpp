#include <random>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

int dumb_fib(int n) {
  if(n == 0) return 0;
  else if(n <= 2) return 1;
  else {
    int i, j;
    #pragma omp task shared(i) if (n > 20)
    i = dumb_fib(n - 1);
    #pragma omp task shared(j) if (n > 20)
    j = dumb_fib(n - 1);
    #pragma omp taskwait
    return i + j;
  }
}

int main() {
  int n = 25;
  std::cout << "dumb_fib(" << n << ") = " << dumb_fib(n) << std::endl;
}