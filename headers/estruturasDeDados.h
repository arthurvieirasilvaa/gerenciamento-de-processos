typedef struct {
    char instrucao;
    int n;
    char *novo_arquivo;
}ArrayProgramas;

typedef struct no {
    int indiceProcesso;
    struct no *proximo;
}No;

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
    char estado; // E (Executando), P (Pronto) ou B (Bloqueado)
    int tempoInicio;
    int cpuUsada;
}TabelaPcb;

typedef struct {
    int tempo;
    CPU cpu;
    TabelaPcb* pcb;
    No *estadoPronto;
    No *estadoBloqueado;
    No *estadoExecutando;
}ProcessManager;