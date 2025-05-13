#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bptree/bptree.h"

int ORDEM = ORDEM_PADRAO;

void processar_csv(FILE *csv, FILE *relacao, FILE *arq_indice, FILE *saida)
{
    char linha[LINHA_MAX];

    fgets(linha, sizeof(linha), csv);

    while (fgets(linha, sizeof(linha), csv))
    {
        linha[strcspn(linha, "\n")] = 0;

        char *token = strtok(linha, ",");
        int contador = 0;
        int ano_colheita = 0;

        while (token != NULL)
        {
            if (contador == 2)
            {
                ano_colheita = atoi(token);
                break;
            }
            token = strtok(NULL, ",");
            contador++;
        }

        long pos = ftell(relacao);
        fprintf(relacao, "%s\n", linha);

        inserir(ano_colheita, pos, arq_indice);

        fprintf(saida, "INC:%d\n", ano_colheita);
    }

    int altura = calcular_altura(arq_indice);
    fprintf(saida, "H/%d\n", altura);
}

// TODO: Revisar a implementação de main
int main()
{
    FILE *csv = fopen("vinhos.csv", "r");
    FILE *relacao = fopen("relacao.dat", "a+");
    FILE *arq_indice = fopen("indice.dat", "r+");
    FILE *saida = fopen("out.txt", "w");

    if (!csv || !relacao || !saida)
    {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    if (!arq_indice)
    {
        arq_indice = fopen("indice.dat", "w+");
        if (!arq_indice)
        {
            perror("Erro ao criar arquivo de índice");
            return 1;
        }
    }

    processar_csv(csv, relacao, arq_indice, saida);

    fclose(csv);
    fclose(relacao);
    fclose(arq_indice);
    fclose(saida);
    return 0;
}
