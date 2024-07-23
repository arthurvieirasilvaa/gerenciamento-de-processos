void atualizaVariavel(ProcessManager*, int);
void somaVariavel(ProcessManager*, int);
void subtraiVariavel(ProcessManager*, int);
void bloqueiaProcessoSimulado(ProcessManager*, int*); 
void terminaProcessoSimulado(ProcessManager*, int);
void criaNovoProcessoSimulado(ProcessManager*, int*, int, int);
void armazenarPrograma(ArrayProgramas**, char*);
void substituiPrograma(ArrayProgramas**, ProcessManager*, int);
void verificarInstrucao(char instrucao, ProcessManager*, int*, ArrayProgramas**, int, int);
