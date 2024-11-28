#include <cassert>
#include <cstdio>
#include <string>
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

    if(num_procs % 2 != 0) {
        printf("This program must be ran with an even number of processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    /** TODO: processes exchange "ping" / "pong" messages `num_ping_pong` times. */

    /* Tear down the communication infrastructure */
    MPI_Finalize();
    return 0;
}