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