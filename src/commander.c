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

void lerComando() {

    int tamPm = 1;
    ProcessManager *pm = (ProcessManager*) malloc(tamPm * sizeof(ProcessManager));
    int indiceSimulado = 0;

    ArrayProgramas *arr = (ArrayProgramas*) malloc(sizeof(ArrayProgramas));


    No *removido, *fila = NULL;




    // Verificando se a memória foi alocada corretamente:
    if(pm == NULL || arr == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }
    
    int fd[2];

    if(pipe(fd) == -1) {
        printf("An error occurred with pipe\n");
        exit(1);
    }

    int pid1 = fork(); // Processo commander cria o Processo manager
    int pid2;

    if(pid1 == -1) {
        printf("An error occurred with fork\n");
        exit(1);
    }

    if(pid1 != 0) {
        close(fd[0]); // Fecha a parte de leitura do pipe no Processo comander
    }

    else {
    
        close(fd[1]); // Fecha a parte de escrita do pipe no Processo manager

        pid2 = fork(); // Processo manager cria o primeiro Processo simulado

        if(pid2 == -1) {
            printf("An error occurred with fork\n");
            exit(1);
        }
    }

    // Dentro do Processo manager, inicializamos a estrutura de dados do tipo ProcessManager:
    if(pid1 == 0 && pid2 != 0) {

        armazenarPrograma(&arr, "init.txt");
        inicializaEstruturasDeDados(&pm, indiceSimulado, arr);
        imprimeArray(arr);
    }

    char comando = ' ';

    // Lemos comandos da entrada padrão enquanto o comando 'T' não for digitado:
    while(comando != 'T') {

        // Processo commander (Processo principal):
        if(pid1 != 0) {

            // Envia um sinal para o Processo manager (Processo filho) pausar a sua execução:
            kill(pid1, SIGSTOP);

            printf("Digite um comando: ");
            scanf(" %c", &comando);
            fflush(stdin);
                        
            // Envia o comando para o Processo manager (Processo filho) através do pipe:
            if(write(fd[1], &comando, sizeof(comando)) == -1) {
                exit(1);
            }

            // Envia um sinal para o Processo manager (Processo filho) continuar a sua execução:
            kill(pid1, SIGCONT);
            sleep(1); // Espera 1 segundo para ler o próximo comando
        }

        // Processo manager (Processo filho):
        else if(pid1 == 0 && pid2 != 0) {

            /*
                Lê o comando enviado pelo Processo commander (Processo principal) 
                através do pipe:
            */
            if(read(fd[0], &comando, sizeof(comando)) == -1) {
                exit(1);
            }

            verificaComandoPipe(comando, &arr, &indiceSimulado, &pm, &tamPm);      
        }

        //if(comando == 'T') {
            //break;
        //}
    }

    // Processo commander (Processo principal):
    if(pid1 != 0) {

        // Envia um sinal para o Processo manager (Processo filho) encerrar a sua execução:
        kill(pid1, SIGKILL); 
        wait(NULL); // Espera o Processo manager (Processo filho) terminar a sua execução
        close(fd[1]); // Fecha a parte de escrita do pipe
    }

    // Processo manager (Processo filho):
    else {

        close(fd[0]); // Fecha a parte de leitura do pipe
    }

    free(pm);
    free(arr);
}