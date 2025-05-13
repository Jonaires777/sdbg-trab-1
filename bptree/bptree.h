#ifndef BPTREE_H
#define BPTREE_H

#include <stdbool.h>
#include <stdio.h>

#define ORDEM_PADRAO 3
#define MAX_FILHOS 10
#define LINHA_MAX 256

typedef struct NoBMais
{
    bool folha;
    int num_chaves;
    int chaves[MAX_FILHOS - 1];
    int filhos[MAX_FILHOS];
    int valores[MAX_FILHOS - 1];
    int proximo;
    int posicao_arquivo;
} NoBMais;

extern int ORDEM;

NoBMais *criar_no(bool folha);
int salvar_no(NoBMais *no, FILE *arq);
NoBMais *carregar_no(int linha, FILE *arq);
int inserir(int chave, int valor, FILE *arq_indice);
int buscar(int chave, FILE *arq_indice);
int calcular_altura(FILE *arq_indice);

#endif