# Trabalho de AED 2: Estruturas de Árvores e Comparação de Desempenho

Este projeto foi desenvolvido como parte da disciplina de Algoritmos e Estruturas de Dados 2. O objetivo principal é a implementação e análise comparativa de diferentes estruturas de dados para busca.

## Descrição do Projeto

O trabalho visa consolidar os conhecimentos sobre a estrutura de **árvores binárias**. Além da implementação dos algoritmos básicos, o projeto foca na comparação de desempenho entre:
1. **Busca Binária em Vetores** (ordenados).
2. **Árvore Binária de Pesquisa (ABP/BST)**.
3. **Árvore AVL** (Árvore binária balanceada).

Toda a implementação foi desenvolvida seguindo os conceitos de **Tipos Abstratos de Dados (TADs)**, garantindo modularidade e encapsulamento do código.

## Funcionalidades Implementadas

- [x] TAD Vetor com busca binária.
- [x] TAD Árvore Binária de Pesquisa (Inserção, Remoção e Busca).
- [x] TAD Árvore AVL (Rotações simples e duplas para balanceamento).
- [x] TAD para o Pacote
- [x] Algoritmos de caminhamento (In-ordem, Pré-ordem, Pós-ordem).
- [x] Módulo de medição de tempo para análise de desempenho.

## Compilação e Execução

raiz do projeto (`work2-aed2/`) para executar o comando abaixo:

```bash
gcc main.c ArvoreBinaria.c AVL.c metricas.c Vetor.c Pacote.c -o prog
./prog
```

## Tecnologias Utilizadas

- Linguagem: C
- Compilador: GCC

## Enunciados das Qustõees

1) Crie uma árvore binária de pesquisa com aproximadamente 20 elementos e mostre o
resultado para os três tipos de caminhamento: pré-fixado, central e pós-fixado.

2) Crie uma árvore binária de pesquisa para controlar o recebimento de pacotes em uma
transmissão em rede (vamos simular a chegada dos pacotes). Suponha cada pacote
identificado por um número inteiro e mais um dado de um tipo qualquer (a sua escolha),
que representa o conteúdo do arquivo. A ordem de chegada deve ser parcialmente
ordenada. Deve haver pacotes repetidos (um percentual pequeno do total), simulando
a retransmissão de pacotes. No final, o seu programa deve montar o arquivo, ou seja,
criar um arquivo com os dados recebidos dos pacotes na ordem correta, conforme o
identificador. O arquivo pode ser do tipo texto ou binário.

3) Crie uma árvore binária de pesquisa com 1.000.000 (um milhão) de elementos do tipo
inteiro. Implemente as operações de inserção e de busca por um valor. Compare o
tempo da busca na árvore com o tempo da busca binária no vetor, aproveitando a
implementação do vetor do Trabalho 1. Os valores presentes no vetor e na árvore
devem ser os mesmos. Execute a busca 30 vezes usando os mesmos valores de busca
para a árvore e para o vetor em cada execução. Calcule o tempo de execução para
cada busca e a média entre as 30 execuções para cada estrutura de dados. Entre as 30
buscas, pelo menos 15 buscas devem ser de elementos presentes nas estruturas de
dados. Observe a utilização de memória durante a execução do programa: qual a menor
e a maior ocupação da memória verificados?

4) Crie uma árvore AVL e uma árvore binária de pesquisa com os mesmos 1.000.000 (um
milhão) de elementos do tipo inteiro. Implemente as operações de inserção e de busca
por um valor. Crie 10 árvores de cada tipo (com os mesmos valores em cada execução)
e compare o tempo da criação da árvore AVL com o tempo da criação da árvore binária
de pesquisa. Mostre a altura das árvores em cada uma das 10 execuções. Compare
também o tempo da busca na árvore AVL com o tempo da busca na árvore binária de
pesquisa, calculando o tempo de execução para 30 consultas e a média entre as 30
consultas para cada árvore.

