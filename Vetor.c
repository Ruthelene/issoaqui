#include "Vetor.h"

Vetor *vetorCriar(int tamanho) {
    Vetor *v = (Vetor *) malloc(sizeof(Vetor));
    if (!v) { perror("malloc Vetor"); exit(EXIT_FAILURE); }
    v->dados = (int *) malloc(sizeof(int) * tamanho);
    if (!v->dados) { perror("malloc dados"); exit(EXIT_FAILURE); }
    v->tamanho = tamanho;
    return v;
}

void vetorDestruir(Vetor *v) {
    if (!v) return;
    free(v->dados);
    free(v);
}

void vetorInserir(Vetor *v, int indice, int valor) {
    if (indice < 0 || indice >= v->tamanho) return;
    v->dados[indice] = valor;
}

static int cmpInt(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void vetorOrdenar(Vetor *v) {
    qsort(v->dados, v->tamanho, sizeof(int), cmpInt);
}

int vetorBuscaBinaria(Vetor *v, int valor) {
    int esq = 0, dir = v->tamanho - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        if (v->dados[meio] == valor) return meio;
        if (v->dados[meio] < valor)  esq = meio + 1;
        else                          dir = meio - 1;
    }
    return -1;
}
