#include <vector>
#include <iostream>
#include <thread>
#include <span>
#include <random>
#include <algorithm>

void map(auto& values, auto f) {
  for(auto& x : values) {
    x = f(x);
  }
}

template <class T>
void parallel_map(std::vector<T>& values, auto f) {
  size_t mid = values.size() / 2;
  std::span half1(values.begin(), mid);
  std::span half2(values.begin() + mid, values.size() - mid);
  std::jthread a([&](){ map(half1, f); });
  std::jthread b([&](){ map(half2, f); });
}

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 100};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

template<class F>
long benchmark_one_ms(F&& f) {
  auto start = std::chrono::steady_clock::now();
  f();
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void check_equal_vector(const std::vector<int>& a, const std::vector<int>& b) {
  if(a.size() != b.size()) {
    std::cerr << "Size of arrays differs..." << std::endl;
  }
  else {
    for(size_t i = 0; i < a.size(); ++i) {
      if(a[i] != b[i]) {
        std::cerr << "Found an error: " << a[i] << " != " << b[i] << " at index " << i << std::endl;
        exit(1);
      }
    }
  }
}

// A dumb function to have a map with more CPU work per element in the array.
int more_work(int x) {
  int i = 0;
  while(x > 0) {
    x /= 2;
    i++;
  }
  return i;
}

int main(int argc, char** argv) {
  if(argc != 3) {
    std::cout << "usage: " << argv[0] << " <vector size> <num_threads>" << std::endl;
    exit(1);
  }
  size_t n = std::stoll(argv[1]);
  size_t num_threads = std::stoi(argv[2]);
  std::vector<int> v1 = init_random_vector(n);
  std::vector<int> v2 = v1;
  auto twice = [](int x) { return x * 2; };
  long single_thread_ms = benchmark_one_ms([&](){ map(v1, twice); });
  long multithreaded_ms = benchmark_one_ms([&](){ parallel_map(v2, twice); });
  std::cout << "Single threaded: " << single_thread_ms << "ms" << std::endl;
  std::cout << num_threads << " threads: " << multithreaded_ms << "ms" << std::endl;
  check_equal_vector(v1, v2);
  return 0;
}
