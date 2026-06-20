#ifndef MEMORIA_H
#define MEMORIA_H

// Estrutura para os resultados
typedef struct {
    int trocas_fifo;
    int trocas_lru;
    int trocas_nfu;
    int trocas_otimo;
} ResultadosMemoria;

// Protótipos - VOCÊS SÓ IMPLEMENTAM O QUE LHES CABE
int simular_fifo(int pid, int pagina_acessada, int* molduras, int tamanho_molduras);
int simular_lru(int pid, int pagina_acessada, int* molduras, int tamanho_molduras);
int simular_nfu(int pid, int pagina_acessada, int* molduras, int tamanho_molduras);
int simular_otimo(int pid, int pagina_acessada, int* molduras, int tamanho_molduras, int* futuro_acessos);

#endif