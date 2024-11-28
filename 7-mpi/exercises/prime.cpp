#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank, num_procs;

  /* Initialize the infrastructure necessary for communication */
  MPI_Init(&argc, &argv);

  if(argc < 2) {
    printf("usage: %s <upper_bound_prime>\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int n = std::stoi(argv[1]);

  /* Identify this process */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Find out how many total processes are active */
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  /** This is a sequential loop: need to be parallelized. */
  int totalPrimes = 0;
  bool prime;
  for(int i = 2; i <= n; i++) {
    prime = true;
    for(int j = 2; j < i; j++) {
      if((i % j) == 0){
        prime = false;
        break;
      }
    }
    totalPrimes += prime;
  }

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}