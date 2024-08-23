#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "../headers/estruturasDeDados.h"
#include "../headers/commander.h"
#include "../headers/manager.h"
#include "../headers/simulado.h"

void lerComando(int pid, int *fd, ArrayProgramas *arr, int indiceProcesso, ProcessManager *pm, int tamPcb) {
    
    char comando = ' ';
    
    // Lemos comandos da entrada padrão enquanto o comando 'T' não for digitado:
    while(comando != 'T') {

        // Processo commander (Processo principal):
        if(pid != 0) {
            scanf(" %c", &comando);
            fflush(stdin);
                                    
            // Envia o comando para o Processo manager (Processo filho) através do pipe:
            if(write(fd[1], &comando, sizeof(comando)) == -1) {
                exit(1);
            }

            sleep(1); // Espera 1 segundo para ler o próximo comando
        }

        // Processo manager (Processo filho):
        else {

            /*
                Lê o comando enviado pelo Processo commander (Processo principal) 
                através do pipe:
            */
            if(read(fd[0], &comando, sizeof(comando)) == -1) {
                exit(1);
            }
                
            verificaComandoPipe(comando, &arr, &indiceProcesso, pm, &tamPcb);

            // Realiza o escalonamento dos processos:
            escalonar(pm, &indiceProcesso);
            
        }
    }
}

// Função utilizada para ler o comando que será digitado pelo usuário na entrada padrão:
void executarCommander(ProcessManager pm, int tamPcb, int indiceProcesso, ArrayProgramas *arr) {
    
    int fd[2];

    if(pipe(fd) == -1) {
        printf("Ocorreu um erro ao criar o pipe\n");
        exit(1);
    }

    int pid = fork(); // Processo commander cria o Processo manager

    if(pid == -1) {
        printf("Ocorreu um erro ao realizar um fork\n");
        exit(1);
    }

    if(pid != 0) {
        close(fd[0]); // Fecha a parte de leitura do pipe no Processo comander
    }

    else {
        close(fd[1]); // Fecha a parte de escrita do pipe no Processo manager
    }
    
    pm.pcb[indiceProcesso].estado = 'E'; // O Processo simulado agora está executando
    // Colocamos o índice desse Processo simulaod no array de estadoExecutando:
    inserirNaFila(&pm.estadoExecutando, indiceProcesso);

    lerComando(pid, fd, arr, indiceProcesso, &pm, tamPcb);

    // Processo commander (Processo principal):
    if(pid != 0) {
        wait(NULL);
        close(fd[1]); // Fecha a parte de escrita do pipe
    }

    // Processo manager (Processo filho):
    else {
        close(fd[0]); // Fecha a parte de leitura do pipe
    }
}