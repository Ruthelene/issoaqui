#include "Pacote.h"

NoPacote *pacoteCriar(int id, const char *dado) {
    NoPacote *p = (NoPacote *) malloc(sizeof(NoPacote));
    if (!p) { perror("malloc NoPacote"); exit(EXIT_FAILURE); }
    p->id  = id;
    strncpy(p->dado, dado, sizeof(p->dado) - 1);
    p->dado[sizeof(p->dado) - 1] = '\0';
    p->esq = p->dir = NULL;
    return p;
}

NoPacote *pacoteInserir(NoPacote *raiz, int id, const char *dado) {
    if (!raiz) return pacoteCriar(id, dado);
    if (id < raiz->id)
        raiz->esq = pacoteInserir(raiz->esq, id, dado);
    else if (id > raiz->id)
        raiz->dir = pacoteInserir(raiz->dir, id, dado);
    /* id duplicado: ignora (retransmissão) */
    return raiz;
}

/* Imprime todos os pacotes em ordem crescente de ID (caminhamento central). */
void pacoteImprimirEmOrdem(NoPacote *raiz) {
    if (!raiz) return;
    pacoteImprimirEmOrdem(raiz->esq);
    printf("  ID=%04d  dado='%s'\n", raiz->id, raiz->dado);
    pacoteImprimirEmOrdem(raiz->dir);
}

void pacoteMontarArquivo(NoPacote *raiz, FILE *f) {
    if (!raiz) return;
    pacoteMontarArquivo(raiz->esq, f);
    fprintf(f, "ID %04d: %s\n", raiz->id, raiz->dado);
    pacoteMontarArquivo(raiz->dir, f);
}

void pacoteDestruir(NoPacote *raiz) {
    if (!raiz) return;
    pacoteDestruir(raiz->esq);
    pacoteDestruir(raiz->dir);
    free(raiz);
}
