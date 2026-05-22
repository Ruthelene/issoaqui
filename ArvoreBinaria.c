#include "ArvoreBinaria.h"

/* ---------- alocação ---------- */

No *criarNo(int valor) {
    No *novo = (No *) malloc(sizeof(No));
    if (!novo) { perror("malloc criarNo"); exit(EXIT_FAILURE); }
    novo->valor = valor;
    novo->esq   = NULL;
    novo->dir   = NULL;
    return novo;
}

/* ---------- inserção ---------- */

No *inserir(No *raiz, int valor) {
    if (raiz == NULL)
        return criarNo(valor);
    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    /* valor == raiz->valor: ignora duplicatas */
    return raiz;
}

/* ---------- busca ---------- */

No *buscar(No *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor)
        return raiz;
    if (valor < raiz->valor)
        return buscar(raiz->esq, valor);
    return buscar(raiz->dir, valor);
}

/* ---------- caminhamentos ---------- */

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        emOrdem(raiz->dir);
    }
}

void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->valor);
    }
}

/* ---------- altura ---------- */

int altura(No *raiz) {
    if (raiz == NULL) return -1;
    int he = altura(raiz->esq);
    int hd = altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}

/* ---------- destruição ---------- */

void destruirArvore(No *raiz) {
    if (raiz == NULL) return;
    destruirArvore(raiz->esq);
    destruirArvore(raiz->dir);
    free(raiz);
}
