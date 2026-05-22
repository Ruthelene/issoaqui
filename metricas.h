#ifndef METRICAS_H
#define METRICAS_H

#include <stdio.h>
#include <time.h>

/* Retorna tempo atual em segundos com precisão de nanosegundos. */
double metricasAgora(void);

/* Calcula a média de um vetor de doubles de tamanho n. */
double metricasMedia(double *tempos, int n);

/* Imprime tabela simples: índice | tempo (s) */
void metricasImprimirTabela(const char *titulo,
                             double     *tempos,
                             int         n,
                             double      media);

/* Retorna uso de memória RSS do processo em KB (-1 se indisponível). */
long metricasMemoriaKB(void);

#endif /* METRICAS_H */
