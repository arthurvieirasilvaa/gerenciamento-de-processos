#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "headers/estruturasDeDados.h"
#include "headers/commander.h"
#include "headers/manager.h"
#include "headers/simulado.h"

void lerComando() {

    /*
        Variável que especifica o tamanho de uma variável do tipo ProcessManager,
        isto é, o número de processos que estão criados (executando ou bloqueados):
    */
    int tamPm = 1; 
    ProcessManager *pm = (ProcessManager*) malloc(tamPm * sizeof(ProcessManager));

    // Verificando se a memória foi alocada corretamente:
    if(pm == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    /*
    arr irá armazenar o programa de todos os processos simulados durante a execução:
        arr[tamPm] => armazena todo o arquivo de um programa simulado;
        arr[tamPm][tamProg] => armazena cada instrução (linha do arquivo) de um 
        programa simulado.
    */

    ArrayProgramas **arr = (ArrayProgramas**) malloc(1 * sizeof(ArrayProgramas*));
    arr[tamPm-1] = (ArrayProgramas*) malloc(1 * sizeof(ArrayProgramas));

    // Verificando se a memória foi alocada corretamente:
    if(arr == NULL || arr[tamPm-1] == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    armazenarPrograma(&arr, tamPm); 

    printf("%c %d\n", arr[0][0].instrucao, arr[0][0].n);

    imprimeArray(arr, tamPm);
    inicializaEstruturasDeDados(&pm, tamPm, arr);
    
    printf("%c %d\n", (*pm[0].cpu.ponteiroPrograma).instrucao, (*pm[0].cpu.ponteiroPrograma).n);

    int fd[2];

    if(pipe(fd) == -1) {
        exit(1);
    }

    int pid = fork();

    if(pid == -1) {
        exit(1);
    }

    if(pid != 0) {
        close(fd[0]); // Fecha a parte de leitura do pipe
    }

    else {
        close(fd[1]); // Fecha a parte de escrita do pipe
    }

    char comando = ' ';

    // Lemos comandos da entrada padrão enquanto o comando 'T' não for digitado:
    do {

        // Processo commander (Processo principal):
        if(pid != 0) {

            // Envia um sinal para o Processo manager (Processo filho) pausar a sua execução:
            kill(pid, SIGSTOP);

            printf("Digite um comando: ");
            scanf(" %c", &comando);
            fflush(stdin);
                    
            // Envia o comando para o Processo manager (Processo filho) através do pipe:
            if(write(fd[1], &comando, sizeof(comando)) == -1) {
                exit(1);
            }

            // Envia um sinal para o Processo manager (Processo filho) continuar a sua execução:
            kill(pid, SIGCONT);
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

            verificaComandoPipe(comando, &pm, &tamPm);

            int pid2 = fork();

            if(pid2 == -1) {
                exit(1);
            }

            // Processo manager (Processo principal):
            //if(pid2 != 0) {
        
                //kill(pid2, SIGCONT);

                //verificaComandoPipe(comando, &pm, &tamPm, &arr, &tamProg);
                //kill(pid2, SIGCONT);
                //wait(NULL);
            //}
    
            // Processo simulado (Processo filho):
            if(pid2 == 0) {
    
                // Lê o programa do primeiro Processo simulado:
                //armazenarPrograma(&arr, tamPm, &tamProg); 
                //imprimeArray(arr, tamPm, tamProg);
            }
        }
    }while(comando != 'T');

    // Processo commander (Processo principal):
    if(pid != 0) {

        // Envia um sinal para o Processo manager (Processo filho) encerrar a sua execução:
        kill(pid, SIGKILL); 
        wait(NULL); // Espera o Processo manager (Processo filho) terminar a sua execução
        close(fd[1]); // Fecha a parte de escrita do pipe
    }

    // Processo manager (Processo filho):
    else {

        close(fd[0]); // Fecha a parte de leitura do pipe
    }
}