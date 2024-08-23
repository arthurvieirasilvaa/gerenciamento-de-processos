#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "../headers/estruturasDeDados.h"
#include "../headers/manager.h"
#include "../headers/simulado.h"

#define MAX_FILE_LENGTH 50

// Função utilizada para atualizar o valor da variável inteira para n (instrução S):
void atualizaVariavel(ProcessManager *pm, int indiceProg) {
    int n = (*pm).cpu.ponteiroPrograma[indiceProg].n;
    (*pm).cpu.valor = n;
}

// Função utilizada para somar n na variável inteira (instrução A):
void somaVariavel(ProcessManager *pm, int indiceProg) {
    int n = (*pm).cpu.ponteiroPrograma[indiceProg].n;
    (*pm).cpu.valor += n;
}

// Função utilizada para subtrair n na variável inteira (instrução D):
void subtraiVariavel(ProcessManager *pm, int indiceProg) {
    int n = (*pm).cpu.ponteiroPrograma[indiceProg].n;
    (*pm).cpu.valor -= n;
}

// Função utilizada para bloquear o processo simulado (instrução B):
void bloqueiaProcessoSimulado(ProcessManager *pm, int *indiceProcesso) {
    (*pm).pcb[*indiceProcesso].estado = 'B'; // Bloquea o Processo atual
    
    // Verifica se há algum processo pronto para ser executado:
    if((*pm).estadoPronto) {
        (*pm).pcb[*indiceProcesso].estado = "B";
        
        // Removemos o índice do processo atual da fila de estadoExecutando:
        No *removido = NULL;
        removido = removerDaFila(&(*pm).estadoExecutando);

        // Adicionamos o índice do processo que foi bloqueado na fila de estadoBloqueado: 
        inserirNaFila(&(*pm).estadoBloqueado, *indiceProcesso);
    }   
    
    else {
        printf("Não há nenhum processo pronto para ser executado!\n");
        printf("Digite U para desbloquear o primeiro processo bloqueado para a fila de prontos\n");
    }
}

// Função utilizada para terminar o processo simulado (instrução E):
void terminaProcessoSimulado(ProcessManager *pm, int indiceProcesso) {
    kill((*pm).pcb[indiceProcesso].idProcesso, SIGKILL);
}

// Função utilizada para criar um novo processo simulado (instrução F):
void criaNovoProcessoSimulado(ProcessManager *pm, int *tamPcb, int indiceProcesso, int indiceProg) {
    int n = (*pm).cpu.ponteiroPrograma[indiceProg].n;
    int contadorProgAux = (*pm).cpu.contadorPrograma;

    (*tamPcb)++; // Incrementamos o tamanho da variável tamPcb

    pm->pcb = (TabelaPcb*) realloc(pm->pcb, *tamPcb * sizeof(TabelaPcb)); 
    int pid = fork();
    
    if(pid == -1) {
        printf("Ocorreu um erro ao realizar um fork\n");
        exit(1);
    }

    // Processo principal:
    if(pid != 0) {

        (*pm).cpu.contadorPrograma += n - 1;
    }

    // Processo filho:
    else {

        indiceProcesso++; // Incrementamos a variável indiceProcesso
        inicializaEstruturasDeDados(pm, *tamPcb, indiceProcesso, (*pm).cpu.ponteiroPrograma);

        // Inicializamos as estruturas de dados do novo Processo simulado
        (*pm).pcb[indiceProcesso].idProcesso = getpid();
        (*pm).pcb[indiceProcesso].idProcessoPai = getppid();
        (*pm).cpu.contadorPrograma = contadorProgAux;
        
        // O tempo de início é inicializado com o tempo atual:
        (*pm).pcb[indiceProcesso].tempoInicio = (*pm).cpu.tempoAtual;
        
        (*pm).cpu.fatiaTempo = 0;
        
        // O índice do novo programa é colocado no array de estadoPronto:
        inserirNaFila(&(*pm).estadoPronto, indiceProcesso);
    }
}

