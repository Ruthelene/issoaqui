#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NoAVL {
    int valor;
    int fatorBalanceamento; /* altura(dir) - altura(esq) */
    struct NoAVL *esq;
    struct NoAVL *dir;
} NoAVL;

NoAVL *avlInserir(NoAVL *raiz, int valor);
NoAVL *avlBuscar(NoAVL *raiz, int valor);
int    avlAltura(NoAVL *raiz);
void   avlDestruir(NoAVL *raiz);

#endif /* AVL_H */
