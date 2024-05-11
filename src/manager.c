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

void inicializaEstruturasDeDados(ProcessManager **pm, int indiceSimulado, ArrayProgramas *arr) {
    
    (*pm)[indiceSimulado].tempo = 0; // Inicialização do tempo
 
    pm[indiceSimulado]->cpu.ponteiroPrograma = arr;
    (*pm)[indiceSimulado].cpu.contadorPrograma = 0; 
    (*pm)[indiceSimulado].cpu.valor = 0;
    (*pm)[indiceSimulado].cpu.fatiaTempo = 0;
    (*pm)[indiceSimulado].cpu.tempoAtual = 0;

    (*pm)[indiceSimulado].pcb = (TabelaPcb*) malloc(sizeof(TabelaPcb)); 
    if ((*pm)[indiceSimulado].pcb == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    pm[indiceSimulado]->pcb->ponteiroContadorPrograma = NULL;
    (*pm)[indiceSimulado].pcb->valor = 0;
    (*pm)[indiceSimulado].pcb->prioridade = 0;
    (*pm)[indiceSimulado].pcb->estado = 0;
    (*pm)[indiceSimulado].pcb->tempoInicio = 0;
    (*pm)[indiceSimulado].pcb->cpuUsada = 0;

    pm[indiceSimulado]->estadoPronto = NULL;
    pm[indiceSimulado]->estadoBloqueado = NULL;
    pm[indiceSimulado]->estadoExecutando = NULL;
}

void executaComandoQ(ProcessManager **pm, ArrayProgramas **arr, int indiceSimulado, int tamPm) {

    int indiceProg = (*pm)[indiceSimulado].cpu.contadorPrograma;

    char instrucao;
    instrucao = (*pm)[indiceSimulado].cpu.ponteiroPrograma[indiceProg].instrucao;

    verificarInstrucao(instrucao, pm, tamPm, arr, indiceProg, indiceSimulado);

    (*pm)[indiceSimulado].cpu.contadorPrograma += 1;
    (*pm)[indiceSimulado].cpu.tempoAtual += 1;
}

void substituiProcesso() {

}

void transicaoDeEstados() {

}

void escalonamento() {

}

void trocaDeContexto() {

}

void criaProcessoReporter(ProcessManager *pm, int tamPm) {

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
        imprimeEstadoAtual(pm, tamPm);
    }
}

/*
    Função utilizada para ler o comando enviado pelo Processo commander 
    (Processo principal) através do pipe:
*/
void verificaComandoPipe(char comando, ArrayProgramas **arr, int *indiceSimulado, ProcessManager **pm, int *tamPm) {

    switch (comando) {
        case 'Q':
            executaComandoQ(pm, arr, *indiceSimulado, *tamPm);
            break;
        
        case 'U':
            break;
        
        // Imprime o estado atual do sistema:
        case 'P':
            criaProcessoReporter(*pm, *tamPm);
            break;
        
        case 'T':
            criaProcessoReporter(*pm, *tamPm);
            break;
        default:
            printf("Invalid command! Please try again...\n");
    }
}