#include <random>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <cstdio>
#include <chrono>

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{0, 1};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

size_t num_zeroes_false_sharing(size_t nthreads, const std::vector<int>& data) {
  size_t zeroes[nthreads] = {0};
  size_t global_zeroes = 0;
  #pragma omp parallel num_threads(nthreads)
  {
    int tid = omp_get_thread_num();
    #pragma omp for
    for (size_t i = 0; i < data.size(); i++) {
      if(data[i] == 0) {
        ++zeroes[tid];
      }
    }
  }
  for(int i = 0; i < nthreads; ++i) {
    global_zeroes += zeroes[i];
  }
  return global_zeroes;
}

size_t num_zeroes_fixed(size_t nthreads, const std::vector<int>& data) {
  size_t padding = 16;
  size_t zeroes[nthreads * padding] = {0};
  size_t global_zeroes = 0;
  #pragma omp parallel num_threads(nthreads)
  {
    int tid = omp_get_thread_num();
    #pragma omp for
    for (size_t i = 0; i < data.size(); i++) {
      if(data[i] == 0) {
        ++zeroes[tid*padding];
      }
    }
  }
  for(int i = 0; i < nthreads; ++i) {
    global_zeroes += zeroes[i*padding];
  }
  return global_zeroes;
}

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cout << "usage: " << argv[0] << " <num_threads>" << std::endl;
    exit(1);
  }
  size_t nthreads = std::stoll(argv[1]);
  std::vector<int> v = init_random_vector(100000000);
  auto start = std::chrono::steady_clock::now();
  size_t zeroes1 = num_zeroes_false_sharing(nthreads, v);
  auto end = std::chrono::steady_clock::now();
  std::cout << "(false sharing) time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  std::cout << "zeroes1 = " << zeroes1 << std::endl;

  start = std::chrono::steady_clock::now();
  size_t zeroes2 = num_zeroes_fixed(nthreads, v);
  end = std::chrono::steady_clock::now();
  std::cout << "(fixed) time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  std::cout << "zeroes2 = " << zeroes2 << std::endl;
  return 0;
}
