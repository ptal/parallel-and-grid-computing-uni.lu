#include <random>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

int main() {
  int num_procs = omp_get_num_procs();
  std::cout << "Number of available processors: " << num_procs << std::endl;
  using namespace std::chrono_literals;
  #pragma omp parallel num_threads(4)
  {
    #pragma omp single nowait
    {
      printf ("%d: creating two tasks!\n", omp_get_thread_num());
      #pragma omp task
      {
        #pragma omp critical
        printf ("%d: first task!\n", omp_get_thread_num());
      }
      // #pragma omp taskwait
      #pragma omp task
      {
        #pragma omp critical
        printf ("%d: second task!\n", omp_get_thread_num());
      }
    }
    printf ("%d: exit!\n", omp_get_thread_num());
  }

  // Dynamic creation of tasks with a loop.
  #pragma omp parallel num_threads(4)
  {
    #pragma omp single nowait
    {
      printf ("%d: creating 10 tasks!\n", omp_get_thread_num());
      for(int i = 0; i < 10; ++i) {
        #pragma omp task
        {
          #pragma omp critical
          printf ("%d: task %d!\n", omp_get_thread_num(), i);
          std::this_thread::sleep_for(1000ms);
        }
      }
    }
    printf ("%d: exit!\n", omp_get_thread_num());
  }
  return 0;
}
