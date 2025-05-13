#include "bptree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ORDEM = 3;

NoBMais *criar_no(bool folha)
{
    NoBMais *novo = (NoBMais *)malloc(sizeof(NoBMais));
    novo->folha = folha;
    novo->num_chaves = 0;
    novo->proximo = -1;
    novo->posicao_arquivo = -1;
    for (int i = 0; i < MAX_FILHOS; i++)
        novo->filhos[i] = -1;
    return novo;
}

int salvar_no(NoBMais *no, FILE *arq)
{
    if (no->posicao_arquivo == -1)
    {
        fseek(arq, 0, SEEK_END);
        no->posicao_arquivo = ftell(arq) / LINHA_MAX;
    }
    else
    {
        fseek(arq, no->posicao_arquivo * LINHA_MAX, SEEK_SET);
    }

    char linha[LINHA_MAX];
    if (no->folha)
    {
        snprintf(linha, LINHA_MAX, "FOL|%d|", no->num_chaves);
        for (int i = 0; i < no->num_chaves; i++)
        {
            char temp[32];
            snprintf(temp, sizeof(temp), "%d:%d ", no->chaves[i], no->valores[i]);
            strcat(linha, temp);
        }
        char prox[32];
        snprintf(prox, sizeof(prox), "|%d\n", no->proximo);
        strcat(linha, prox);
    }
    else
    {
        snprintf(linha, LINHA_MAX, "INT|%d|", no->num_chaves);
        for (int i = 0; i < no->num_chaves; i++)
        {
            char temp[16];
            snprintf(temp, sizeof(temp), "%d ", no->chaves[i]);
            strcat(linha, temp);
        }
        strcat(linha, "|");
        for (int i = 0; i <= no->num_chaves; i++)
        {
            char temp[16];
            snprintf(temp, sizeof(temp), "%d ", no->filhos[i]);
            strcat(linha, temp);
        }
        strcat(linha, "\n");
    }

    fputs(linha, arq);
    fflush(arq);
    return no->posicao_arquivo;
}

NoBMais *carregar_no(int linha, FILE *arq)
{
    fseek(arq, linha * LINHA_MAX, SEEK_SET);
    char buffer[LINHA_MAX];
    if (!fgets(buffer, LINHA_MAX, arq))
        return NULL;

    NoBMais *no = (NoBMais *)malloc(sizeof(NoBMais));
    no->posicao_arquivo = linha;

    char tipo[4];
    sscanf(buffer, "%3[^|]", tipo);
    no->folha = strcmp(tipo, "FOL") == 0;

    if (no->folha)
    {
        sscanf(buffer, "FOL|%d|", &no->num_chaves);
        char *dados = strchr(buffer, '|');
        dados = strchr(dados + 1, '|') + 1;
        for (int i = 0; i < no->num_chaves; i++)
        {
            sscanf(dados, "%d:%d", &no->chaves[i], &no->valores[i]);
            dados = strchr(dados, ' ') + 1;
        }
        sscanf(strrchr(buffer, '|') + 1, "%d", &no->proximo);
    }
    else
    {
        sscanf(buffer, "INT|%d|", &no->num_chaves);
        char *dados = strchr(buffer, '|');
        dados = strchr(dados + 1, '|') + 1;
        for (int i = 0; i < no->num_chaves; i++)
        {
            sscanf(dados, "%d", &no->chaves[i]);
            dados = strchr(dados, ' ') + 1;
        }
        dados = strchr(dados, '|') + 1;
        for (int i = 0; i <= no->num_chaves; i++)
        {
            sscanf(dados, "%d", &no->filhos[i]);
            dados = strchr(dados, ' ');
            if (dados)
                dados++;
        }
    }
    return no;
}

int inserir(int chave, int valor, FILE *arq_indice)
{
    // TODO: implementar inserção em árvore B+ com leitura/escrita em arquivo
    return 1;
}

int buscar(int chave, FILE *arq_indice)
{
    // TODO: implementar busca em árvore B+ com leitura de arquivo
    return 1;
}

int calcular_altura(FILE *arq_indice)
{
    // TODO: ler os nós da raiz até folha e contar altura
    return 1;
}
