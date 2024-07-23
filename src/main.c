#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "../headers/estruturasDeDados.h"
#include "../headers/commander.h"
#include "../headers/manager.h"
#include "../headers/simulado.h"

int main() {

    int tamPcb = 1; // Variável que representa o tamanho da Tabela PCB (número de Processos).
    ProcessManager pm;
    int indiceProcesso = 0; // Variável usada para "acompanhar" qual o Processo atual

    ArrayProgramas *arr = (ArrayProgramas*) malloc(1 * sizeof(ArrayProgramas));

    // Verificando se a memória foi alocada corretamente:
    if(arr == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    armazenarPrograma(&arr, "init.txt"); // Armazena o programa do arquivo init.txt
    inicializaEstruturasDeDados(&pm, tamPcb, indiceProcesso, arr);
    executarCommander(pm, tamPcb, indiceProcesso, arr);

    // Liberamos a memória alocada dinamicamente:
    free(arr);

    return 0;
}