#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TAREFAS 10

typedef struct Tarefas{
    char nome[10];
    int periodo;
    int deadline;
    int burst;
    int tempo_restante;
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

    char *algoritimo;

    algoritimo = argv[1];

    if(strcmp(algoritimo, "rate") != 0 && strcmp(algoritimo, "edf") != 0){
        fprintf(stderr, "Erro: ./scheduler <rate> <edf> voo.tx\nt");
        exit(1);
    }

    FILE *arq = fopen(argv[2], "w");
    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abiri o arquivo\n");
        exit(1);
    }

    int tempoTotal = fscanf(arq, "%d", &tempoTotal);

    if(tempoTotal < 0){
        fprintf(stderr, "Erro: Tempo Invalido\n");
        exit(1);
    }

    int cont = 0;
    while(fscanf(arq, "%s %d %d %d", tarefas[cont].nome, &tarefas[cont].periodo, &tarefas[cont].deadline, &tarefas[cont].burst)){
        
        if(! feof(arq)){
            fprintf(stderr, "Erro: arquivo de entrada malformado\n");
            exit(1);
        }
        
        if(tarefas[cont].burst > tarefas[cont].deadline || tarefas[cont].deadline > tarefas[cont].periodo){
            fprintf(stderr, "Erro: parametros inconsistente\n");
            exit(1);
        }

        tarefas[cont].tempo_restante = 0;
        tarefas[cont].instante_chegada = 0;
        tarefas[cont].contador_lost = 0;
        tarefas[cont].contador_complete = 0;
        tarefas[cont].contador_killed = 0;

        cont++;
    }

    return 0;
}
