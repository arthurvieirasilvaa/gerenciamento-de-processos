void inserirNaFila(No**, int);
No *removerDaFila(No**);
void inicializaEstruturasDeDados(ProcessManager*, int, int, ArrayProgramas*);
void escalonar(ProcessManager*, int*);
void executarProximaInstrucao(ProcessManager*, ArrayProgramas**, int, int);
void desbloquearProcesso(ProcessManager*);
void criaProcessoReporter(ProcessManager);
void verificaComandoPipe(char, ArrayProgramas**, int*, ProcessManager*, int*);