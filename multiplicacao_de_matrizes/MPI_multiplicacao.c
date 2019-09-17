#include <stdio.h>
#include <stdlib.h>
#include "matrizes.h"
#include <mpi.h>

long long int* pscaoRank; //diz em qual i*j o rank esta calculando

int main(int argc, char **argv) {

    int numtask,rank;
    long long int i,j,t,linhas,colunas;
    double *Ap,*Bp,result; 
    MPI_Status Stat;
    FILE *arqMatA, *arqMatB, *arqSaida;


    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        
        if(rank == 0) {
            struct Matriz A,B,Mat;
            if ((arqMatA = fopen(argv[1], "r")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo");
                exit(1);
            }
            if ((arqMatB = fopen(argv[2], "r")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo");
                exit(1);
            }
            if ((arqSaida = fopen(argv[3], "w")) == (FILE *) NULL) {
                printf("erro ao abrir arquivo");
                exit(1);
            }
            constroiMatrizArquivo(&A,arqMatA);
            constroiMatrizArquivo(&B,arqMatB); 
               
            if(A.col != B.row) {
                printf("Nao e possivel fazer a multiplicacao, numero de colunas da matriz A diferente da matriz B\n");
                exit(1);
            }
            transposta(&B);
            colunas = A.col;
            linhas = A.row;
            pscaoRank = malloc(numtask*(sizeof(long long int)));
            //salvaMatriz(&B,arqSaida);
            for(t=1;t<numtask;t++) {
                MPI_Send(&colunas,1,MPI_LONG_LONG_INT,t,1,MPI_COMM_WORLD);
                MPI_Send(&linhas,1,MPI_LONG_LONG_INT,t,1,MPI_COMM_WORLD);
            }

            Ap = A.M;
            Bp = B.M;
            t=0;//quantas vezes enviou

            constroiMatriz(&Mat,linhas);
            for(i=0;i<linhas;i++) { //linhas de A
                for(j=0;j<linhas;j++) { //colunas de B OBS: FIZ A TRANSPOSTA DE B
                    MPI_Send(Ap,colunas,MPI_DOUBLE,(t%(numtask-1))+1,1,MPI_COMM_WORLD);
                    MPI_Send(Bp,colunas,MPI_DOUBLE,(t%(numtask-1))+1,1,MPI_COMM_WORLD);
                    
                    pscaoRank[(t%(numtask-1))+1] = i*linhas + j;
                    //MPI_Recv(&result,1,MPI_DOUBLE, t%(numtask-1)+1,1,MPI_COMM_WORLD,&Stat);
                    MPI_Recv(&result,1,MPI_DOUBLE,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&Stat);
                    //printf("result: %lf source: %d\n", result, Stat.MPI_SOURCE);
                    Mat.M[pscaoRank[Stat.MPI_SOURCE]] = result;
                    //result=0;
                    t++;
                    Bp = (Bp+colunas);
                }
                //printf("rank: 0 Ap[0]: %lf\n",Ap[0]);
                Ap = (Ap+(colunas));
                Bp = B.M;
            }

            salvaMatriz(&Mat,arqSaida);
        }

        else {
            int numOp; //numero operacoes que vao ser feitas pelo rank
            MPI_Recv(&colunas,1,MPI_LONG_LONG_INT,0,1,MPI_COMM_WORLD,&Stat);
            MPI_Recv(&linhas,1,MPI_LONG_LONG_INT,0,1,MPI_COMM_WORLD,&Stat);
            
            numOp = (linhas*linhas)/(numtask-1) + ( rank <= (linhas*linhas)%(numtask-1));
            //printf("rank %d, numop: %d\n", rank, numOp);
            Ap = malloc(colunas*(sizeof(double)));
            Bp = malloc(colunas*(sizeof(double)));
            
            
            for(i=0;i<numOp;i++) {
                MPI_Recv(Ap,colunas,MPI_DOUBLE, 0,1,MPI_COMM_WORLD,&Stat);
                MPI_Recv(Bp,colunas,MPI_DOUBLE, 0,1,MPI_COMM_WORLD,&Stat);
                //printf("rank: %d Ap[0]: %lf Bp[0]: %lf\n",rank,Ap[0],Bp[0]);
                result = 0;
                for(j=0; j<colunas; j++) {
                    result += Ap[j] * Bp[j];
                } 
                
                MPI_Send(&result,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
            }           
        }
    MPI_Finalize();
    return 0;
}