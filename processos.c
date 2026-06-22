#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "processos.h"
#include "interface.h"

// Antes era necessário garantir que o quantum fosse positivo para evitar loops infinitos,
// mas agora isso é tratado diretamente na função de simulação, então essa verificação 
// foi removida daqui. O quantum é garantido como positivo dentro da função simular(), 
// onde é realmente necessário para o controle do tempo de execução dos processos.
//#define MAX_PROCESSOS 100

// Função para verificar se duas strings são iguais, ignorando diferenças de maiúsculas e minúsculas, 
// para facilitar a comparação do nome do algoritmo lido do arquivo com os algoritmos suportados
static int strings_iguais_ignore_case(const char *a, const char *b) {
    while (*a && *b) {// Enquanto ambos os caracteres não forem nulos, compara-os ignorando o caso
        // Usa tolower para converter ambos os caracteres para minúsculas antes de comparar, 
        // garantindo que "Alternancia", "alternancia" e "ALTERNANCIA" sejam considerados iguais
        // Verifica se os caracteres atuais são diferentes, e se forem, retorna 0 (falso),
        // indicando que as strings não são iguais
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }

        // Avança para o próximo caractere em ambas as strings
        a++;
        b++;
    }
    // Se ambos os caracteres atuais são nulos, isso significa que chegamos ao final de ambas as strings e 
    // todas as comparações anteriores foram iguais, então retorna 1 (verdadeiro), indicando que as strings
    // são iguais ignorando o caso
    return *a == '\0' && *b == '\0';
}

