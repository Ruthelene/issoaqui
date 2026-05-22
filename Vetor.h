#ifndef VETOR_H
#define VETOR_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int  *dados;
    int   tamanho;
} Vetor;

Vetor *vetorCriar(int tamanho);
void   vetorDestruir(Vetor *v);
void   vetorInserir(Vetor *v, int indice, int valor);
int    vetorBuscaBinaria(Vetor *v, int valor);  
void   vetorOrdenar(Vetor *v);                 

#endif
