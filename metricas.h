#ifndef METRICAS_H
#define METRICAS_H

#include <stdio.h>
#include <time.h>

double metricasAgora(void);

double metricasMedia(double *tempos, int n);


void metricasImprimirTabela(const char *titulo,
                             double     *tempos,
                             int         n,
                             double      media);


long metricasMemoriaKB(void);

#endif 
