#include <random>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <cstdio>

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 100};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  int num_procs = omp_get_num_procs();
  std::cout << "Number of available processors: " << num_procs << std::endl;
  std::vector<int> v = init_random_vector(100000);
  // omp_set_num_threads(num_procs * 2);
  // #pragma omp parallel for num_threads(num_procs * 2)
  #pragma omp parallel for
  for (int i = 0; i < v.size(); i++) {
    v[i] *= 2;
    if(i % 1000 == 0) {
      #pragma omp critical
      {
        /* This block will be executed by at most one thread at a time. */
        printf("Thread %d got iteration %lu\n", omp_get_thread_num(), i);
      }
    }
  }
  return 0;
}
