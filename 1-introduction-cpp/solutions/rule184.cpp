#include <iostream>
#include <cstdio>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>

std::vector<int> initialize_cells(size_t n) {
  std::vector<int> cells(n);
  // std::mt19937 m{std::random_device{}()};
  std::mt19937 m{0}; // fix the seed to ease debugging.
  std::uniform_int_distribution<int> u{0, 1};
  for(int i = 0; i < cells.size(); ++i) {
    cells[i] = u(m);
  }
  return std::move(cells);
}

void print(const std::vector<int>& cells) {
  for(int i = 0; i < cells.size(); ++i) {
    std::cout << (cells[i] == 0 ? " " : "\u25A0");
  }
  std::cout << std::endl;
}

void simulate_step(const std::vector<int>& current, std::vector<int>& next) {
  for(int i = 1; i < current.size()-1; ++i) {
    if(current[i] == 0) {
      next[i] = current[i-1];
    }
    else {
      next[i] = current[i+1];
    }
  }
}

int longest_queue(const std::vector<int>& cells) {
  int kmax = 0;
  int k = 0;
  for(int i = 0; i < cells.size(); ++i) {
    if(cells[i] == 1) {
      ++k;
      kmax = std::max(kmax, k);
    }
    else {
      k = 0;
    }
  }
  return kmax;
}

void simulate(size_t steps, std::vector<int>& current, std::vector<int>& next) {
  using namespace std::chrono_literals;
  std::mt19937 m{0}; // fixed seed to ease debugging.
  // std::uniform_int_distribution<int> d{0, 1};
  std::discrete_distribution<> d({10, 90});
  int lmax = 0;
  for(int i = 0; i < steps; ++i) {
    simulate_step(current, next);
    std::swap(current, next);
    lmax = std::max(lmax, longest_queue(current));
    print(current);
    current[0] = d(m); // Next car, random.
    std::this_thread::sleep_for(1000ms);
  }
  std::cout << "Longest queue: " << lmax << std::endl;
}

int main(int argc, char** argv) {
  size_t n = 50;
  size_t steps = 50;
  std::vector<int> cells = initialize_cells(n);
  std::vector<int> next(n);
  simulate(steps, cells, next);
  return 0;
}
