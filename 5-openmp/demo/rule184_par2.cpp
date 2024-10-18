#include <iostream>
#include <cstdio>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <omp.h>

#define VERBOSE

#ifdef VERBOSE
// Surround an instruction `S` by two timers in order to measure and print how much time it took to be executed.
  #define CLOCK_PRINT(i, K, MSG, S) \
    auto start##K = std::chrono::steady_clock::now(); \
    S; \
    auto end##K = std::chrono::steady_clock::now(); \
    std::cout << "[" << i << "] " << MSG << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(end##K - start##K).count() << "ms\n";
#else
  #define CLOCK_PRINT(i, K, MSG, S) S;
#endif

std::vector<int> initialize_cells(size_t n) {
  std::vector<int> cells(n);
  // std::mt19937 m{std::random_device{}()};
  std::mt19937 m{0}; // fix the seed to ease debugging.
  std::discrete_distribution<> d({10, 90});
  for(size_t i = 0; i < cells.size(); ++i) {
    cells[i] = d(m);
  }
  return std::move(cells);
}

void print(const std::vector<int>& cells) {
  for(size_t i = 0; i < cells.size(); ++i) {
    std::cout << (cells[i] == 0 ? " " : "\u25A0");
  }
  std::cout << std::endl;
}

void simulate_step(const std::vector<int>& current, std::vector<int>& next) {
  #pragma omp parallel for
  for(size_t i = 1; i < current.size()-1; ++i) {
    if(current[i] == 0) {
      next[i] = current[i-1];
    }
    else {
      next[i] = current[i+1];
    }
  }
}

struct chunk_data {
  size_t left;
  size_t right;
  size_t internal;
  size_t chunk_size;
  chunk_data(): left(0), right(0), internal(0), chunk_size(0){}
};

chunk_data process_chunk(size_t b, size_t e, const std::vector<int>& data) {
  size_t kmax = 0;
  size_t k = 0;
  chunk_data ld;
  ld.chunk_size = e - b;
  size_t i = b;
  for(;i < e && data[i] == 1; ++i) {
    ld.left++;
  }
  for(; i < e; ++i) {
    if(data[i] == 1) {
      ++k;
      ld.internal = std::max(ld.internal, k);
    }
    else {
      k = 0;
    }
  }
  ld.right = k;
  return ld;
}

int longest_queue(const std::vector<int>& data) {
  size_t num_threads = omp_get_max_threads();
  std::vector<chunk_data> results(num_threads);
  #pragma omp parallel num_threads(num_threads)
  {
    int tid = omp_get_thread_num(); // Get thread ID
    size_t chunk_size = data.size() / num_threads; // Define the chunk size
    size_t b = tid * chunk_size; // Start index for this thread
    size_t e = (tid == num_threads - 1) ? data.size() : b + chunk_size; // End index (handle remainder)

    // Each thread calls process_chunk on its assigned portion of the array
    results[tid] = process_chunk(b, e, data);
  }
  size_t max = std::max(results[0].left, results[0].internal);
  for(int i = 1; i < results.size(); ++i) {
    max = std::max(max, results[i-1].right + results[i].left);
    if(results[i].left == results[i].chunk_size) {
      results[i].right = results[i-1].right + results[i].left;
    }
    max = std::max(max, results[i].internal);
  }
  return max;
}

size_t simulate(size_t steps, std::vector<int>& current, std::vector<int>& next) {
  using namespace std::chrono_literals;
  std::mt19937 m{0}; // fixed seed to ease debugging.
  // std::uniform_int_distribution<int> d{0, 1};
  std::discrete_distribution<> d({10, 90});
  int lmax = 0;
  for(int i = 0; i < steps; ++i) {
    CLOCK_PRINT(i, 1, "longest", lmax = std::max(lmax, longest_queue(current)))
    CLOCK_PRINT(i, 2, "simulate", simulate_step(current, next))
    std::swap(current, next);
    current[0] = d(m); // Next car, random.
  }
  return lmax;
}

int main(int argc, char** argv) {
  if(argc < 3) {
    std::cout << "usage: " << argv[0] << " <size> <steps> [threads]\n";
    exit(1);
  }
  size_t n = std::atoll(argv[1]);
  size_t steps = std::atoll(argv[2]);
  size_t num_threads = argc == 4 ? std::atoll(argv[3]) : omp_get_max_threads();
  std::cout << "num_threads = " << num_threads << std::endl;
  omp_set_num_threads(num_threads);
  std::vector<int> cells = initialize_cells(n);
  std::vector<int> next(n);
  auto start = std::chrono::steady_clock::now();
  size_t lmax = simulate(steps, cells, next);
  auto end = std::chrono::steady_clock::now();
  std::cout << "longest_queue = " << lmax << std::endl;
  std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
  return 0;
}
