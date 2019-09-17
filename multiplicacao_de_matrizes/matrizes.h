#include <stdio.h>
#include <stdlib.h>

struct Matriz {
    double* M;
    long long int row;
    long long int col;
};

void constroiMatrizArquivo (struct Matriz* Matriz, FILE* fp) {
    long long int linhas = 0, colunas = 0,index;
    char scan;
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
        fscanf(fp,"%lf", &(*Matriz).M[index]);
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

void constroiMatriz(struct Matriz* Matriz, int linhas) {
    (*Matriz).row = linhas;
    (*Matriz).col = linhas;
    (*Matriz).M = (double*) malloc(linhas*linhas*(sizeof(double)));
}

void salvaMatriz(struct Matriz* Matriz, FILE* fp) {
    long long int i,j;
    printf("salva matriz| linhas %lld colunas %lld\n", (*Matriz).row,(*Matriz).col);
    for(i=0; i<(*Matriz).row; i++ ){
        for(j=0; j<(*Matriz).col; j++) {
            fprintf(fp,"%lf ",(*Matriz).M[i*(*Matriz).col + j]);
        }
        fprintf(fp,"\n");
    }
}


