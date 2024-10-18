#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>

std::vector<double> proportion(const std::vector<int>& data) {
  std::vector<double> p(data.size());
  int total = 0;
  #pragma omp parallel for reduction(+:total)
  for (int i = 0; i < data.size(); ++i) {
    total += data[i];
  }
  // Implicit barrier
  #pragma omp parallel for
  for (int i = 0; i < data.size(); ++i) {
    p[i] = static_cast<double>(data[i]) / static_cast<double>(total);
  }
  // Implicit barrier
  return std::move(p);
}

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 100};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  std::vector<int> a = init_random_vector(1000000);
  std::vector<double> p = proportion(a);
  double sum_p = 0.0;
  for(double d : p) {
    sum_p += d;
  }
  std::cout << "Sum: " << sum_p << std::endl;
}
