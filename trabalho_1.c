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
        if(rank>0 && rank<numtask-1) {
            MPI_Recv(&result,1,MPI_INT, rank-1,1,MPI_COMM_WORLD,&Stat); 
            result=result+rank;
            MPI_Send(&result,1,MPI_INT,rank+1,1,MPI_COMM_WORLD);
            MPI_Send(&result,1,MPI_INT,0,1,MPI_COMM_WORLD);
        } else if(rank == 0) {
            result = atoi(argv[1]);
            //fprintf(arq, "numero inicial %d",result);
            if ((arq = fopen(argv[2], "w")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo");
                exit(1);
            }
            MPI_Send(&result,1,MPI_INT,rank+1,1,MPI_COMM_WORLD);
            
            for(i=0;i<numtask-1;i++) {
                MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE,1, MPI_COMM_WORLD, &Stat);
                fprintf(arq,"recebido de %d, resultado = %d\n",Stat.MPI_SOURCE,result);
            }
            
        } else {
            MPI_Recv(&result,1,MPI_INT, rank-1,1,MPI_COMM_WORLD,&Stat);
            result=result+rank;
            MPI_Send(&result,1,MPI_INT,0,1,MPI_COMM_WORLD);
        }
    MPI_Finalize();
    return 0;
}
