#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int contaVizinhos(long y, long x, int r){
    int soma = 0;
    for(long vy=y-1; vy<=y+1; vy++){
        for(long vx=x-1; vx<=x+1; vx++){
            soma += tabuleiro[vy][vx];
        }
    }
    (tabuleiro[y][x] == 1 ) ? soma-- : soma;
    return soma;
}

void jogo(char ** novoTab, char ** tab, int tam, int nunT){
    int vizinhos = 0;
    long l, c;
    for(l=1; l<=tam; l++){
        for(c=1; c<=tam/nunT; c++){
            vizinhos = contaVizinhos(l,c);
            if(vizinhos < 2) {
                novoTab[l*tam + c] = 0;
            } 
            else if(vizinhos == 3){
                novoTab[l*tam + c] = 1;
            }
            else if (vizinhos > 3){
                novoTab[l*tam + c] = 0;
            }
            else {
                novoTab[l*tam + c] = tab[l*tam + c];
            } 
        }
    }
}

void printTabuleiro(char ** tab, int tam){
    for(int i=0; i<tam; i++){
        printf("-");
    }
    printf("\n"); 
    for(int i=1; i<=tam; i++){
        for(int j=1; j<=tam; j++){
            printf("%c", tab[i*tam + j] == 1 ? 'x' : ' ');
        }
        printf("\n");
    }
}

int main(int argc, char * argv[]) {

    int numtask,rank,i;
    MPI_Status Stat;
    FILE * pfTabuleiro;

    char * tabuleiro;
    char * novoTabuleiro;
    char * antigoTabuleiro;
    long tamanho, passos;

    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        if (rank == 0 ) {
            char * vet;
            char scan;
            char lineBreack;
            pfTabuleiro = fopen(argv[1], "r");
            fscanf(pfTabuleiro, "%ld %ld%c", &tamanho, &passos, &lineBreack);
            long size = ((tamanho+2)*(tamanho+2))/numtask);
            tabuleiro = (char **) malloc(size*sizeof(char *));
            novoTabuleiro = (char **) malloc(size*sizeof(char *));
            antigoTabuleiro = (char **) malloc(size*sizeof(char *));
            for(int i=1; i<=tamanho; i++){
                for(int j=1; j<=tamanho; j++){
                    fscanf(pfTabuleiro, "%c", &scan);
                    //printf("[%d][%d] = %d\n", i, j, (scan == 'x'));
                    tabuleiro[i*tamanho + j] = (scan == 'x');
                }
            }
            vet = tabuleiro;
            for(int i = 1; i<numtask; i++){
                MPI_Send(&tamanho,1,MPI_INT,i,1,MPI_COMM_WORLD);
                MPI_Send(&passos,1,MPI_INT,i,1,MPI_COMM_WORLD);
                MPI_Send(vet,size,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
                vet = (vet + tamanho + 2);
            }
        }
        else {
            MPI_Recv(&tamanho,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            MPI_Recv(&passos,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            long size = ((tamanho+2)*(tamanho+2))/numtask);
            tabuleiro = (char **) malloc( size*sizeof(char *));
            novoTabuleiro = (char **) malloc(size*sizeof(char *));
            antigoTabuleiro = (char **) malloc(size*sizeof(char *));
        }
    MPI_Finalize();
}