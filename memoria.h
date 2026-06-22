#ifndef MEMORIA_H
#define MEMORIA_H

#include "processos.h"//Necessário para acessar a struct Processo
// Estrutura para os resultados

//Declarando as politicas através de nomes amigáveis,
// para facilitar a leitura do código e evitar erros de digitação
typedef enum{
    POLITICA_FIFO,
    POLITICA_LRU,
    POLITICA_NFU,
    POLITICA_OTIMO
}PoliticaMemoria;

//Estrutura para armazenar os resultados de trocas de páginas para cada política,
//que será usada para comparar o desempenho das políticas de gerenciamento de memória
typedef struct {
    int trocas_fifo;
    int trocas_lru;
    int trocas_nfu;
    int trocas_otimo;
} ResultadosMemoria;

// Protótipos das funções de simulação
// Lógica alterada para receber o processo e a página acessada, e retornar se houve troca ou não
//Protótipo principal para todos os algoritmos
int gerenciar_acesso(Processo *P,int pagina_acessada,int politica,int* sequencia_futura,int tamanho_futuro);
// Cada algoritmo acessará p->paginas, p->tempo_carregamento e outros campos conforme necessário
int simular_fifo(Processo *p, int pagina_acessada);
int simular_lru(Processo *p, int pagina_acessada);
int simular_nfu(Processo *p, int pagina_acessada);
int simular_otimo(Processo *p,int pagina_acessada,int* sequencia_futura,int tamanho_futuro);

#endif