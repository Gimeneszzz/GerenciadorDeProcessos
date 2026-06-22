# Simulador de Escalonamento de Processos (Sistemas Operacionais)

Este projeto implementa um simulador de escalonamento de processos preemptivo e gerenciamento de memória virtual em C, desenvolvido como requisito para a disciplina de Sistemas Operacionais. O simulador processa um arquivo de configuração e simula a execução na CPU utilizando quatro algoritmos de escalonamento, comparando simultaneamente quatro políticas de substituição de página.

## Integrantes do Grupo
* [Lara Letittja Sague Lopez Guardiola Velloso/150886]
* [Gustavo Gimenes/Matrícula 2]

## Algoritmos Implementados
### Escalonamento de Processos
1. **Alternância Circular (Round Robin)**
2. **Prioridade**
3. **Loteria**
4. **CFS (Completely Fair Scheduler)**

### Substituição de Páginas (Memória)
1. **FIFO** (First-In, First-Out)
2. **LRU** (Least Recently Used)
3. **NFU** (Not Frequently Used)
4. **Ótimo**

## Estrutura de Arquivos
* `processos.h`: Definição da estrutura do Bloco de Controle de Processo (PCB).
* `processos.c`: Parser de entrada, validação e inicialização do sistema.
* `algoritmoDeEscalonamento.c`: Implementação da lógica de escalonamento e loop principal (relogio).
* `memoria.c` / `memoria.h`: Implementação dos algoritmos de substituição de página.
* `interface.h` / `interface.c`: Módulos da interface gráfica animada via CLI.
* `gerenciador.txt`: Arquivo de entrada com as configurações globais e lista de processos.

## Como Compilar e Executar (Linux/Unix)
O sistema foi desenvolvido utilizando bibliotecas padrão da linguagem C (ANSI C), garantindo alta portabilidade.

1. Abra o terminal e navegue até a pasta do projeto.
2. Para compilar todos os módulos juntos, execute o comando:
   ```bash
   gcc *.c -o simulador

3. Para executar o simulador, digite:
   ```bash
   ./simulador

## Como configurar o arquivo gerenciador.txt
O simulador lê os dados estritamente do arquivo gerenciador.txt localizado no mesmo diretório do executável. O formato deve seguir estritamente o padrão abaixo:

Linha 1: Algoritmo|FatiaDeTempo|PolíticaMemória|TamanhoMemória|TamanhoPáginas|PercentualAlocação

Linhas Seguintes: TempoCriação|PID|TempoExecução|Prioridade|QuantidadeMemória|SequênciaAcessos

Por exemplo:
alternancia|5|local|65536|512|75
0|1|15|59|4096|1 2 3 3 4 5 5 6 1 3 2 6 7 7 8
4|2|24|32|2048|1 2 2 2 3 4 3 4 4 4 2 3 2 1 3 2 1 2 2 3 4 3 2 2


Projeto desenvolvido para a disciplina de Sistemas Operacionais - 2026.