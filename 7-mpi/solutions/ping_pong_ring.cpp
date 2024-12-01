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
    printf("usage: %s <num-ping-pong>\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int num_ping_pong = std::stoi(argv[1]);

  /* Identify this process */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Find out how many total processes are active */
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  MPI_Request rq_send;

  std::string ping = "ping";
  std::string pong = "pong";
  std::string rping(4, ' ');
  std::string rpong(4, ' ');
  int next_rank = (rank + 1) % num_procs;
  int prev_rank = ((rank + num_procs) - 1) % num_procs;
  for(int i = 0; i < num_ping_pong; ++i) {
    /** Send "ping" to the next process in the ring. */
    MPI_Isend(ping.data(), ping.size(), MPI_CHAR, next_rank, 0, MPI_COMM_WORLD, &rq_send);
    /** Receive "ping" from the previous process in the ring. */
    MPI_Recv(rping.data(), rping.size(), MPI_CHAR, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << rank << " received " << rping << std::endl;
    MPI_Wait(&rq_send, MPI_STATUS_IGNORE);

    /** Send "pong" to the previous process in the ring. */
    MPI_Isend(pong.data(), pong.size(), MPI_CHAR, prev_rank, 0, MPI_COMM_WORLD, &rq_send);
    /** Receive "pong" from the next process in the ring. */
    MPI_Recv(rpong.data(), rpong.size(), MPI_CHAR, next_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << rank << " received " << rpong << std::endl;
    MPI_Wait(&rq_send, MPI_STATUS_IGNORE);
  }

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}