#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TAREFAS 10

typedef struct Tarefas{
    char nome[10];
    int periodo;
    int deadline;
    int burst;
    int burst_restante;
    int instante_chegada;
    int contador_lost;
    int contador_complete;
    int contador_killed;
}Tarefas;

int main(int argc, char const *argv[]){

    Tarefas tarefas[MAX_TAREFAS];
    
    if(argc != 3){
        fprintf(stderr, "Erro: ./scheduler <rate> <edf> voo.txt\n");
        exit(1);
    }

    const char *algoritimo;

    algoritimo = argv[1];

    if(strcmp(algoritimo, "rate") != 0 && strcmp(algoritimo, "edf") != 0){
        fprintf(stderr, "Erro: ./scheduler <rate> <edf> voo.tx\nt");
        exit(1);
    }

    FILE *arq = fopen("voo.txt", "r");
    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abiri o arquivo\n");
        exit(1);
    }
    
    int tempoTotal;
    int itens_lidos = fscanf(arq, "%d", &tempoTotal);

    if(itens_lidos != 1 || tempoTotal <= 0){
        fprintf(stderr, "Erro: Tempo Invalido\n");
        exit(1);
    }
    printf("%d\n", tempoTotal);

    int cont = 0;
    while(fscanf(arq, "%s %d %d %d", tarefas[cont].nome, &tarefas[cont].periodo, &tarefas[cont].deadline, &tarefas[cont].burst)){
        
        if(! feof(arq)){
            fprintf(stderr, "Erro: arquivo de entrada mal formado\n");
            exit(1);
        }
        
        if(tarefas[cont].burst > tarefas[cont].deadline || tarefas[cont].deadline > tarefas[cont].periodo){
            fprintf(stderr, "Erro: parametros inconsistente\n");
            exit(1);
        }

        tarefas[cont].burst_restante = 0;
        tarefas[cont].instante_chegada = 0;
        tarefas[cont].contador_lost = 0;
        tarefas[cont].contador_complete = 0;
        tarefas[cont].contador_killed = 0;

        cont++;
    }

    int criterio_atual, criterio_melhor, idle = 0, units = 0;

    FILE *arqRate = fopen("rate_lcw.out", "w");
    if(arqRate == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abiri o arquivo\n");
        exit(1);
    }
        
    FILE *arqEdf = fopen("edf_lcw.out", "w");
    if(arqEdf == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abiri o arquivo\n");
        exit(1);
    }

    for(int t=0; t<tempoTotal; t++){
        for(int i=0; i<cont; i++){
            if(t % tarefas[i].periodo == 0){
                tarefas[i].burst_restante = tarefas[i].burst;
                tarefas[i].instante_chegada = t;
            }

            if(tarefas[i].instante_chegada + tarefas[i].deadline == t && tarefas[i].burst_restante > 0){
                tarefas[i].burst_restante = 0;
                tarefas[i].contador_lost++;
            }
        }

        int indice_escolhido = -1;
        
        for(int i=0; i<cont; i++){
            if(tarefas[i].burst_restante > 0){
                if(strcmp(algoritimo, "rate") == 0){
                    criterio_atual = tarefas[i].periodo;
                    
                    criterio_melhor = (indice_escolhido != -1) ? tarefas[indice_escolhido].periodo : 9999;
                }

                else{
                    criterio_atual = tarefas[i].instante_chegada + tarefas[i].deadline;
                    
                    criterio_melhor = (indice_escolhido != -1) ? tarefas[indice_escolhido].instante_chegada + tarefas[indice_escolhido].deadline : 9999;
                }

                if(indice_escolhido == -1 || criterio_atual < criterio_melhor) indice_escolhido = i;
            }
        }

        if(indice_escolhido != -1){
            tarefas[indice_escolhido].burst_restante -= 1;

            if(tarefas[indice_escolhido].burst_restante == 0)tarefas[indice_escolhido].contador_complete += 1;
        }

        else{
            idle++;
        }
        
        for(int i=0; i<cont; i++){
            if(tarefas[i].burst_restante > 0){
                tarefas[i].contador_killed++;
            }
        }
        
        char *tarefaAntiga = tarefas[-1].nome;
        if(strcmp(tarefaAntiga, tarefas[t].nome) == 0){
            units++;
        } 

        else{
            if(strcmp(algoritimo, "rate") == 0){
                fprintf(arqRate, "[%s] for %d units - ", tarefas[t].nome, units);
                units = 0;  
            }

            else if(strcmp(algoritimo, "edf") == 0){
                fprintf(arqEdf, "[%s] for %d units - ", tarefas[t].nome, units);
                units = 0;  
            }
        }

    }

    return 0;
}
