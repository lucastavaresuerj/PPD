
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int main(int argc, char **argv) {

    int size,numtask,rank,i,j, numTeste;
    double tempInicio, tempFim;
    MPI_Status Stat;
    FILE * arq;
    double* vet;
    
    //printf("hey");
    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        if(rank==0) {
            if ((arq = fopen(argv[3], "w")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo\n");
                exit(1);
            }

            size = atoi(argv[1]);
            numTeste = atoi(argv[2]);
            MPI_Send(&numTeste,1,MPI_INT,1,1,MPI_COMM_WORLD);
            MPI_Send(&size,1,MPI_INT,1,1,MPI_COMM_WORLD);
            vet = malloc(size*sizeof(double));
            for(i=0;i<size;i++) {
                vet[i] = -1.0;
            }

            tempInicio=MPI_Wtime();
            for(j=0;j<numTeste;j++) {
                for(i=0;i<size;i++) {
                    vet[i] = 2.0;
                }
                MPI_Send(vet,size,MPI_DOUBLE,1,1,MPI_COMM_WORLD);
                MPI_Recv(vet,size,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&Stat); 
            }
            tempFim=MPI_Wtime();
            fprintf(arq,"tempo passado: %f s\n",tempFim-tempInicio);
            
        } else {
            MPI_Recv(&numTeste,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            MPI_Recv(&size,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            vet = malloc(size*sizeof(double));
            for(j=0;j<numTeste;j++){
                MPI_Recv(vet,size,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&Stat); 
                for(i=0;i<size;i++) {
                    vet[i]=4.0;
                }
                MPI_Send(vet,size,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
            }
        }
    MPI_Finalize();
    return 0;
}
