#ifndef PROCESSOS_H
#define PROCESSOS_H

#define MAX_PROCESSOS 100
#define MAX_MOLDURAS 10
#define MAX_ACESSOS 1000
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
    //Campos para os arquivos de entrada
    int qtde_memoria; // Quantidade de de memória exigida pelo processo
    int sequencia_acessos[MAX_ACESSOS]; // Array que guarda: 1, 2, 3, 3, 4, 5...
    int total_acessos_sequencia;// Tamanho da sequência lida do arquivo
    int acesso_atual;// Índice de qual página o processo vai pedir agora
    //Campos para a memória física(Molduras)
    int paginas[MAX_MOLDURAS]; // páginas que o processo acessa
    int tempo_carregamento[MAX_MOLDURAS]; // Necessário para FIFO e LRU
    int ponteiro_fifo; // Índice circular para o FIFO
    int n_paginas_ocupadas; // Quantas molduras estão em uso
} Processo;

void escalonar_alternancia(Processo processos[], int n, int quantum);
void escalonar_prioridade(Processo processos[], int n, int quantum);
void escalonar_loteria(Processo processos[], int n, int quantum);
void escalonar_cfs(Processo processos[], int n, int quantum);

#endif