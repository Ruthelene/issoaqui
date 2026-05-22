#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ArvoreBinaria.h"
#include "AVL.h"
#include "Vetor.h"
#include "metricas.h"
#include "Pacote.h"

/* ================================================================
 * Configurações
 * ================================================================ */
#define TAMANHO_GRANDE      1000000
#define NUM_BUSCAS          30
#define BUSCAS_PRESENTES    15     /* pelo menos 15 das 30 devem existir */
#define NUM_ARVORES         10     /* questão 4 */

/* ================================================================
 * QUESTÃO 1 — Caminhamentos na BST (~20 elementos, gerados aleatoriamente)
 * ================================================================ */

void questao1(void) {
    puts("========================================");
    puts("QUESTAO 1 — Caminhamentos na BST");
    puts("========================================");

    const int N = 20;
    const int RANGE = 200; /* valores entre 1 e 200 para evitar muitas colisões */

    srand((unsigned int) time(NULL));

    No *raiz = NULL;
    int inseridos = 0;
    printf("Elementos inseridos: ");
    while (inseridos < N) {
        int v = (rand() % RANGE) + 1;
        /* Só insere se não existir (para garantir exatamente N nós distintos) */
        if (buscar(raiz, v) == NULL) {
            printf("%d ", v);
            raiz = inserir(raiz, v);
            inseridos++;
        }
    }
    puts("");

    printf("Altura da arvore: %d\n\n", altura(raiz));

    printf("Pre-fixado  (raiz, esq, dir): ");
    preOrdem(raiz);
    puts("");

    printf("Central     (esq, raiz, dir): ");
    emOrdem(raiz);
    puts("");

    printf("Pos-fixado  (esq, dir, raiz): ");
    posOrdem(raiz);
    puts("");

    destruirArvore(raiz);
}

/* ================================================================
 * QUESTÃO 2 — Simulação de recebimento de pacotes
 * ================================================================ */

void questao2(void) {
    puts("\n========================================");
    puts("QUESTAO 2 — Simulacao de pacotes de rede");
    puts("========================================");

    const int TOTAL_PACOTES = 30;
    const int IDS_UNICOS    = 27;

    NoPacote *arvore = NULL;
    srand(42);

    int ids[27];
    for (int i = 0; i < IDS_UNICOS; i++) ids[i] = i + 1;

    /* Embaralha os IDs únicos */
    for (int i = IDS_UNICOS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = ids[i]; ids[i] = ids[j]; ids[j] = tmp;
    }

    int duplicatas[3] = {ids[0], ids[3], ids[7]};

    printf("Simulando chegada de %d pacotes (%d unicos + 3 retransmissoes):\n",
           TOTAL_PACOTES, IDS_UNICOS);

    int dup_idx = 0, dup_pos[3] = {5, 12, 22};
    int uniq = 0;

    for (int pos = 0; pos < TOTAL_PACOTES; pos++) {
        int id;
        char dado[64];

        if (dup_idx < 3 && pos == dup_pos[dup_idx]) {
            id = duplicatas[dup_idx++];
            snprintf(dado, sizeof(dado), "RETRANSMISSAO do pacote %d", id);
        } else {
            id = ids[uniq++];
            snprintf(dado, sizeof(dado), "Conteudo do id %d", id);
        }

        printf("  [pos=%02d] Recebido pacote ID=%d  dado='%s'\n", pos, id, dado);
        arvore = pacoteInserir(arvore, id, dado);
    }

    const char *nome_arquivo = "arquivo_montado.txt";
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) {
        perror("fopen arquivo_montado.txt");
    } else {
        pacoteMontarArquivo(arvore, f);
        fclose(f);
        printf("Arquivo '%s' gerado com sucesso.\n", nome_arquivo);
    }

    pacoteDestruir(arvore);
}

/* ================================================================
 * QUESTÃO 3 — BST vs Busca Binária no Vetor (1 milhão de elementos)
 * ================================================================ */

void questao3(void) {
    puts("\n========================================");
    puts("QUESTAO 3 — BST vs Busca Binaria no Vetor");
    puts("========================================");

    srand(12345);
    int *valores = (int *) malloc(sizeof(int) * TAMANHO_GRANDE);
    if (!valores) { perror("malloc valores q3"); exit(EXIT_FAILURE); }

    for (int i = 0; i < TAMANHO_GRANDE; i++) valores[i] = i + 1;
    for (int i = TAMANHO_GRANDE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = valores[i]; valores[i] = valores[j]; valores[j] = tmp;
    }

    printf("Construindo vetor com %d elementos...\n", TAMANHO_GRANDE);
    long memAntes = metricasMemoriaKB();

    Vetor *v = vetorCriar(TAMANHO_GRANDE);
    for (int i = 0; i < TAMANHO_GRANDE; i++)
        vetorInserir(v, i, valores[i]);
    vetorOrdenar(v);

    long memDepoisVetor = metricasMemoriaKB();

    printf("Construindo BST com %d elementos...\n", TAMANHO_GRANDE);
    No *bst = NULL;
    for (int i = 0; i < TAMANHO_GRANDE; i++)
        bst = inserir(bst, valores[i]);

    long memDepoisBST = metricasMemoriaKB();
    printf("Memoria (RSS) antes: %ld KB | apos vetor: %ld KB | apos BST: %ld KB\n",
           memAntes, memDepoisVetor, memDepoisBST);

    int busca[NUM_BUSCAS];
    for (int i = 0; i < BUSCAS_PRESENTES; i++)
        busca[i] = valores[TAMANHO_GRANDE - 1 - i];
    for (int i = BUSCAS_PRESENTES; i < NUM_BUSCAS; i++)
        busca[i] = -(i + 1);

    /* ----- buscas na BST ----- */
    double temposBST[NUM_BUSCAS];
    for (int i = 0; i < NUM_BUSCAS; i++) {
        double t0 = metricasAgora();
        buscar(bst, busca[i]);
        temposBST[i] = metricasAgora() - t0;
    }
    double mediaBST = metricasMedia(temposBST, NUM_BUSCAS);
    metricasImprimirTabela("Busca na BST", temposBST, NUM_BUSCAS, mediaBST);

    /* ----- buscas no vetor ----- */
    double temposVetor[NUM_BUSCAS];
    for (int i = 0; i < NUM_BUSCAS; i++) {
        double t0 = metricasAgora();
        vetorBuscaBinaria(v, busca[i]);
        temposVetor[i] = metricasAgora() - t0;
    }
    double mediaVetor = metricasMedia(temposVetor, NUM_BUSCAS);
    metricasImprimirTabela("Busca Binaria no Vetor", temposVetor, NUM_BUSCAS, mediaVetor);

    long memFinal = metricasMemoriaKB();
    printf("\nMemoria (RSS) menor observada: %ld KB | maior observada: %ld KB\n",
           memAntes < memDepoisVetor ? memAntes : memDepoisVetor,
           memDepoisBST > memFinal   ? memDepoisBST : memFinal);

    destruirArvore(bst);
    vetorDestruir(v);
    free(valores);
}

