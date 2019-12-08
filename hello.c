#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char **argv) {

    int result,numtask,rank,i;
    MPI_Status Stat;
    FILE * arq;


    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        printf("hello parallel\n");
    MPI_Finalize();
}