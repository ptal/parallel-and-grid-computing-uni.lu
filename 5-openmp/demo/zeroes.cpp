#include <random>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <cstdio>
#include <chrono>

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{0, 100};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

size_t num_zeroes(const std::vector<int>& data) {
  size_t global_zeroes = 0;
  #pragma omp parallel
  {
    #pragma omp for
    for (size_t i = 0; i < data.size(); i++) {
      if(data[i] == 0) {
        #pragma omp atomic
        ++global_zeroes;
      }
    }
  }
  return global_zeroes;
}

size_t num_zeroes_less_atomics(const std::vector<int>& data) {
  size_t zeroes = 0;
  size_t global_zeroes = 0;
  #pragma omp parallel firstprivate(zeroes)
  {
    #pragma omp for
    for (size_t i = 0; i < data.size(); i++) {
      if(data[i] == 0) {
        ++zeroes;
      }
    }
    // #pragma omp critical
    // {
      // printf("%d: local_zeroes = %d\n", omp_get_thread_num(), zeroes);
    // }
    #pragma omp atomic
    global_zeroes += zeroes;
  }
  return global_zeroes;
}

int main() {
  std::vector<int> v = init_random_vector(10000000);
  auto start = std::chrono::steady_clock::now();
  size_t zeroes1 = num_zeroes(v);
  auto end = std::chrono::steady_clock::now();
  std::cout << "(global counter) time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  std::cout << "zeroes1 = " << zeroes1 << std::endl;

  start = std::chrono::steady_clock::now();
  size_t zeroes2 = num_zeroes_less_atomics(v);
  end = std::chrono::steady_clock::now();
  std::cout << "(local counter) time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  std::cout << "zeroes2 = " << zeroes2 << std::endl;
  return 0;
}
