typedef struct {
    char instrucao;
    int n;
    char *novo_arquivo;
}ArrayProgramas;

typedef struct {
    ArrayProgramas *ponteiroPrograma;
    int contadorPrograma;
    int valor;
    int fatiaTempo;
    int tempoAtual;
}CPU;

typedef struct {
    int idProcesso;
    int idProcessoPai;
    int *ponteiroContadorPrograma;
    int valor;
    int prioridade;
    int estado;
    int tempoInicio;
    int cpuUsada;
}TabelaPcb;

typedef struct {
    int tempo;
    CPU cpu;
    TabelaPcb* pcb;
    int* estadoPronto;
    int* estadoBloqueado;
    int* estadoExecutando;
}ProcessManager;

typedef struct no {
    ProcessManager processo;
    struct no *proximo;
}No;