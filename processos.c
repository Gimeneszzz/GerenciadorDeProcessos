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
    //Linha dos processos
    Processo processos[MAX_PROCESSOS];
    int total_processos = 0;

    ///Garantir que foi aberto
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    //Ler a primeira linha do arquivo
    //Verifica arquivo e se tem o formato esperado, algoritmo e quantum separados por "|"
    //garantindo que o programa só continue se o cabeçalho do arquivo estiver no formato correto, 
    //se for diferente de 2, indica que o formato do cabeçalho é inválido, 
    //e o programa exibe uma mensagem de erro e termina a execução para evitar comportamentos 
    //inesperados devido a um formato de arquivo incorreto
    if (fscanf(arquivo, " %31[^|]|%d", algoritmo, &quantum) != 2) {
        printf("Cabeçalho inválido.\n");
        fclose(arquivo);
        return 1;
    }

    //Ler os processos
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
