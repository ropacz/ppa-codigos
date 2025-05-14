#ifndef __MATRIZ_H
#define __MATRIZ_H

#define random() ((rand() ^ rand()) / (RAND_MAX + 1.0))

typedef struct {
      double **dados;
      int linhas;
      int colunas;
} matriz_t;

matriz_t *matriz_criar(int linhas, int colunas);

void matriz_destruir(matriz_t *m);

void matriz_preencher_rand(matriz_t *m);

void matriz_preencher(matriz_t *m, double valor);

matriz_t *matriz_multiplicar(matriz_t *A, matriz_t *B);

matriz_t *matriz_somar(matriz_t *A, matriz_t *B);

void matriz_imprimir(matriz_t *m);

#endif 
