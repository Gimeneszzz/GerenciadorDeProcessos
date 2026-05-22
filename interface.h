#ifndef INTERFACE_H
#define INTERFACE_H

#include "processos.h"

// Protótipos das funções da interface gráfica de terminal
void limpar_tela();
void mostrar_cabecalho_simulador(const char* nome_algoritmo, int quantum);
void animar_execucao(int tempo, int indice_cpu, const Processo processos[], int n, const char* nome_algoritmo);
void imprimir_relatorio_colorido(const Processo processos[], int n);

#endif