#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "headers/estruturasDeDados.h"
#include "headers/simulado.h"
#include "headers/manager.h"
#include "headers/reporter.h"

void inicializaEstruturasDeDados(ProcessManager **pm, int tamPm, ArrayProgramas **arr) {

    printf("\nINICIALIZANDO ESTRUTURAS DE DADOS\n");
    
    (*pm)[tamPm-1].tempo = 0; // Inicialização do tempo
 
    pm[tamPm-1]->cpu.ponteiroPrograma = arr[tamPm-1];
    (*pm)[tamPm-1].cpu.contadorPrograma = 0; 
    (*pm)[tamPm-1].cpu.valor = 0;
    (*pm)[tamPm-1].cpu.fatiaTempo = 0;
    (*pm)[tamPm-1].cpu.tempoAtual = 0;

    (*pm)[tamPm-1].pcb = (TabelaPcb*) malloc(sizeof(TabelaPcb)); 
    if ((*pm)[tamPm-1].pcb == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    (*pm)[tamPm-1].pcb->ponteiroContadorPrograma = NULL;
    (*pm)[tamPm-1].pcb->valor = 0;
    (*pm)[tamPm-1].pcb->prioridade = 0;
    (*pm)[tamPm-1].pcb->estado = 0;
    (*pm)[tamPm-1].pcb->tempoInicio = 0;
    (*pm)[tamPm-1].pcb->cpuUsada = 0;

    (*pm)[tamPm-1].estadoPronto = NULL;
    (*pm)[tamPm-1].estadoBloqueado = NULL;
    (*pm)[tamPm-1].estadoExecutando = NULL;

    printf("ESTRUTURAS DE DADOS INICIALIZADA!\n");
}

void executaComandoQ(ProcessManager **pm, ArrayProgramas ***arr, int tamPm, int tamProg) {

    printf("\nEXECUTANDO COMANDO Q\n");

    char comando;

    (*pm)[tamPm-1].cpu.contadorPrograma += 1;
    (*pm)[tamPm-1].cpu.tempoAtual += 1;

    int i = (*pm)[tamPm-1].cpu.contadorPrograma - 1;

    comando = (*arr)[i][tamProg-1].instrucao;

    verificarInstrucao(comando, pm, tamPm, arr, i);
}

void substituiProcesso() {

}

void transicaoDeEstados() {

}

void escalonamento() {

}

void trocaDeContexto() {

}

void criaProcessoReporter() {
}

/*
    Função utilizada para ler o comando enviado pelo Processo commander 
    (Processo principal) através do pipe:
*/
void verificaComandoPipe(char comando, ProcessManager **pm, int *tamPm, ArrayProgramas ***arr, int *tamProg) {

    printf("\nVERIFICANDO COMANDO %c\n", comando);

    switch (comando) {
        case 'Q':
            executaComandoQ(pm, arr, (*tamPm), (*tamProg));
            break;
        
        case 'U':
            break;
        
        // Imprime o estado atual do sistema:
        case 'P':
            imprimeEstadoAtual((*pm), (*tamPm));
            break;
        
        case 'T':
            break;
        default:
            printf("Invalid command! Please try again...\n");
    }
}
