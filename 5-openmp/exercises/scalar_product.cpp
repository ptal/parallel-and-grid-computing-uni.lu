#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>

int scalar_product(const std::vector<int>& a, const std::vector<int>& b) {
  assert(a.size() == b.size());
  int s = 0;
  for(int i = 0; i < a.size(); ++i) {
    s += a[i] * b[i];
  }
  return s;
}

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 100};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  std::vector<int> a = init_random_vector(100);
  std::vector<int> b = init_random_vector(100);
  std::cout << "Result: " << scalar_product(a, b) << std::endl;
}
