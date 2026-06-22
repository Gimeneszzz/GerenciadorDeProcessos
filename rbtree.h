#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>

typedef enum { VERMELHO, PRETO } CorNode;

// Estrutura do nó da Árvore Rubro-Negra
typedef struct Node {
    int proc_idx;           // Índice do processo no vetor principal
    float vruntime;         // Chave de ordenação (Menor vruntime fica à esquerda)
    CorNode cor;
    struct Node *esquerda;
    struct Node *direita;
    struct Node *pai;
} Node;

// Função auxiliar para criar um novo nó
static Node* criar_no_rb(int proc_idx, float vruntime) {
    Node* novo = (Node*)malloc(sizeof(Node));
    if (novo) {
        novo->proc_idx = proc_idx;
        novo->vruntime = vruntime;
        novo->cor = VERMELHO;
        novo->esquerda = novo->direita = novo->pai = NULL;
    }
    return novo;
}

// Rotação à Esquerda
static void rotacionar_esquerda(Node** raiz, Node* x) {
    Node* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) *raiz = y;
    else if (x == x->pai->esquerda) x->pai->esquerda = y;
    else x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

// Rotação à Direita
static void rotacionar_direita(Node** raiz, Node* y) {
    Node* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != NULL) x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL) *raiz = x;
    else if (y == y->pai->direita) y->pai->direita = x;
    else y->pai->esquerda = x;
    x->direita = y;
    y->pai = x;
}

// Correcção de balanceamento após inserção
static void corrigir_insercao_rb(Node** raiz, Node* z) {
    while (z != *raiz && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            Node* y = z->pai->pai->direita;
            if (y != NULL && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacionar_esquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionar_direita(raiz, z->pai->pai);
            }
        } else {
            Node* y = z->pai->pai->esquerda;
            if (y != NULL && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacionar_direita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionar_esquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

// Insere um processo na árvore RB baseado no vruntime
static void inserir_rbtree(Node** raiz, int proc_idx, float vruntime) {
    Node* z = criar_no_rb(proc_idx, vruntime);
    Node* y = NULL;
    Node* x = *raiz;

    while (x != NULL) {
        y = x;
        if (z->vruntime < x->vruntime) x = x->esquerda;
        else x = x->direita;
    }

    z->pai = y;
    if (y == NULL) *raiz = z;
    else if (z->vruntime < y->vruntime) y->esquerda = z;
    else y->direita = z;

    corrigir_insercao_rb(raiz, z);
}

// Procura e remove o nó com o MENOR vruntime (o nó mais à esquerda)
static int extrair_menor_rbtree(Node** raiz) {
    if (*raiz == NULL) return -1;

    Node* atual = *raiz;
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }

    int idx = atual->proc_idx;

    // Remoção simplificada de nós folha ou com um filho para simulação académica
    Node* filho = (atual->esquerda != NULL) ? atual->esquerda : atual->direita;
    if (filho) filho->pai = atual->pai;

    if (atual->pai == NULL) *raiz = filho;
    else if (atual == atual->pai->esquerda) atual->pai->esquerda = filho;
    else atual->pai->direita = filho;

    free(atual);

    // CORREÇÃO DO SEGFAULT: Garantir que a nova raiz seja sempre PRETA
    // Isso evita que a inserção futura procure um "avô" inexistente e acesse memória nula
    if (*raiz != NULL) {
        (*raiz)->cor = PRETO;
    }

    return idx;
}
// Liberta a memória da árvore ao fechar o programa
static void libertar_rbtree(Node* raiz) {
    if (raiz == NULL) return;
    libertar_rbtree(raiz->esquerda);
    libertar_rbtree(raiz->direita);
    free(raiz);
}

#endif