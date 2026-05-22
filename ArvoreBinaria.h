#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

No*  criarNo(int valor);
No*  inserir(No *raiz, int valor);
No*  buscar(No *raiz, int valor);
void emOrdem(No *raiz);
void preOrdem(No *raiz);
void posOrdem(No *raiz);
int  altura(No *raiz);
void destruirArvore(No *raiz);

#endif /* ARVORE_H */
