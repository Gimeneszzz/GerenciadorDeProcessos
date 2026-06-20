#ifndef PROCESSOS_H
#define PROCESSOS_H

#define MAX_PROCESSOS 100

typedef struct {
    char pid[10];
    int tempo_criacao;
    int tempo_execucao_total;
    int tempo_restante;
    int prioridade; // prioridade ou quantidade de bilhetes
    float vruntime; // usado no CFS
    int tempo_espera; // tempo em estado pronto
    int tempo_conclusao;
    int na_cpu;
} Processo;

void escalonar_alternancia(Processo processos[], int n, int quantum);
void escalonar_prioridade(Processo processos[], int n, int quantum);
void escalonar_loteria(Processo processos[], int n, int quantum);
void escalonar_cfs(Processo processos[], int n, int quantum);

#endif