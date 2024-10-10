#include <vector>
#include <iostream>
#include <thread>
#include <span>
#include <random>
#include <algorithm>

using namespace std::chrono_literals;


std::vector<int> initialize_cells(size_t n)
{
  std::vector<int> cells(n);
  std::mt19937 m{0}; // fix the seed to ease debugging.
  std::uniform_int_distribution<int> u{0, 1};
  for (int i = 0; i < cells.size(); ++i)
  {
    cells[i] = u(m);
  }
  return std::move(cells);
}

void printOutput(std::vector<int> output){

  for(int ele : output){
    std::cout<<ele;
  }
    std::cout<<std::endl;
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

void simulate_steps(std::vector<int> &current, std::vector<int> &next)
{
    int arraySize = current.size();
  for (int ele = 0; ele < arraySize; ++ele)
  {
    int curr = current[ele];
    int right = current[(ele + 1)%arraySize];
    if (curr == 1 && right == 0){
      next[ele] = 0;
      next[(ele + 1)% arraySize] = 1;
    }
    else if (next[ele] != 1){
      next[ele] = curr;
    }
  }

  current = next;

  return;
}

void parallel_simulate_steps(std::vector<int> &current, std::vector<int> &next)
{
  size_t mid = current.size() / 2;
  std::span half1(current.begin(), mid);
  std::span half2(current.begin() + mid, current.size() - mid);
  std::thread a([&](){ simulate_steps(current, next); });
  std::thread b([&](){ simulate_steps(current, next); });
  a.join();
  b.join();
  return;
}

int simulate(std::vector<int> current, std::vector<int> &next, bool isMultiThreaded)
{
    int iterations = 0;
    auto now = std::chrono::steady_clock::now;
    auto timeLimit = 60s;
    if(isMultiThreaded){
        auto start = now();
        while(now()-start<timeLimit){
            iterations++;
            parallel_simulate_steps(current, next);
        }
        printOutput(current);
    }
    else{
        auto start = now();
        while(now()-start<timeLimit){
            iterations++;
            simulate_steps(current, next);
        }
        printOutput(current);
    }

   
  return iterations;
}

int main(int argc, char **argv)
{
  if(argc != 4) {
    std::cout << "usage: " << argv[0] << " <vector size> <num_threads>" << std::endl;
    exit(1);
  }  
  size_t array_size = std::stoi(argv[1]);
  size_t num_threads = std::stoi(argv[2]);
//   int upper_limit = 
  
  std::vector<int> single_current = initialize_cells(array_size);
  std::vector<int> multi_current = single_current;
  std::vector<int> single_op(array_size);
  std::vector<int> multi_op = single_op;


  std::mt19937 m{0};
  std::uniform_int_distribution<int> u{0, 1};
  int single_iterations =  simulate(single_current,single_op, false);
  int multithreaded_iterations = simulate(multi_current, multi_op, true);

  std::cout<<"Iterations in single thread in 1 min: "<<single_iterations<<std::endl;
  std::cout<<"Iterations in "<<num_threads<<" threads in 1 min: "<<multithreaded_iterations<<std::endl;

  check_equal_vector(single_op, multi_op);

  return 0;
}
