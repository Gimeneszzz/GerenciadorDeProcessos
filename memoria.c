#include <stdio.h>
#include "memoria.h"
/*Letittja
FIFO: Gerencie uma fila simples ou apenas um índice circular que aponta para a 
próxima moldura a ser substituída.
LRU: Você precisará manter um "timestamp" ou uma lista ordenada. 
Toda vez que uma página for acessada, atualizeo "tempo" dela. 
Na hora da troca, percorra as molduras e remova a que tiver o timestamp 
mais antigo.
DESENVOLVER O simular_fifo e simular_lru*/

/*Gustavo
NFU: Deve manter um contador de acessos para cada página presente na memória.
Ótimo: Este é o mais chatinho. Ele precisa receber o "futuro" da execução 
(a sequência restante). Para facilitar, ele pode ler a sequência de acessos
 completa no início da simulação e guardá-la em um vetor global de 
 "futuros acessos".
 DESENVOLVER O simular_nfu e simular_otimo*/

/*ATENÇÃO!!! Implementar as funções apenas com um return 0;*/

//Declarando as funções como int para não fazer overhead(excesso de peso)
//Usar struct nesse caso é desnecessário, já que só precisamos saber se houve ou não troca

//Variável declarada como global e static para saber qual o tempo global da simulação,
//para atualizar os tempos de carregamento das páginas
//RELÓGIO GLOBAL PARA A SIMULAÇÃO DE LRU
static int tempo_global = 0;

int gerenciar_acesso(Processo *P, int pagina_acessada, int politica){
    //Incrementando o tempo global a cada acesso para o LRU
    tempo_global++; 

    //Primeira passo: Verificar se está na memória
    //percorrendo o array de páginas do processo
    for (int i = 0; i < P -> n_paginas_ocupadas; i++) {
        //Verificando se a página está na memória
        if (P -> paginas[i] == pagina_acessada) {
            //Se estiver na memória, atualiza o tempo de carregamento da página acessada
            if (politica == POLITICA_LRU) { 
                P->tempo_carregamento[i] = tempo_global;
            }
            //Se for FIFO, não precisa atualizar o tempo de carregamento, pois a ordem é fixa.
            //Tanto para o FIFO quanto para o LRU, se a página já estiver na memória, 
            //não há troca, então retorna 0
            return 0; // Acerto, sem troca
        }
    }

    //Segundo passo: Verificar se está cheio
    if (P->n_paginas_ocupadas == MAX_MOLDURAS){
        //Se cheio ele chama a respectiva politica
        if (politica == POLITICA_FIFO) {
            return simular_fifo(P, pagina_acessada);
        } else if (politica == POLITICA_LRU) {
            return simular_lru(P, pagina_acessada);
        }

    //Terceiro passo: memória livre, adiciona na pagina    
    }else {
        int pos = P->n_paginas_ocupadas;
        P->paginas[pos] = pagina_acessada;
        //Atualiza o tempo de carregamento da página inserida,
        //que é o tempo global atual para o LRU, enquanto o FIFO é usada para manter a consistência dos dados
        P -> tempo_carregamento[pos] = tempo_global;
        P -> n_paginas_ocupadas++;
        return 0; // Inserido, sem troca
    }    
    
    return 0;
} 
// Função FIFO: usa o ponteiro circular da struct
int simular_fifo(Processo *P, int pagina_acessada) {    
    P->paginas[P->ponteiro_fifo] = pagina_acessada;
    P->ponteiro_fifo = (P->ponteiro_fifo + 1) % MAX_MOLDURAS;
    
    return 1; // Página substituída, houve troca
}

// Função LRU: procura a página com menor timestamp
int simular_lru(Processo *P, int pagina_acessada) {
    int indice_mais_antigo = 0;
    int menor_tempo = P->tempo_carregamento[0];

    // Procura a página que não é usada há mais tempo
    for (int i = 1; i < MAX_MOLDURAS; i++) {
        if (P->tempo_carregamento[i] < menor_tempo) {
            menor_tempo = P->tempo_carregamento[i];
            indice_mais_antigo = i;
        }
    }
    
    P->paginas[indice_mais_antigo] = pagina_acessada;
    P->tempo_carregamento[indice_mais_antigo] = tempo_global;
    
    return 1; // Página substituída, houve troca
}