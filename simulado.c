#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "string.h"

#include "headers/estruturasDeDados.h"
//#include "headers/manager.h"
#include "headers/simulado.h"

#define MAX_FILE_LENGTH 50

void imprimeArray(ArrayProgramas ***arr, int tamPm, int tamProg) {

    printf("\nIMPRIMINDO OS ARRAYS DOS PROGRAMA!\n");

    for(int i = 0; i < tamPm; i++) {

        printf("PROCESSO SIMULADO %d:\n", i+1);

        for(int j = 0; j < tamProg; j++) {
            
            printf("%c ", (*arr)[i][j].instrucao);

            switch((*arr)[i][j].instrucao) {
                case 'R':
                printf("%s\n", (*arr)[i][j].novo_arquivo);
                break;
                
            case 'S':
                printf("%d\n", (*arr)[i][j].n);
                break;
                
            case 'A':
                printf("%d\n", (*arr)[i][j].n);
                break;
                
            case 'D':
                printf("%d\n", (*arr)[i][j].n);
                break;
                
            case 'F':
                printf("%d\n", (*arr)[i][j].n);
                break;
            }   
        }

        printf("\n");
    }
}
    
void atualizaVariavel(ProcessManager **pm, int n) {
    (*pm)->cpu.valor = n;
}

void somaVariavel(ProcessManager **pm, int n) {
    (*pm)->cpu.valor += n;
}

void subtraiVariavel(ProcessManager **pm, int n) {
    (*pm)->cpu.valor -= n;
}

void bloqueiaProcessoSimulado() {

}

void terminaProcessoSimulado() {

}

void criaNovoProcessoSimulado(int n) {
    printf("TESTE: %d\n", n);
}

void substituiPrograma() {    
}

void armazenarPrograma(ArrayProgramas ***arr, int tamPm, int *tamProg) {
    
    printf("\nLENDO PROGRAMA...\n");
    
    FILE *input;
    input = fopen("./inputs/init.txt", "r"); // Abrimos o arquivo init.txt

    // Se o arquivo init não for encontrado:
    if(input == NULL) {
        printf("File init was not found!\n");
        exit(1);
    }

    while(fscanf(input, " %c", &((*arr)[tamPm-1][(*tamProg)-1].instrucao)) != EOF) {
        
        switch((*arr)[tamPm-1][(*tamProg)-1].instrucao) {
            case 'R':
                (*arr)[tamPm-1][(*tamProg)-1].novo_arquivo = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
                fscanf(input, " %s", (*arr)[tamPm-1][(*tamProg)-1].novo_arquivo);
                break;
                
            case 'S':
                fscanf(input, " %d", &((*arr)[tamPm-1][(*tamProg)-1].n));
                break;
                
            case 'A':
                fscanf(input, " %d", &((*arr)[tamPm-1][(*tamProg)-1].n));
                break;
                
            case 'D':
                fscanf(input, " %d", &((*arr)[tamPm-1][(*tamProg)-1].n));
                break;
                
            case 'F':
                fscanf(input, " %d", &((*arr)[tamPm-1][(*tamProg)-1].n));
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

        (*tamProg)++;
        (*arr)[tamPm-1] = (ArrayProgramas*) realloc(*arr[tamPm-1], (*tamProg) * sizeof(ArrayProgramas));

        // Verificando se a memória foi alocada corretamente:
        if(arr[tamPm-1] == NULL) {
            printf("A memória não foi alocada corretamente!\n");
            exit(1);
        }
    }

    fclose(input);
}

void verificarInstrucao(char instrucao, ProcessManager **pm, int tamPm, ArrayProgramas ***arr, int j) {

    printf("\nVERIFICANDO INSTRUÇÃO (R, S, A, D, F, B ou E)\n");

    int n;
    char *novo_arquivo = NULL;

    printf("INSTRUÇÃO %c\n", instrucao);
    switch (instrucao) {
        case 'R':
            strcpy(novo_arquivo, (*arr)[tamPm-1][j].novo_arquivo);
            substituiPrograma();
            break;
        
        case 'S':
            n = (*arr)[tamPm-1][j].n;
            atualizaVariavel(pm, n);
            break;
        
        case 'A':
            n = (*arr)[tamPm-1][j].n;
            somaVariavel(pm, n);
            break;
        
        case 'D':
            n = (*arr)[tamPm-1][j].n;
            subtraiVariavel(pm, n);
            break;
        
        case 'F':
            n = (*arr)[tamPm-1][j].n;
            break;
        
        case 'B':
            bloqueiaProcessoSimulado();
            break;
        
        case 'E':
            terminaProcessoSimulado();
            break;
    }
}