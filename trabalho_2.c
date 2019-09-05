#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int main(int argc, char *argv[]) {

    int numtask,rank,i,j;
    int result;
    MPI_Status Stat;
    FILE * arq;


    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        if( rank>0 ) {
            MPI_Recv(&result,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            for(j=0;j<100;j++) {
                MPI_Send(&result,1,MPI_INT,0,1,MPI_COMM_WORLD);
            }
        } else {
            result = atoi(argv[1]);
            printf("arg num %d\n",result);
            if ((arq = fopen(argv[2], "w")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo");
                exit(1);
            }
            for(i=1;i<numtask;i++) {
                MPI_Send(&result,1,MPI_INT,i,1,MPI_COMM_WORLD);
            } 
            
            for(i=0;i<(numtask-1)*100;i++) {
                MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE,1, MPI_COMM_WORLD, &Stat);
                fprintf(arq,"identificação = %d  mensagem %d\n",Stat.MPI_SOURCE,result);
            }
        }
    MPI_Finalize();
    return 0;
}
