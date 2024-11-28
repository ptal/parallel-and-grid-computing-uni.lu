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

  /* Identify this process */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Find out how many total processes are active */
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  std::string hello("hello");
  std::string world("world");

  if(rank == 0) {
    MPI_Send(hello.data(), hello.size(), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    MPI_Send(world.data(), world.size(), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
  }
  else if(rank == 1) {
    MPI_Recv(hello.data(), hello.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(world.data(), world.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "We should receive hello world, and we received: " << hello << " " << world << std::endl;
  }

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}