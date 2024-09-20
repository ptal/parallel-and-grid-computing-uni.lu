#include <vector>
#include <iostream>

template <class T>
void filter(const std::vector<T>& values, auto f, std::vector<T>& output) {
  for(int x : values) {
    if(f(x)) {
      output.push_back(x);
    }
  }
}

int main() {
  std::vector<int> v{4,2,1,4,5};
  std::vector<int> o;
  int threshold = 3;
  filter(v, [threshold](int x) { return x < threshold; }, o);
  for(int x: o) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}