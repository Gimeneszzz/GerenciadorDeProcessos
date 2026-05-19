# Simulador de Escalonamento de Processos (Sistemas Operacionais)

Este projeto implementa um simulador de escalonamento de processos preemptivo em C, desenvolvido como requisito para a disciplina de Sistemas Operacionais. O simulador lê uma lista de processos a partir de um arquivo de texto e simula a sua execução na CPU utilizando quatro algoritmos diferentes.

## Integrantes do Grupo
* [Lara Letittja Sague Lopez Guardiola Velloso/150886]
* [Nome 2/Matrícula 2]

## Algoritmos Implementados
1. **Alternância Circular (Round Robin)**
2. **Prioridade**
3. **Loteria**
4. **CFS (Completely Fair Scheduler)**

## Estrutura de Arquivos
* `processos.h`: Definição da estrutura do Bloco de Controle de Processo (PCB).
* `processos.c`: Função principal (main), responsável pelo parser (leitura do arquivo), validação e inicialização do sistema.
* `algoritmoDeEscalonamento.c`: Implementação da lógica matemática de escolha e do relógio (loop) da simulação.
* `interface.h` / `interface.c`: Módulos responsáveis pela interface gráfica animada via CLI (Command Line Interface).
* `gerenciador.txt`: Arquivo de banco de dados com as configurações do escalonador e a lista de processos.

## Como Compilar e Executar (Linux/Unix)

O sistema foi desenvolvido utilizando bibliotecas padrão da linguagem C (ANSI C), garantindo alta portabilidade. 

1. Abra o terminal e navegue até a pasta do projeto.
2. Para compilar todos os módulos juntos, execute o comando:
   ```bash
   gcc processos.c algoritmoDeEscalonamento.c interface.c -o simulador
3. Para executar o simulador, digite:
./simulador

## Como configurar o arquivo gerenciador.txt
O simulador lê os dados estritamente do arquivo gerenciador.txt, que deve estar no mesmo diretório do executável. Utilize o arquivo base como o disponível no repositório ou faça o seu. O arquivo deve seguir o formato abaixo:

Linha 1: Algoritmo|FatiaDeTempo
Linhas Seguintes: M[TempoDeCriação]|P[ID]|TempoDeExecução|PrioridadeOuBilhetes

Exemplo: 
Alternancia|10
M0|P1|8|50
M0|P2|50|99
M2|P3|37|10
M4|P4|15|70
M4|P5|100|63