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
  int tag1 = 1;
  int tag2 = 2;
  if(rank == 0) {
    size_t buf_size = hello.size() * sizeof(char) + MPI_BSEND_OVERHEAD;
    char* b = (char*)malloc(buf_size);
    MPI_Buffer_attach(b, buf_size);
    MPI_Bsend(hello.data(), hello.size(), MPI_CHAR, 1, tag1, MPI_COMM_WORLD);
    MPI_Ssend(world.data(), world.size(), MPI_CHAR, 1, tag2, MPI_COMM_WORLD);
  }
  else if(rank == 1) {
    MPI_Recv(world.data(), world.size(), MPI_CHAR, 0, tag2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(hello.data(), hello.size(), MPI_CHAR, 0, tag1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "We should receive hello world, and we received: " << hello << " " << world << std::endl;
  }

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}