/* 
    Função utilizada para armazenar o programa de um Processo simulado em um 
    vetor, com uma instrução para cada posição:
*/  
void armazenarPrograma(ArrayProgramas **arr, char *entrada) {
        
    FILE *input;
    char caminho[100] = "./inputs/"; // Caminho onde estão localizados os arquivos de entrada
    strcat(caminho, entrada); // Concatenamos a variável caminho com o nome do arquivo de entrada
    input = fopen(caminho, "r"); // Abrimos o arquivo de entrada do Processo simulado

    // Se o arquivo de entrada não for encontrado:
    if(input == NULL) {
        printf("Input file was not found!\n");
        exit(1);
    }

    int tamProg = 1; // Variável usada para representar o tamanho do array do programa

    // Lemos o arquivo de entrada até o final:
    while(fscanf(input, " %c", &((*arr)[tamProg-1].instrucao)) != EOF) {
        
        switch((*arr)[tamProg-1].instrucao) {
            // Se a linha tiver uma instrução R:
            case 'R':
                (*arr)[tamProg-1].novo_arquivo = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
                // Armazenamos também o nome do arquivo do programa do novo Processo simulado:
                fscanf(input, " %s", (*arr)[tamProg-1].novo_arquivo);
                break;
            
            // Se a linha tiver uma instrução S:
            case 'S':
                // Armazenamos também um inteiro n:
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
            
            // Se a linha tiver uma instrução A:
            case 'A':
                // Armazenamos também um inteiro n:
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
            
            // Se a linha tiver uma instrução D:
            case 'D':
                // Armazenamos também um inteiro n:
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
            
            // Se a linha tiver uma instrução F:
            case 'F':
                // Armazenamos também um inteiro n:
                fscanf(input, " %d", &((*arr)[tamProg-1].n));
                break;
            
            // Se a linha tiver uma instrução B:
            case 'B':
                // Só há a instrução nessa linha do programa:
                break;
            
            // Se a linha tiver uma instrução E:
            case 'E':
                // Só há a instrução nessa linha do programa:
                break;
            
            // Se tiver outra instrução:
            default:
                printf("Invalid command! Please, check the input file.\n");
                fclose(input);
                exit(1);
        }

        // Se ainda não chegamos na última instrução (E):
        if((*arr)[tamProg-1].instrucao != 'E') {
            tamProg++; // Incrementamos o tamanho do array do programa em 1 unidade
            // Fazemos a realocação do arr (para que o seu tamanho aumente 1 unidade):
            (*arr) = (ArrayProgramas*) realloc((*arr), tamProg * sizeof(ArrayProgramas));

            // Verificando se a memória foi alocada corretamente:
            if((*arr) == NULL) {
                printf("A memória não foi alocada corretamente!\n");
                exit(1);
            }
        }
    }

    fclose(input); // Fechamos o arquivo de entrada lido
}

/* 
    Função utilizada para substituir o programa do Processo simulado por um novo programa
    (instrução R):
*/
void substituiPrograma(ArrayProgramas **arr, ProcessManager *pm, int indiceProg) {   

    char *novo_arquivo = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));

    // Se o arquivo de entrada não for encontrado:
    if(novo_arquivo == NULL) {
        printf("O arquivo de entrada não foi encontrado!\n");
        exit(1);
    }

    // Copiamos o nome do novo arquivo para a variável novo_arquivo:
    strcpy(novo_arquivo, (*pm).cpu.ponteiroPrograma[indiceProg].novo_arquivo);

    armazenarPrograma(arr, novo_arquivo); // Armazenamos o programa do novo arquivo
    
    // A variável ponteiroPrograma agora aponta para o programa do novo arquivo:
    (*pm).cpu.ponteiroPrograma = *arr; 
   
    /* 
        Contador de Programa é colocado em -1 pois, ao retornarmos para a função
        executaProximaInstrucao() ele será incrementado novamente tendo, assim, 
        o valor 0:
    */
    (*pm).cpu.contadorPrograma = -1;
    (*pm).cpu.valor = 0; // Reseta o valor da variável inteira

    free(novo_arquivo); // Liberamos a memória alocada dinamicamente
}

/*
    Função utilizada para verificar qual instrução será executada no momento no programa
    do Processo simulado:
*/
void verificarInstrucao(char instrucao, ProcessManager *pm, int *tamPcb, ArrayProgramas **arr, int indiceProg, int indiceProcesso) {    
    switch (instrucao) {
        // Se a instrução executada for R:
        case 'R':
            substituiPrograma(arr, pm, indiceProg);
            break;
        
        // Se a instrução executada for S:
        case 'S':
            atualizaVariavel(pm, indiceProg);
            break;

        // Se a instrução executada for A:
        case 'A':
            somaVariavel(pm, indiceProg);
            break;
        
        // Se a instrução executada for D:
        case 'D':
            subtraiVariavel(pm, indiceProg);
            break;
        
        // Se a instrução executada for F:
        case 'F':
            criaNovoProcessoSimulado(pm, tamPcb, indiceProcesso, indiceProg);
            break;
        
        // Se a instrução executada for B:
        case 'B':
            bloqueiaProcessoSimulado(pm, &indiceProcesso);
            break;
        
        // Se a instrução executada for E:
        case 'E':
            terminaProcessoSimulado(pm, indiceProcesso);
            (*tamPcb)--;
            break;
    }
}