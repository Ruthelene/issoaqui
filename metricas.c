#include "metricas.h"
#include <time.h>
#include <string.h>

double metricasAgora(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

double metricasMedia(double *tempos, int n) {
    if (n <= 0) return 0.0;
    double soma = 0.0;
    for (int i = 0; i < n; i++) soma += tempos[i];
    return soma / n;
}

void metricasImprimirTabela(const char *titulo,
                             double *tempos,
                             int n,
                             double media) {
    printf("\n=== %s ===\n", titulo);
    printf("%-10s | %20s\n", "Execucao", "Tempo (s)");
    printf("%-10s-+-%20s\n", "----------", "--------------------");
    for (int i = 0; i < n; i++)
        printf("%-10d | %20.9f\n", i + 1, tempos[i]);
    printf("%-10s | %20.9f\n", "MEDIA", media);
}

long metricasMemoriaKB(void) {
    long kb = -1;
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return kb;
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        if (strncmp(linha, "VmRSS:", 6) == 0) {
            sscanf(linha + 6, "%ld", &kb);
            break;
        }
    }
    fclose(f);
    return kb;
}
