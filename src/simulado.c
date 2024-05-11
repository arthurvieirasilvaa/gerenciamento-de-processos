#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "string.h"

#include "../headers/estruturasDeDados.h"
#include "../headers/simulado.h"

#define MAX_FILE_LENGTH 50

void imprimeArray(ArrayProgramas *arr) {

    printf("\nIMPRIMINDO O ARRAY DO PROGRAMA!\n");

    for(int i = 0; ; i++) {
            
        if(arr[i].instrucao == 'R') {
            printf("%c %s\n", arr[i].instrucao, arr[i].novo_arquivo);
        }

        else if(arr[i].instrucao == 'S' || arr[i].instrucao == 'A' || arr[i].instrucao == 'D' || arr[i].instrucao == 'F') {
            printf("%c %d\n", arr[i].instrucao, arr[i].n);
        }

        else if(arr[i].instrucao == 'B') {
            printf("%c\n", arr[i].instrucao);
        }

        else {
            printf("%c\n", arr[i].instrucao);
            break;
        }
    }
}

void adicionaProcessoNaFila(No **fila, ProcessManager pm) {

    No *aux = (No*) malloc(sizeof(No));
    No *novo_processo = (No*) malloc(sizeof(No));

    // Verificando se a memória foi alocada corretamente:
    if(aux == NULL || novo_processo == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    novo_processo->processo = pm;
    novo_processo->proximo = NULL;

    if(*fila == NULL) {
        *fila = novo_processo;
    }

    else {
        aux = *fila;

        while(aux->proximo) {
            aux = aux->proximo;
        }

        aux->proximo = novo_processo;
    }
}

No *removeProcessoDaFila(No **fila) {

    No *remove = NULL;

    if(*fila) {
        remove = *fila;
        *fila = remove->proximo;
    }

    else {
        printf("Não há nenhum processo na fila\n");
    }

    return remove;
}
    
void atualizaVariavel(ProcessManager **pm, int indiceSimulado, int n) {
    (*pm)[indiceSimulado].cpu.valor = n;
}

void somaVariavel(ProcessManager **pm, int indiceSimulado, int n) {
    (*pm)[indiceSimulado].cpu.valor += n;
}

void subtraiVariavel(ProcessManager **pm, int indiceSimulado, int n) {
    (*pm)[indiceSimulado].cpu.valor -= n;
}

void bloqueiaProcessoSimulado() {

}

void terminaProcessoSimulado() {

}

void criaNovoProcessoSimulado(int n) {
    
    n = 0;
    printf("\nTESTE: %d\n", n);
}


void armazenarPrograma(ArrayProgramas **arr, char *entrada) {
        
    FILE *input;
    char caminho[100] = "./inputs/";
    strcat(caminho, entrada);
    input = fopen(caminho, "r"); // Abrimos o arquivo de entrada do Processo simulado

    printf("ARMAZENANDO O PROGRAMA: %s\n", caminho);

    // Se o arquivo de entrada não for encontrado:
    if(input == NULL) {
        printf("Input file was not found!\n");
        exit(1);
    }

    int tamProg = 1;

    while(fscanf(input, " %c", &((*arr)[tamProg-1].instrucao)) != EOF) {
        
        switch((*arr)[tamProg-1].instrucao) {
            case 'R':
                (*arr)[tamProg-1].novo_arquivo = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
                fscanf(input, " %s", (*arr)[tamProg-1].novo_arquivo);
                break;
                
            case 'S':
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
                
            case 'A':
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
                
            case 'D':
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
                
            case 'F':
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
                
            case 'B':
                break;
                
            case 'E':
                break;
                
            default:
                printf("Invalid command! Please, check the input file.\n");
                fclose(input);
                exit(1);
        }

        if((*arr)[tamProg-1].instrucao != 'E') {
            tamProg++;
            (*arr) = (ArrayProgramas*) realloc((*arr), tamProg * sizeof(ArrayProgramas));

            // Verificando se a memória foi alocada corretamente:
            if((*arr) == NULL) {
                printf("A memória não foi alocada corretamente!\n");
                exit(1);
            }
        }
    }

    fclose(input);
}

void substituiPrograma(ArrayProgramas **arr, ProcessManager **pm, char *novo_arquivo, int indiceSimulado) {    

    printf("\nSUBSTITUINDO PROGRAMA ATUAL\n");
    armazenarPrograma(arr, novo_arquivo);
    pm[indiceSimulado]->cpu.ponteiroPrograma = *arr;
    imprimeArray(*arr);

    (*pm)[indiceSimulado].cpu.contadorPrograma = 0; 
}

void verificarInstrucao(char instrucao, ProcessManager **pm, int tamPm, ArrayProgramas **arr, int indiceProg, int indiceSimulado) {

    printf("\nTESTE: %d\n", tamPm);
    int n;
    char *novo_arquivo = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));

    // Se o arquivo de entrada não for encontrado:
    if(novo_arquivo == NULL) {
        printf("Input file was not found!\n");
        exit(1);
    }
    
    switch (instrucao) {
        case 'R':
            strcpy(novo_arquivo, (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].novo_arquivo);
            printf("Arquivo: %s\n", novo_arquivo);
            substituiPrograma(arr, pm, novo_arquivo, indiceSimulado);
            break;
        
        case 'S':
            n = (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].n;
            atualizaVariavel(pm, indiceSimulado, n);
            break;
        
        case 'A':
            n = (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].n;
            somaVariavel(pm, indiceSimulado, n);
            break;
        
        case 'D':
            n = (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].n;
            subtraiVariavel(pm, indiceSimulado, n);
            break;
        
        case 'F':
            n = (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].n;
            break;
        
        case 'B':
            bloqueiaProcessoSimulado();
            break;
        
        case 'E':
            terminaProcessoSimulado();
            break;
    }
}