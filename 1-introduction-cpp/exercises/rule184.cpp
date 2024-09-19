#include <iostream>
#include <random>
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

int main(int argc, char** argv) {
  using namespace std::chrono_literals;
  std::mt19937 m{0};
  std::uniform_int_distribution<int> u{0, 1};
  size_t n = 50;
  size_t steps = 10;
  std::vector<int> cells = initialize_cells(n);

  /** Notes:
   * `std::this_thread::sleep_for(1000ms);` sleeps the program for 1 second.
   * `u(m)` randomly generates either 0 or 1 following a uniform distribution.
   * You can use `u(m)` to generate whether a car enters the queue (cells[0]) at the current step.
  */
  return 0;
}
