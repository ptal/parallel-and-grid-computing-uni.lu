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

    if(num_procs != 3) {
        printf("This program must be ran with 3 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }

    /** TODO: Stop when either process 1 or 2 wakes up. */

    /* Tear down the communication infrastructure */
    MPI_Finalize();
    return 0;
}