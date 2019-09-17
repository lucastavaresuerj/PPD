#include <stdio.h>
#include <stdlib.h>

struct Matriz {
    double* M;
    int row;
    int col;
};

void constroiMatrizArquivo (struct Matriz* Matriz, FILE* fp) {
    int linhas = 0, colunas = 0,index;
    char scan;
    float d;
    while ( !feof(fp) ) { //enquanto nao for o final do arquivo
        fscanf(fp, "%c",&scan);
        if(scan == '\n') {
            linhas++;
            colunas++;
        } else if (scan == ' ') {
            colunas++;
        } 
    } 
    rewind(fp);
    linhas--;
    colunas--;
    colunas = colunas/linhas;

    (*Matriz).row = linhas;
    (*Matriz).col = colunas;
    (*Matriz).M = (double*) malloc(linhas*colunas*(sizeof(double)));
    index = 0;
    while(linhas*colunas - index) {
        fscanf(fp,"%f", &d);
        printf("%f",d);
        index++;
    }
}

void transposta(struct Matriz* Matriz) {
    double* mat;
    int i,j;
    int c = (*Matriz).col;
    int l = (*Matriz).row;
    mat = (double*) malloc( c*l*(sizeof(double)));
    for(i=0;i<l;i++) {
        for(j=0;j<c;j++) {
            mat[j*l + i] = (*Matriz).M[i*c + j]; 
        }
    }
    (*Matriz).M = mat;
    (*Matriz).col = l;
    (*Matriz).row = c;
}

void salvaMatriz(struct Matriz* Matriz, FILE* fp) {
    int i,j;
    for(i=0; i<(*Matriz).row; i++ ){
        for(j=0; j<(*Matriz).col; j++) {
            fprintf(fp,"%lf ",(*Matriz).M[i*(*Matriz).col + j]);
        }
        fprintf(fp,"\n");
    }
}

int main (int argc, char **argv) {
    FILE *arqFonte,*arqSaida;
    int size;
    struct Matriz mat;

    if ((arqFonte = fopen(argv[1], "r")) == (FILE *) NULL) {
        printf("erro ao abrir arquivo");
        exit(1);
    }
    if ((arqSaida = fopen(argv[2], "w")) == (FILE *) NULL) {
        printf("erro ao abrir arquivo");
        exit(1);
    }
    constroiMatrizArquivo(&mat,arqFonte);
    transposta(&mat);
    printf("%lf\n", mat.M[3]);
    salvaMatriz(&mat,arqSaida);
}