int main(void){
    //Primeira parte: Abrir o arquivo gerenciador.txt
    ///Abrir o arquivo para leitura
    FILE *arquivo = fopen("gerenciador.txt", "r");
    //Buffer para o texto que foi lido no arquivo
    //Para o nome do algoritmo, que tem um limite de 31 caracteres mais o caractere nulo,
    //garantindo que não haja estouro de buffer ao ler o nome do algoritmo do arquivo
    char algoritmo[32];
    //Variável para guardar a fatia de tempo 
    int quantum = 0;

    //Variável para o nome da política de memória
    char politica_memoria[16];
    //Variável para os algortimos de memória, que serão lidos do arquivo e comparados com os algoritmos suportados
    int tamanho_memoria = 0;
    int tamanho_paginas = 0;
    int percentual_alocacao = 0;

    //Linha dos processos
    Processo processos[MAX_PROCESSOS];
    int total_processos = 0;

    ///Garantir que foi aberto
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    //Ler a primeira linha do arquivo
    /*VERIFICAÇÃO DO ARQUIVO ANTIGO
    //Verifica arquivo e se tem o formato esperado, algoritmo e quantum separados por "|"
    //garantindo que o programa só continue se o cabeçalho do arquivo estiver no formato correto, 
    //se for diferente de 2, indica que o formato do cabeçalho é inválido, 
    //e o programa exibe uma mensagem de erro e termina a execução para evitar comportamentos 
    //inesperados devido a um formato de arquivo incorreto
    if (fscanf(arquivo, " %31[^|]|%d", algoritmo, &quantum) != 2) {
        printf("Cabeçalho inválido.\n");
        fclose(arquivo);
        return 1;
    }*/
    //Nova verificação
    if (fscanf(arquivo, " %31[^|]|%d|%15[^|]|%d|%d|%d", 
               algoritmo, &quantum, politica_memoria, 
               &tamanho_memoria, &tamanho_paginas, &percentual_alocacao) != 6) {
        printf("Cabeçalho inválido.\n");
        fclose(arquivo);
        return 1;
    }

    //Ler os processos
    /*LEITURA NO FORMATO ANTIGO
    //Enquanto o número de processos lidos for menor que o máximo permitido 
    //e a leitura do arquivo for como esperado(tempo de criação, PID, tempo total de execução e prioridade) == 4,
    //o programa continua lendo os processos do arquivo, preenchendo a estrutura
    while (total_processos < MAX_PROCESSOS &&
        fscanf(arquivo, " M%d|P%9[^|]|%d|%d",
                &processos[total_processos].tempo_criacao,
                processos[total_processos].pid,
                &processos[total_processos].tempo_execucao_total,
                &processos[total_processos].prioridade) == 4) {
        //Pega o tempo_restante do processo, que inicialmente é igual ao tempo total de execução, 
        processos[total_processos].tempo_restante = processos[total_processos].tempo_execucao_total;
        //e também inicializa o vruntime, o tempo de espera, 
        processos[total_processos].vruntime = 0.0f;
        processos[total_processos].tempo_espera = 0;
        //o tempo de conclusão e o estado de na_cpu para cada processo lido do arquivo,
        //preparando-os para a simulação dos algoritmos de escalonamento,
        //onde esses campos serão atualizados conforme os processos são executados e preemptados
        processos[total_processos].tempo_conclusao = -1;
        processos[total_processos].na_cpu = 0;
        // INICIALIZAÇÃO DA MEMÓRIA
        processos[total_processos].n_paginas_ocupadas = 0; // Começa sem nada na memória
        processos[total_processos].ponteiro_fifo = 0;      // Começa apontando para a primeira moldura
       // Laço para iniciar as páginas e tempos de carregamento do processo,
       // definindo as páginas como -1 para indicar que estão vazias,
        for (int i = 0; i < MAX_MOLDURAS; i++) {
            processos[total_processos].paginas[i] = -1;           // -1 significa slot vazio
            processos[total_processos].tempo_carregamento[i] = 0; // Inicia tempo zerado
        }

        total_processos++;
    }*/
    //Leitua no formato novo
    while (total_processos < MAX_PROCESSOS &&
           fscanf(arquivo, " %d|%9[^|]|%d|%d|%d| ", 
                  &processos[total_processos].tempo_criacao,
                  processos[total_processos].pid,
                  &processos[total_processos].tempo_execucao_total,
                  &processos[total_processos].prioridade,
                  &processos[total_processos].qtde_memoria) == 5) {
        
        //CÁLCULO DO LIMITE DE MOLDURAS
        //Descobre quantas páginas o processo precisa (arredondando pra cima)
        int paginas_virtuais = processos[total_processos].qtde_memoria / tamanho_paginas;
        if (processos[total_processos].qtde_memoria % tamanho_paginas != 0) {
            paginas_virtuais++;
        }
        //Calcula quantas molduras ele tem direito na RAM
        processos[total_processos].limite_molduras = (paginas_virtuais * percentual_alocacao) / 100;
        //Garante que ele tenha pelo menos 1 moldura para não travar o sistema
        if (processos[total_processos].limite_molduras < 1) {
            processos[total_processos].limite_molduras = 1;
        }
        // Inicializações antigas do escalonamento
        processos[total_processos].tempo_restante = processos[total_processos].tempo_execucao_total;
        processos[total_processos].vruntime = 0.0f;
        processos[total_processos].tempo_espera = 0;
        processos[total_processos].tempo_conclusao = -1;
        processos[total_processos].na_cpu = 0;
        
        // Inicializações novas de controle da sequência
        processos[total_processos].total_acessos_sequencia = 0;
        processos[total_processos].acesso_atual = 0; // Vai começar lendo o índice 0

        // Inicialização da memória física do processo
        processos[total_processos].n_paginas_ocupadas = 0; 
        processos[total_processos].ponteiro_fifo = 0;      
        for (int i = 0; i < MAX_MOLDURAS; i++) {
            processos[total_processos].paginas[i] = -1;           
            processos[total_processos].tempo_carregamento[i] = 0; 
            processos[total_processos].contador_nfu[i] = 0; // Inicializa contador NFU
        }

        // Lê o restante da linha (os números com espaço) até a quebra de linha
        char linha_acessos[2048];
        if (fgets(linha_acessos, sizeof(linha_acessos), arquivo) != NULL) {
            // O strtok quebra a string toda vez que encontra um espaço ou quebra de linha
            char *token = strtok(linha_acessos, " \r\n");
            
            // Enquanto houver números na linha, converte (atoi) e guarda no array
            while (token != NULL && processos[total_processos].total_acessos_sequencia < MAX_ACESSOS) {
                int index = processos[total_processos].total_acessos_sequencia;
                processos[total_processos].sequencia_acessos[index] = atoi(token);
                processos[total_processos].total_acessos_sequencia++;
                
                token = strtok(NULL, " \r\n"); // Pega o próximo número
            }
        }

        total_processos++;
    }

    fclose(arquivo);

    // Verificar se o algoritmo é válido
    if (total_processos == 0) {
        printf("Nenhum processo encontrado no arquivo.\n");
        return 1;
    }
    // Comparar o algoritmo lido com os algoritmos suportados
    if (quantum <= 0) {
        quantum = 1; // Valor padrão para quantum
    }

    // Dando início à parte gráfica
    // Chama a tela de abertura colorida e animada
    mostrar_cabecalho_simulador(algoritmo, quantum);

    //printf("Algoritmo: %s | Quantum: %d\n", algoritmo, quantum);
    //printf("Processos carregados: %d\n\n", total_processos);

    // Iniciando a semente para a loteria, garantindo que os resultados sejam diferentes a cada execução do programa, 
    //Para simular a aleatoriedade dos sorteios de bilhetes no algoritmo de loteria
    srand((unsigned int)time(NULL));

    // Chama o algoritmo correspondente
    if (strings_iguais_ignore_case(algoritmo, "Alternancia") ||
        strings_iguais_ignore_case(algoritmo, "RoundRobin")) {
        escalonar_alternancia(processos, total_processos, quantum);
    } else if (strings_iguais_ignore_case(algoritmo, "Prioridade")) {
        escalonar_prioridade(processos, total_processos, quantum);
    } else if (strings_iguais_ignore_case(algoritmo, "Loteria")) {
        escalonar_loteria(processos, total_processos, quantum);
    } else if (strings_iguais_ignore_case(algoritmo, "CFS")) {
        escalonar_cfs(processos, total_processos, quantum);
    } else {
        limpar_tela(); // Limpa a tela para mostrar a mensagem de erro de forma mais clara
        printf("Algoritmo desconhecido: %s\n", algoritmo);
        return 1;
    }

    return 0;
}
