#include "AVL.h"

/* ================================================================
 * AVL.c  –  Árvore AVL (TAD)
 *
 * Convenção de fator de balanceamento (FB):
 *   FB = altura(subárvore direita) - altura(subárvore esquerda)
 *   Invariante AVL: FB ∈ {-1, 0, +1} para todo nó.
 * ================================================================ */

/* ---------- utilitários internos ---------- */

static int max2(int a, int b) { return a > b ? a : b; }

static int alturaNo(NoAVL *n) {
    return (n == NULL) ? -1 : n->fatorBalanceamento;
    /* Reaproveitamos o campo fatorBalanceamento para guardar a altura real.
       O nome do campo é herdado do .h; internamente tratamos como altura. */
}

/* Recalcula a altura armazenada no nó a partir dos filhos. */
static void atualizarAltura(NoAVL *n) {
    if (n)
        n->fatorBalanceamento = 1 + max2(alturaNo(n->esq), alturaNo(n->dir));
}

/* Fator de balanceamento real: altura(dir) - altura(esq). */
static int fb(NoAVL *n) {
    if (!n) return 0;
    return alturaNo(n->dir) - alturaNo(n->esq);
}

/* ---------- alocação ---------- */

static NoAVL *criarNoAVL(int valor) {
    NoAVL *novo = (NoAVL *) malloc(sizeof(NoAVL));
    if (!novo) { perror("malloc criarNoAVL"); exit(EXIT_FAILURE); }
    novo->valor             = valor;
    novo->fatorBalanceamento = 0; /* altura 0 (folha) */
    novo->esq               = NULL;
    novo->dir               = NULL;
    return novo;
}

/* ---------- rotações ---------- */

/*
 * Rotação simples à direita (caso esquerda-esquerda):
 *
 *       z                y
 *      / \             /   \
 *     y   T4          x     z
 *    / \      →      / \   / \
 *   x   T3          T1 T2 T3  T4
 *  / \
 * T1  T2
 */
static NoAVL *rotacaoDireita(NoAVL *z) {
    NoAVL *y = z->esq;
    NoAVL *T3 = y->dir;

    y->dir = z;
    z->esq = T3;

    atualizarAltura(z);
    atualizarAltura(y);
    return y;
}

/*
 * Rotação simples à esquerda (caso direita-direita):
 *
 *     z                   y
 *    / \                 / \
 *   T1   y      →       z   x
 *       / \            / \ / \
 *      T2   x         T1 T2 T3 T4
 *          / \
 *         T3  T4
 */
static NoAVL *rotacaoEsquerda(NoAVL *z) {
    NoAVL *y = z->dir;
    NoAVL *T2 = y->esq;

    y->esq = z;
    z->dir = T2;

    atualizarAltura(z);
    atualizarAltura(y);
    return y;
}

/* Rotação dupla esquerda-direita (caso esquerda-direita). */
static NoAVL *rotacaoEsquerdaDireita(NoAVL *z) {
    z->esq = rotacaoEsquerda(z->esq);
    return rotacaoDireita(z);
}

/* Rotação dupla direita-esquerda (caso direita-esquerda). */
static NoAVL *rotacaoDireitaEsquerda(NoAVL *z) {
    z->dir = rotacaoDireita(z->dir);
    return rotacaoEsquerda(z);
}

/* ---------- rebalanceamento ---------- */

static NoAVL *rebalancear(NoAVL *n) {
    atualizarAltura(n);
    int f = fb(n);

    /* Caso Esquerda-Esquerda */
    if (f < -1 && fb(n->esq) <= 0)
        return rotacaoDireita(n);

    /* Caso Esquerda-Direita */
    if (f < -1 && fb(n->esq) > 0)
        return rotacaoEsquerdaDireita(n);

    /* Caso Direita-Direita */
    if (f > 1 && fb(n->dir) >= 0)
        return rotacaoEsquerda(n);

    /* Caso Direita-Esquerda */
    if (f > 1 && fb(n->dir) < 0)
        return rotacaoDireitaEsquerda(n);

    return n; /* já balanceado */
}

/* ---------- inserção pública ---------- */

NoAVL *avlInserir(NoAVL *raiz, int valor) {
    if (raiz == NULL)
        return criarNoAVL(valor);

    if (valor < raiz->valor)
        raiz->esq = avlInserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = avlInserir(raiz->dir, valor);
    else
        return raiz; /* duplicata: ignora */

    return rebalancear(raiz);
}

/* ---------- busca pública ---------- */

NoAVL *avlBuscar(NoAVL *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor)
        return raiz;
    if (valor < raiz->valor)
        return avlBuscar(raiz->esq, valor);
    return avlBuscar(raiz->dir, valor);
}

/* ---------- altura pública ---------- */

int avlAltura(NoAVL *raiz) {
    return alturaNo(raiz);
}

/* ---------- destruição pública ---------- */

void avlDestruir(NoAVL *raiz) {
    if (raiz == NULL) return;
    avlDestruir(raiz->esq);
    avlDestruir(raiz->dir);
    free(raiz);
}
