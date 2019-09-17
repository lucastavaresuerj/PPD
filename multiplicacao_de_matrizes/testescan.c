#include <stdio.h>
#include <stdlib.h>

int main() {
    int linhas = 0, colunas = 0;
    FILE *arq;
    char scan;
    if ((arq = fopen("entrada", "r")) == (FILE *) NULL) {
        printf("erro ao abrir arquivo");
        exit(1);
    }
    
    while ( !feof(arq) ) { //enquanto nao for o final do arquivo
        fscanf(arq, "%c",&scan);
        if(scan == '\n') {
            linhas++;
            colunas++;
        } else if (scan == ' ') {
            colunas++;
        } 
    } 
    linhas--;
    colunas--;
    colunas = colunas/linhas;
    printf("linhas :%d colunas %d",linhas, colunas);
}