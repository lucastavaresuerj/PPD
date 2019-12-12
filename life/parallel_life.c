#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int contaVizinhos(long y, long x, char * tab, int tam){
    int soma = 0;
    for(long vy=y-1; vy<=y+1; vy++){
        for(long vx=x-1; vx<=x+1; vx++){
            soma += tab[vy*tam + vx];
        }
    }
    (tab[y*tam + x] == 1 ) ? soma-- : soma;
    return soma;
}

void jogo(char * novoTab, char * tab, int tam, int nunL){
    int vizinhos = 0;
    long l, c;
    for(l=1; l<=nunL; l++){
        for(c=1; c<=tam; c++){
            vizinhos = contaVizinhos(l,c, tab, tam);
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

void printTabuleiro(char * tab, int tam, int numL){
    for(int i=0; i<tam; i++){
        printf("-");
    }
    printf("\n"); 
    for(int i=1; i<=numL; i++){
        for(int j=1; j<=tam; j++){
            printf("%c", tab[i*tam + j] == 1 ? 'x' : ' ');
        }
        printf("\n");
    }
}

void printVector(char * vet, int tam, int numL) {
    for(int i = 0; i< numL; i++){
        for(int j= 0; j<tam; j++){
            printf("%c", vet[i*tam + j] == 1 ? 'x' : ' ');
        }
        printf("\n");
    }
}

void adicionaFronteira(char * tab, char * front, int tam, int pos) {
    for(int i = 0; i<tam; i++){ // toda a linha
        tab[pos*tam + i] = front[i];
    }
}

int main(int argc, char * argv[]) {

    int numtask,rank,i;
    MPI_Status Stat;
    FILE * pfTabuleiro;

    char * tabuleiro;
    char * novoTabuleiro;
    char * antigoTabuleiro;
    int tamanho, passos;

    MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtask);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        if (rank == 0 ) {
            char * vetorASerEnviado;
            char * vetorASerRecebido;
            char scan;
            char lineBreack;
            pfTabuleiro = fopen(argv[1], "r");
            fscanf(pfTabuleiro, "%d %d%c", &tamanho, &passos, &lineBreack);
            int size = ((tamanho+2) * (tamanho+2));
            tabuleiro = (char *) malloc(size*sizeof(char ));
            novoTabuleiro = (char *) calloc(size,sizeof(char ));
            antigoTabuleiro = (char *) calloc(size,sizeof(char ));
            vetorASerEnviado = (char *) calloc(size,sizeof(char ));//TODO: no final, colocar size/numtask
            vetorASerRecebido = (char *) calloc((tamanho+2),sizeof(char));
            tabuleiro[9] = 1;
            printf("tamanho real tabuleiro: %ld\n", (sizeof(tabuleiro)/sizeof(char) ));
            for(int i=1; i<=tamanho; i++){
                for(int j=1; j<=tamanho; j++){
                    fscanf(pfTabuleiro, "%c", &scan);
                    //printf("[%d][%d] = %d\n", i, j, (scan == 'x'));
                    tabuleiro[i*(tamanho+2) + j] = (scan == 'x');
                    printf("[%d][%d]%d ", i,j,tabuleiro[i*(tamanho+2) + j]);
                }
                printf("\n");
            }
            printf("[0][0] = %d\n", tabuleiro[2*(tamanho+2) + 3]);
            printf("tamanho: %d\n", tamanho);
            //printVector(tabuleiro, tamanho+2, tamanho+2);
            int numeroDeLinhasASerEnviadas = 0; 
            int tamanhoDoBlocoASerEnvidado = 0;
            vetorASerEnviado = (tabuleiro + 2+ tamanho);
            for(int i = 1; i<numtask; i++){
                numeroDeLinhasASerEnviadas = (tamanho)/numtask + (i < tamanho%numtask); //correto
                tamanhoDoBlocoASerEnvidado = (numeroDeLinhasASerEnviadas)*(2+tamanho);
                vetorASerEnviado = (vetorASerEnviado + tamanhoDoBlocoASerEnvidado );
                //printVector(vetorASerEnviado, tamanho, numeroDeLinhasASerEnviadas);
                //printf("rank %d, tamanho: %d\n", i, tamanhoDoBlocoASerEnvidado);
                MPI_Send(&tamanho,1,MPI_INT,i,1,MPI_COMM_WORLD);
                MPI_Send(&passos,1,MPI_INT,i,1,MPI_COMM_WORLD);
                MPI_Send(vetorASerEnviado,tamanhoDoBlocoASerEnvidado,MPI_CHAR,i,1,MPI_COMM_WORLD);
            }
            int numLinhas = (tamanho)/numtask + (0 < tamanho%numtask);
/*
            jogo(tabuleiro, novoTabuleiro, tamanho, numtask);
            antigoTabuleiro = tabuleiro;
            tabuleiro = novoTabuleiro;
            novoTabuleiro = antigoTabuleiro;
            MPI_Send(novoTabuleiro,(tamanho+2),MPI_CHAR,1,1,MPI_COMM_WORLD);

            for(int p = 1; p<passos; p++){
                MPI_Recv(vetorASerRecebido,(tamanho+2),MPI_CHAR,1,1,MPI_COMM_WORLD,&Stat);
                adicionaFronteira(tabuleiro, vetorASerRecebido, tamanho, numLinhas);

                jogo(tabuleiro, novoTabuleiro, tamanho, numLinhas);
                antigoTabuleiro = tabuleiro;
                tabuleiro = novoTabuleiro;
                novoTabuleiro = antigoTabuleiro;

                MPI_Send(novoTabuleiro,(tamanho+2),MPI_CHAR,1,1,MPI_COMM_WORLD);
            }
*/
        }
        else {
            char * vetorASerRecebido;
            MPI_Recv(&tamanho,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            MPI_Recv(&passos,1,MPI_INT,0,1,MPI_COMM_WORLD,&Stat);
            int numLinhas = (tamanho)/numtask + (rank < tamanho%numtask) + 2;
            long size = (tamanho+2)*numLinhas;
            tabuleiro = (char *) malloc( size*sizeof(char ));
            novoTabuleiro = (char *) malloc(size*sizeof(char ));
            antigoTabuleiro = (char *) malloc(size*sizeof(char ));
            vetorASerRecebido = (char *) malloc((tamanho+2)*sizeof(char ));

            MPI_Recv(tabuleiro + (tamanho + 2),size,MPI_CHAR,0,1,MPI_COMM_WORLD,&Stat);
            //if(rank == 1)
               // printVector(tabuleiro, tamanho, numLinhas);
/*
            jogo(tabuleiro, novoTabuleiro, tamanho, numtask);
            antigoTabuleiro = tabuleiro;
            tabuleiro = novoTabuleiro;
            novoTabuleiro = antigoTabuleiro;
            MPI_Send(novoTabuleiro,(tamanho+2),MPI_CHAR,rank+1,1,MPI_COMM_WORLD);
            if(rank < numtask - 1) {
                MPI_Send((novoTabuleiro + (2+tamanho)*(numLinhas-1)),(tamanho+2),MPI_CHAR,rank+1,1,MPI_COMM_WORLD);
            }
            
            for(int p = 1; p<passos; p++){
                MPI_Recv(vetorASerRecebido,(tamanho+2),MPI_CHAR,rank-1,1,MPI_COMM_WORLD,&Stat);
                adicionaFronteira(tabuleiro, vetorASerRecebido, tamanho, 0);
                if(rank < numtask - 1) {
                    MPI_Recv(vetorASerRecebido,(tamanho+2),MPI_CHAR,rank+1,1,MPI_COMM_WORLD,&Stat);
                    adicionaFronteira(tabuleiro, vetorASerRecebido, tamanho, numLinhas);
                }

                jogo(tabuleiro, novoTabuleiro, tamanho, numLinhas);
                antigoTabuleiro = tabuleiro;
                tabuleiro = novoTabuleiro;
                novoTabuleiro = antigoTabuleiro;

                MPI_Send(novoTabuleiro,(tamanho+2),MPI_CHAR,rank+1,1,MPI_COMM_WORLD);
                if(rank < numtask - 1) {
                    MPI_Send((novoTabuleiro + (2+tamanho)*(numLinhas-1)),(tamanho+2),MPI_CHAR,rank+1,1,MPI_COMM_WORLD);
                }
            }
*/
        }
    MPI_Finalize();
}