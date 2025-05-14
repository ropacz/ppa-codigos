#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"
#include <pthread.h>

matriz_t *matriz_criar(int linhas, int colunas) {
    matriz_t *retorno = (matriz_t *) malloc(sizeof(matriz_t));
    if (!retorno) return NULL;

    retorno->linhas = linhas;
    retorno->colunas = colunas;

    double *mat = (double *) malloc(linhas * colunas * sizeof(double));
    if (!mat) {
        free(retorno);
        return NULL;
    }

    retorno->dados = (double **) malloc(sizeof(double *) * linhas);
    if (!retorno->dados) {
        free(mat);
        free(retorno);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        retorno->dados[i] = &mat[i * colunas];
    }

    return retorno;
}

void matriz_destruir(matriz_t *m) {
    if (!m) return;
    free(m->dados[0]);  // free o bloco de dados reais
    free(m->dados);     // free os ponteiros das linhas
    free(m);            // free a estrutura principal
}

void matriz_preencher_rand(matriz_t *m) {
    if (!m) return;
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            m->dados[i][j] = random();
        }
    }
}

void matriz_preencher(matriz_t *m, double valor) {
    if (!m) return;
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            m->dados[i][j] = valor;
        }
    }
}

matriz_t *matriz_multiplicar(matriz_t *A, matriz_t *B) {
    if (!A || !B || A->colunas != B->linhas) return NULL;

    matriz_t *m = matriz_criar(A->linhas, B->colunas);
    if (!m) return NULL;

    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            double sum = 0.0;
            for (int k = 0; k < A->colunas; k++) {
                sum += A->dados[i][k] * B->dados[k][j];
            }
            m->dados[i][j] = sum;
        }
    }
    return m;
}
/*
Cada thread processa linhas da matriz "𝐴"
A alternadamente, de acordo com seu tid (ID da thread).
Exemplo com 4 threads:
Thread 0 calcula linhas 0, 4, 8...
Thread 1 calcula linhas 1, 5, 9...
Esse esquema se chama "distribuição intercalada" (round-robin).
*/
void *matriz_multiplicar_paralelo(void *args) {
   // Função que será executada por cada thread
    thread_params *p = (thread_params *) args;
    int tid = p->tid;
    int num_threads = p->num_threads;
    int linhas = p->A->linhas;

    for (int i = tid; i < linhas; i += num_threads) {
        for (int j = 0; j < p->B->colunas; j++) {
            p->C->dados[i][j] = 0.0;
            for (int k = 0; k < p->A->colunas; k++) {
                p->C->dados[i][j] += p->A->dados[i][k] * p->B->dados[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void matriz_imprimir(matriz_t *m) {
    if (!m) return;
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            printf("%.17f ", m->dados[i][j]);
        }
        printf("\n");
    }
}

void *matriz_somar_paralelo(void *args) {
    thread_params *p = (thread_params *) args;
    int tid = p->tid;
    int num_threads = p->num_threads;

    for (int i = tid; i < p->A->linhas; i += num_threads) {
        for (int j = 0; j < p->A->colunas; j++) {
            p->C->dados[i][j] = p->A->dados[i][j] + p->B->dados[i][j];
        }
    }

    pthread_exit(NULL);
}

matriz_t *matriz_somar(matriz_t *A, matriz_t *B) {
    if (!A || !B || A->linhas != B->linhas || A->colunas != B->colunas)
        return NULL;

    matriz_t *retorno = matriz_criar(A->linhas, A->colunas);
    if (!retorno) return NULL;

    for (int i = 0; i < A->linhas; i++) {
        for (int j = 0; j < A->colunas; j++) {
            retorno->dados[i][j] = A->dados[i][j] + B->dados[i][j];
        }
    }
    return retorno;
}

/// ---- 

matriz_t *matriz_somar_com_threads(matriz_t *A, matriz_t *B, int num_threads) {
   if (!A || !B || A->linhas != B->linhas || A->colunas != B->colunas)
       return NULL;

   matriz_t *C = matriz_criar(A->linhas, A->colunas);
   if (!C) return NULL;

   pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
   thread_params *params = malloc(sizeof(thread_params) * num_threads);

   for (int i = 0; i < num_threads; i++) {
       params[i].tid = i;
       params[i].num_threads = num_threads;
       params[i].A = A;
       params[i].B = B;
       params[i].C = C;
       pthread_create(&threads[i], NULL, matriz_somar_paralelo, &params[i]);
   }

   for (int i = 0; i < num_threads; i++) {
       pthread_join(threads[i], NULL);
   }

   free(threads);
   free(params);

   return C;
}

matriz_t *matriz_multiplicar_com_threads(matriz_t *A, matriz_t *B, int num_threads) {
   if (!A || !B || A->colunas != B->linhas)
       return NULL;

   matriz_t *C = matriz_criar(A->linhas, B->colunas);
   if (!C) return NULL;

   pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
   thread_params *params = malloc(sizeof(thread_params) * num_threads);

   for (int i = 0; i < num_threads; i++) {
       params[i].tid = i;
       params[i].num_threads = num_threads;
       params[i].A = A;
       params[i].B = B;
       params[i].C = C;
       pthread_create(&threads[i], NULL, matriz_multiplicar_paralelo, &params[i]);
   }

   for (int i = 0; i < num_threads; i++) {
       pthread_join(threads[i], NULL);
   }

   free(threads);
   free(params);

   return C;
}
