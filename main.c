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
#define TAMANHO_GRANDE 1000000
#define NUM_BUSCAS 30
#define BUSCAS_PRESENTES 15 /* pelo menos 15 das 30 devem existir */
#define NUM_ARVORES 10 /* questão 4 */

/* ================================================================
 * QUESTÃO 1 — Caminhamentos na BST (~20 elementos, gerados aleatoriamente)
 * ================================================================ */

void questao1(void) {
    printf("\n===================================================\n");
    printf(" QUESTAO 1 - Caminhamentos na BST\n");
    printf("===================================================\n");

    const int N = 20;
    const int RANGE = 200; /* valores entre 1 e 200 para evitar muitas colisões */

    srand((unsigned int) time(NULL));

    No *raiz = NULL;
    int inseridos = 0;
    printf("\nElementos inseridos: ");
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

    printf("Pre-fixado (raiz, esq, dir): ");
    preOrdem(raiz);
    puts("");

    printf("Central (esq, raiz, dir): ");
    emOrdem(raiz);
    puts("");

    printf("Pos-fixado (esq, dir, raiz): ");
    posOrdem(raiz);
    puts("");

    destruirArvore(raiz);
}

/* ================================================================
 * QUESTÃO 2 — Simulação de recebimento de pacotes
 * ================================================================ */

void questao2(void) {
    printf("\n===================================================\n");
    printf(" QUESTAO 2 - Simulacao de pacotes de rede\n");
    printf("===================================================\n");

    const int TOTAL_PACOTES = 30;
    const int IDS_UNICOS = 27;

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

        printf(" [pos=%02d] Recebido pacote ID=%d dado='%s'\n", pos, id, dado);
        arvore = pacoteInserir(arvore, id, dado);
    }

    /* --- PRINT 1: ordem de chegada já foi impressa acima linha a linha --- */
    /* --- PRINT 2: arquivo montado (ordem correta por ID) --- */
    puts("\n--- Arquivo montado (ordem por ID) ---");
    pacoteImprimirEmOrdem(arvore);

    const char *nome_arquivo = "arquivo_montado.txt";
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) {
        perror("fopen arquivo_montado.txt");
    } else {
        pacoteMontarArquivo(arvore, f);
        fclose(f);
        printf("\nArquivo '%s' gravado com sucesso.\n", nome_arquivo);
    }

    pacoteDestruir(arvore);
}

/* ================================================================
 * QUESTÃO 3 — BST vs Busca Binária no Vetor (1 milhão de elementos)
 * ================================================================ */
void questao3(void) {
    printf("\n===================================================\n");
    printf(" QUESTAO 3 - BST vs Busca Binaria no Vetor\n");
    printf("===================================================\n");

    srand(12345);

    Vetor *valores, *vetor, *valoresBusca, *status;
    double temposBST[NUM_BUSCAS], temposVetor[NUM_BUSCAS];

    valores = vetorCriar(TAMANHO_GRANDE);
    if (!valores) { perror("malloc valores q3"); exit(EXIT_FAILURE); }

    //preencher vetor
    for (int i = 0; i < TAMANHO_GRANDE; i++){
        vetorInserir(valores, i, i + 1);
    }

    //preencher vetor
    for (int i = TAMANHO_GRANDE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = valores->dados[i]; 
        vetorInserir(valores, i, valores->dados[j]);
        vetorInserir(valores, j, tmp);
    }

    printf("\nConstruindo vetor com %d elementos...\n", TAMANHO_GRANDE);

    long memAntes = lerMemoriaKB();

    vetor = vetorCriar(TAMANHO_GRANDE);
    for (int i = 0; i < TAMANHO_GRANDE; i++){
        vetorInserir(vetor, i, i + 1);
    }

    long memDepoisVetor = lerMemoriaKB();

    printf("Construindo BST com %d elementos...\n", TAMANHO_GRANDE);
    No *bst = NULL;

    //preenchendo árvore
    for (int i = 0; i < TAMANHO_GRANDE; i++)
        bst = inserir(bst, valores->dados[i]);

    long memDepoisBST = lerMemoriaKB();
    
    printf("\n\n=> Memoria(RSS):\n");
    printf("Inicial: %ld KB\n", memAntes);
    printf("Apos vetor: %ld KB\n", memDepoisVetor);
    printf("Apos BST: %ld KB\n\n", memDepoisBST);

    valoresBusca = vetorCriar(NUM_BUSCAS);
    status = vetorCriar(NUM_BUSCAS);

    for (int i = 0; i < BUSCAS_PRESENTES; i++){
       vetorInserir(valoresBusca, i, valores->dados[TAMANHO_GRANDE - 1 - i]);
    }

    for (int i = BUSCAS_PRESENTES; i < NUM_BUSCAS; i++){
       vetorInserir(valoresBusca, i, -(i + 1));
    }
    printf("Tabela: Arvore Binaria de Pesquisa (BST) x Busca Binaria no Vetor\n");
    printf("-----------+----------------------+----------------------+------------------\n");
    printf("%-10s | %20s | %20s | %15s\n", " Busca ", " Tempo BST (s) ", " Tempo Vetor (s) ", " Status ");
    printf("-----------+----------------------+----------------------+------------------\n");

    int result;
    
    for (int i = 0; i < NUM_BUSCAS; i++) {
        /* ----- buscas na BST ----- */
        double t0 = metricasAgora();
        buscar(bst, valoresBusca->dados[i]);
        temposBST[i] = metricasAgora() - t0;

        /* ----- buscas na Arvore ----- */
        t0 = metricasAgora();
        result = vetorBuscaBinaria(vetor, valoresBusca->dados[i]);
        temposVetor[i] = metricasAgora() - t0;

        if(i<10) {
            if(result != -1){
                 printf(" %d | %.12lf | %.12lf | Encontrado \n", i, temposBST[i], temposVetor[i]);
            } else {
                 printf(" %d | %.12lf | %.12lf | Nao encontrado \n", i, temposBST[i], temposVetor[i]);
            }
           
        } else {
            if(result != -1){
                printf(" %d | %.12lf | %.12lf | Encontrado \n", i, temposBST[i], temposVetor[i]);
            } else {
                printf(" %d | %.12lf | %.12lf | Nao encontrado \n", i, temposBST[i], temposVetor[i]);
            }
        }
    }

    double mediaVetor = metricasMedia(temposVetor, NUM_BUSCAS);
    double mediaBST = metricasMedia(temposBST, NUM_BUSCAS);
    
    printf("-----------+----------------------+----------------------+------------------\n");          
    printf(" Media | %.12lf | %.12lf |\n", mediaBST, mediaVetor);
    printf("-----------+----------------------+----------------------+------------------\n");
    
    long memFinal = lerMemoriaKB();
  
    printf("\nMemoria (RSS) menor observada: %ld KB | maior observada: %ld KB\n",
           memAntes < memDepoisVetor ? memAntes : memDepoisVetor,
           memDepoisBST > memFinal ? memDepoisBST : memFinal);

    destruirArvore(bst);
    vetorDestruir(vetor);
    free(valores);
}


