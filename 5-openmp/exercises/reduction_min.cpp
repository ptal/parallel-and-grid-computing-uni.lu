#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

int reduction_min(const std::vector<int>& a) {
  return 0;
}

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 100000000};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  std::vector<int> a = init_random_vector(1000000000);
  auto start = std::chrono::steady_clock::now();
  int m = reduction_min(a);
  auto end = std::chrono::steady_clock::now();
  std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  std::cout << "result = " << m << std::endl;
}
