#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "../headers/estruturasDeDados.h"
#include "../headers/simulado.h"
#include "../headers/manager.h"
#include "../headers/reporter.h"

void inserirNaFila(No **fila, int indiceInserido) {
    No *aux, *novo = (No*) malloc(sizeof(No));

    if(novo == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    novo->indiceProcesso = indiceInserido;
    novo->proximo = NULL;

    if(*fila == NULL) {
        *fila = novo;
    }

    else {
        aux = *fila;

        while (aux->proximo) {
            aux = aux->proximo;
        }
        aux->proximo = novo;
    }
}

No *removerDaFila(No **fila) {
    No *removido = NULL;

    if(*fila) {
        removido = *fila;
        *fila = removido->proximo;
    }

    else {
        printf("A fila está vazia!");
    }

    return removido;
}

// Função utilizada para inicializar a estrutura de dados para o Processo Manager:
void inicializaEstruturasDeDados(ProcessManager *pm, int tamPcb, int indiceProcesso, ArrayProgramas *arr) {
    
    (*pm).tempo = 0; // Inicialização do tempo
    
    (*pm).cpu.ponteiroPrograma = arr;
    (*pm).cpu.contadorPrograma = 0; 
    (*pm).cpu.valor = 0;
    (*pm).cpu.fatiaTempo = 0;
    (*pm).cpu.tempoAtual = 0;

    pm->pcb = (TabelaPcb*) malloc(tamPcb * sizeof(TabelaPcb)); 
    
    if ((*pm).pcb == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    (*pm).pcb[indiceProcesso].idProcesso = getpid();
    (*pm).pcb[indiceProcesso].idProcessoPai = getppid();
    pm->pcb[indiceProcesso].ponteiroContadorPrograma = NULL;
    (*pm).pcb[indiceProcesso].valor = 0;
    (*pm).pcb[indiceProcesso].prioridade = indiceProcesso;
    (*pm).pcb[indiceProcesso].estado = 'P';
    (*pm).pcb[indiceProcesso].tempoInicio = (*pm).tempo;
    (*pm).pcb[indiceProcesso].cpuUsada = 0;

    pm->estadoPronto = NULL;
    pm->estadoExecutando = NULL;
    pm->estadoBloqueado = NULL;
}

void escalonar(ProcessManager *pm, int *indiceProcesso) {
    
    int prioridade = (*pm).pcb[*indiceProcesso].prioridade;
    int novoIndiceProcesso = -1;
    No *removido = NULL;
    
    // Se há algum processo para pronto para ser executado:
    if((*pm).estadoPronto) {

        // Procura pelo processo com maior prioridade:
        while(pm->estadoPronto) {
            
            if((*pm).pcb[*indiceProcesso].prioridade > prioridade) {
                novoIndiceProcesso = (*pm).estadoPronto->indiceProcesso;
            }

            (*pm).estadoPronto = (*pm).estadoPronto->proximo;
        }
        
        /*
            Se há um processo com prioridade maior do que o que já está
            executando no momento:
        */
        if(novoIndiceProcesso != -1) {
            (*pm).pcb[*indiceProcesso].estado = 'P';
            inserirNaFila(&(*pm).estadoPronto, *indiceProcesso);
            removido = removerDaFila(&(*pm).estadoExecutando);

            *indiceProcesso = novoIndiceProcesso;
            (*pm).pcb[*indiceProcesso].estado = 'E';
            inserirNaFila(&(*pm).estadoExecutando, *indiceProcesso);
            removido = removerDaFila(&(*pm).estadoPronto);
        }
    }   

    // Liberamos a memória alocada dinamicamente:
    free(removido);
}

/* 
    Função utilizada para executar a próxima instrução do programa do Processo simulado
    (se caso o usuário digitar o comando Q):
*/
void executarProximaInstrucao(ProcessManager *pm, ArrayProgramas **arr, int indiceProcesso, int tamPcb) {

    // Variável utilizada para "acompanhar" em qual posição do array do programa estamos:
    int indiceProg = (*pm).cpu.contadorPrograma;

    // Variável utilizada para armazenar qual a instrução atual do array do programa:
    char instrucao = (*pm).cpu.ponteiroPrograma[indiceProg].instrucao;
    
    verificarInstrucao(instrucao, pm, &tamPcb, arr, indiceProg, indiceProcesso); 
    printf("Executando instrução %c...\n", instrucao);

    escalonar(pm, &indiceProcesso);
    printf("Escalonando...\n");

    // Incrementamos o contador de programa e o tempo atual:
    (*pm).cpu.contadorPrograma += 1;
    (*pm).tempo += 1;
    (*pm).cpu.tempoAtual += 1;
}

/*
    Função utilizada para desbloquear o primeiro Processo simulado que está na fila 
    de bloqueados (se caso o usuário digitar o comando U):
*/
void desbloquearProcesso(ProcessManager *pm) {

    No *removido = NULL;
    removido = removerDaFila(&(*pm).estadoBloqueado);

    if(removido == NULL) {
        printf("Não há nenhum processo na fila de bloqueados!");
    }

    else {
        printf("Indíce do processo: %d\n", (*removido).indiceProcesso);
        printf("Processo remvido da fila de bloqueados!\n");
    }

    // Liberamos a memória alocada dinamicamente:
    free(removido);
}

// Função utilizada para criar um Processo reporter (se caso o usuário digitar o comando P):
void criaProcessoReporter(ProcessManager pm) {

    int pid = fork();

    if(pid == -1) {
        exit(1);
    }

    // Processo manager (Processo principal):
    if(pid != 0) {
        wait(NULL); // Espera o Processo reporter (Processo filho) terminar a sua execução
    }

    // Processo reporter (Processo filho):
    else {
        imprimeEstadoAtual(pm);
        kill(getpid(), SIGKILL); 
    }
}

/*
    Função utilizada para ler o comando enviado pelo Processo commander 
    (Processo principal) através do pipe:
*/
void verificaComandoPipe(char comando, ArrayProgramas **arr, int *indiceProcesso, ProcessManager *pm, int *tamPcb) {

    switch (comando) {
        // Se caso o usuário digitou o comando Q:
        case 'Q':
            executarProximaInstrucao(pm, arr, *indiceProcesso, *tamPcb);
            break;
        
        // Se caso o usuário digitou o comando U:
        case 'U':
            desbloquearProcesso(pm);
            break;
        
        // Se caso o usuário digitou o comando P:
        case 'P':
            criaProcessoReporter(*pm);
            break;
        
        // Se caso o usuário digitou o comando T:
        case 'T':
            criaProcessoReporter(*pm);
            break;
        
        // Se caso o usuário digitou outro comando:
        default:
            printf("Invalid command! Please try again...\n");
    }
}
