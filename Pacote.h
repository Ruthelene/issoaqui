#ifndef PACOTE_H
#define PACOTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoPacote {
    int   id;
    char  dado[64];
    struct NoPacote *esq;
    struct NoPacote *dir;
} NoPacote;

NoPacote *pacoteCriar(int id, const char *dado);
NoPacote *pacoteInserir(NoPacote *raiz, int id, const char *dado);
void      pacoteMontarArquivo(NoPacote *raiz, FILE *f);
void      pacoteDestruir(NoPacote *raiz);

#endif /* PACOTE_H */