/* ================================================================
 * QUESTÃO 4 — AVL vs BST: criação, altura e busca
 * ================================================================ */

void questao4(void) {
    printf("\n===================================================\n");
    printf(" QUESTAO 4 - AVL vs BST: criacao e busca \n");
    printf("===================================================\n");

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

    printf("\nTabela: Criacao das arvores (10 execucoes)\n");

     printf("%-10s-+-%20s-+-%10s-+-%20s-+-%10s\n",
           "----------", "--------------------",
           "----------", "--------------------", "----------");
    printf("%-10s | %20s | %10s | %20s | %10s\n",
           "Execucao", "Tempo BST (s)", "Altura BST", "Tempo AVL (s)", "Altura AVL");
    printf("%-10s-+-%20s-+-%10s-+-%20s-+-%10s\n","----------", "--------------------","----------", "--------------------", "----------");

    double temposBST[NUM_BUSCAS];
    double temposAVL[NUM_BUSCAS];

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

            for (int i = 0; i < NUM_BUSCAS; i++) {
                double tb = metricasAgora();
                buscar(bst, busca[i]);
                temposBST[i] = metricasAgora() - tb;

                double ta = metricasAgora();
                avlBuscar(avl, busca[i]);
                temposAVL[i] = metricasAgora() - ta;
            }         
        }

        destruirArvore(bst);
        avlDestruir(avl);
    }
    double mediaBST = metricasMedia(temposBST, NUM_BUSCAS);
    double mediaAVL = metricasMedia(temposAVL, NUM_BUSCAS);  
    double mediaCriacaoBST = metricasMedia(tempoCriacaoBST, NUM_ARVORES);
    double mediaCriacaoAVL = metricasMedia(tempoCriacaoAVL, NUM_ARVORES);

    printf("-----------+----------------------+------------+----------------------+-----------\n");
    printf(" Media | %20.9f | | %20.9f | \n", mediaCriacaoBST, mediaCriacaoAVL);
    printf("-----------+----------------------+------------+----------------------+-----------\n");

    printf("\nTabela: Busca na Arvore Binaria de Pesquisa (BST) x Busca na Arvore AVL\n");
    printf("-----------+----------------------+----------------------\n");
    printf("%-10s | %20s | %20s \n", " Busca ", " Tempo BST (s) ", " Tempo AVL (s) ");
    printf("-----------+----------------------+----------------------\n");

    for (int i = 0; i < NUM_BUSCAS; i++) {
        if(i<10) {
            printf(" %d | %.12lf | %.12lf \n", i, temposBST[i], temposAVL[i]);
        } else {
            printf(" %d | %.12lf | %.12lf \n", i, temposBST[i], temposAVL[i]);
        }
    }
    printf("-----------+----------------------+----------------------\n");          
    printf(" Media | %.12lf | %.12lf \n", mediaBST, mediaAVL);
    printf("-----------+----------------------+----------------------\n");

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