/* ================================================================
 * QUESTÃO 4 — AVL vs BST: criação, altura e busca
 * ================================================================ */

void questao4(void) {
    puts("\n========================================");
    puts("QUESTAO 4 — AVL vs BST: criacao e busca");
    puts("========================================");

    srand(99999);
    int *valores = (int *) malloc(sizeof(int) * TAMANHO_GRANDE);
    if (!valores) { perror("malloc valores q4"); exit(EXIT_FAILURE); }
    for (int i = 0; i < TAMANHO_GRANDE; i++) valores[i] = i + 1;
    for (int i = TAMANHO_GRANDE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = valores[i]; valores[i] = valores[j]; valores[j] = tmp;
    }

    double tempoCriacaoBST[NUM_ARVORES];
    double tempoCriacaoAVL[NUM_ARVORES];

    printf("\n=== Criacao das arvores (10 execucoes) ===\n");
    printf("%-10s | %20s | %10s | %20s | %10s\n",
           "Execucao", "Tempo BST (s)", "Altura BST", "Tempo AVL (s)", "Altura AVL");
    printf("%-10s-+-%20s-+-%10s-+-%20s-+-%10s\n",
           "----------", "--------------------",
           "----------", "--------------------", "----------");

    for (int exec = 0; exec < NUM_ARVORES; exec++) {
        /* BST */
        double t0 = metricasAgora();
        No *bst = NULL;
        for (int i = 0; i < TAMANHO_GRANDE; i++)
            bst = inserir(bst, valores[i]);
        tempoCriacaoBST[exec] = metricasAgora() - t0;
        int hBST = altura(bst);

        /* AVL */
        t0 = metricasAgora();
        NoAVL *avl = NULL;
        for (int i = 0; i < TAMANHO_GRANDE; i++)
            avl = avlInserir(avl, valores[i]);
        tempoCriacaoAVL[exec] = metricasAgora() - t0;
        int hAVL = avlAltura(avl);

        printf("%-10d | %20.9f | %10d | %20.9f | %10d\n",
               exec + 1,
               tempoCriacaoBST[exec], hBST,
               tempoCriacaoAVL[exec], hAVL);

        /* Buscas apenas na última execução */
        if (exec == NUM_ARVORES - 1) {
            int busca[NUM_BUSCAS];
            for (int i = 0; i < BUSCAS_PRESENTES; i++)
                busca[i] = valores[i];
            for (int i = BUSCAS_PRESENTES; i < NUM_BUSCAS; i++)
                busca[i] = -(i + 1);

            double temposBST[NUM_BUSCAS];
            double temposAVL[NUM_BUSCAS];

            for (int i = 0; i < NUM_BUSCAS; i++) {
                double tb = metricasAgora();
                buscar(bst, busca[i]);
                temposBST[i] = metricasAgora() - tb;

                double ta = metricasAgora();
                avlBuscar(avl, busca[i]);
                temposAVL[i] = metricasAgora() - ta;
            }

            double mediaBST = metricasMedia(temposBST, NUM_BUSCAS);
            double mediaAVL = metricasMedia(temposAVL, NUM_BUSCAS);

            metricasImprimirTabela("Busca na BST (30 consultas)",
                                   temposBST, NUM_BUSCAS, mediaBST);
            metricasImprimirTabela("Busca na AVL (30 consultas)",
                                   temposAVL, NUM_BUSCAS, mediaAVL);
        }

        destruirArvore(bst);
        avlDestruir(avl);
    }

    double mediaCriacaoBST = metricasMedia(tempoCriacaoBST, NUM_ARVORES);
    double mediaCriacaoAVL = metricasMedia(tempoCriacaoAVL, NUM_ARVORES);
    printf("%-10s | %20.9f | %10s | %20.9f | %10s\n",
           "MEDIA", mediaCriacaoBST, "-", mediaCriacaoAVL, "-");

    free(valores);
}

/* ================================================================
 * main
 * ================================================================ */

int main(void) {
    questao1();
    questao2();
    questao3();
    questao4();
    return 0;
}
