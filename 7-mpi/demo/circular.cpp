#include <cassert>
#include <cstdio>
#include <string>
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

  std::string ping("ping");
  std::string rdata(4, ' ');
  int next_rank = (rank + 1) % num_procs;
  int prev_rank = ((rank + num_procs) - 1) % num_procs;
  printf("%d: send ping to %d...\n", rank, next_rank);
  MPI_Ssend(ping.data(), ping.size(), MPI_CHAR, next_rank, 0, MPI_COMM_WORLD);
  printf("%d: receive ping from %d...\n", rank, prev_rank);
  MPI_Recv(rdata.data(), rdata.size(), MPI_CHAR, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}