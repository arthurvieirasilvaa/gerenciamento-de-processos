#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "headers/estruturasDeDados.h"
#include "headers/reporter.h"

void imprimeEstadoAtual(ProcessManager *pm, int tamPm) {

    printf("\n****************************************************************\n");
    printf("Estado sistema\n");
    printf("****************************************************************\n\n");

    printf("CONTADOR DE PROGRAMA: %d\n", pm[tamPm-1].cpu.contadorPrograma);

    for(int i = 0; i < tamPm; i++) {

        printf("TEMPO ATUAL: %d\n", pm[i].cpu.tempoAtual);
        printf("PROCESSO EXECUTANDO: \n");
        printf("BLOQUEADO: \n");
        printf("Fila processos bloqueados: \n");
        printf("PROCESSOS PRONTOS: \n");
        printf("****************************************************************\n");
    }
}