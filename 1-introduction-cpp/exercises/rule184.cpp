#include <iostream>
#include <random>
#include <vector>
#include <thread>

std::vector<int> initialize_cells(size_t n)
{
  std::vector<int> cells(n);
  // std::mt19937 m{std::random_device{}()};
  std::mt19937 m{0}; // fix the seed to ease debugging.
  std::uniform_int_distribution<int> u{0, 1};
  for (int i = 0; i < cells.size(); ++i)
  {
    cells[i] = u(m);
  }
  return std::move(cells);
}

void printOutput(std::vector<int> output){
  // for(int ele : output){
  //   if(ele) std::cout<<"#";
  //   else std::cout<<"_";
  // }
  for(int ele : output){
    std::cout<<ele;
  }
    std::cout<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(325));
}

void simulate_steps(std::vector<int> &current, std::vector<int> &next)
{
  for (int ele = 0; ele < current.size(); ++ele)
  {
    int curr = current[ele];
    int right = current[ele + 1];
    if (curr == 1 && right == 0){
      next[ele] = 0;
      next[(ele + 1)] = 1;
    }
    else if (next[ele] != 1){
      next[ele] = curr;
    }
  }

  current = next;

  return;
}

void simulate(size_t steps, std::vector<int> current, std::vector<int> next)
{
  for (int i = 0; i < steps; i++)
  {
    simulate_steps(current, next);
    printOutput(current);
  }
  return;
}

int main(int argc, char **argv)
{
  using namespace std::chrono_literals;
  std::mt19937 m{0};
  std::uniform_int_distribution<int> u{0, 1};
  size_t n = 50;
  size_t steps = 30;
  std::vector<int> current = initialize_cells(n);
  std::vector<int> next(n);
  simulate(steps, current, next);

  /** Notes:
   * `std::this_thread::sleep_for(1000ms);` sleeps the program for 1 second.
   * `u(m)` randomly generates either 0 or 1 following a uniform distribution.
   * You can use `u(m)` to generate whether a car enters the queue (cells[0]) at the current step.
   */
  return 0;
}
