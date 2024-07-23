#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "../headers/estruturasDeDados.h"
#include "../headers/reporter.h"

// Função utilizada para imprimir o estado atual do sistema:
void imprimeEstadoAtual(ProcessManager pm) {
    int i;

    printf("\n****************************************************************\n");
    printf("Estado sistema\n");
    printf("****************************************************************\n\n");
    printf("TEMPO ATUAL: %d\n", pm.tempo);


    while(pm.estadoExecutando) {
        i = pm.estadoExecutando->indiceProcesso;
        
        printf("PROCESSO EXECUTANDO:\n");
        printf("pid %d, ppid %d, prioridade %d, valor %d, tempo início %d, CPU usada até agora %d\n", pm.pcb[i].idProcesso, pm.pcb[i].idProcessoPai, pm.pcb[i].prioridade, pm.cpu.valor, pm.pcb[i].tempoInicio, pm.pcb[i].cpuUsada);

        printf("Indíce do Processo: %d\n", pm.estadoExecutando->indiceProcesso);
        pm.estadoExecutando = pm.estadoExecutando->proximo;
    }
    
    printf("BLOQUEADO:\n");
    printf("Fila processos bloqueados:\n");
    
    while(pm.estadoBloqueado) {
        i = pm.estadoBloqueado->indiceProcesso;
        
        printf("pid %d, ppid %d, prioridade %d, valor %d, tempo início %d, CPU usada até agora %d\n", pm.pcb[i].idProcesso, pm.pcb[i].idProcessoPai, pm.pcb[i].prioridade, pm.cpu.valor, pm.pcb[i].tempoInicio, pm.pcb[i].cpuUsada);

        printf("Indíce do Processo: %d\n", pm.estadoBloqueado->indiceProcesso);
        pm.estadoBloqueado = pm.estadoBloqueado->proximo;    
    }

    printf("PROCESSOS PRONTOS:\n");

    while(pm.estadoPronto) {
        i = pm.estadoPronto->indiceProcesso;

        printf("pid %d, ppid %d, prioridade %d, valor %d, tempo início %d, CPU usada até agora %d\n", pm.pcb[i].idProcesso, pm.pcb[i].idProcessoPai, pm.pcb[i].prioridade, pm.cpu.valor, pm.pcb[i].tempoInicio, pm.pcb[i].cpuUsada);

        printf("Indíce do Processo: %d\n", pm.estadoPronto->indiceProcesso);
        pm.estadoPronto = pm.estadoPronto->proximo;
    }

    printf("****************************************************************\n"); 
}