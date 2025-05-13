#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h> // Para usar omp_get_wtime
#include "matriz.h"

typedef struct {
    matriz_t *A;
    matriz_t *B;
    matriz_t *C;
    int inicio, fim;
} thread_data_t;

void *multiplica(void *arg) {
    thread_data_t *dados = (thread_data_t *)arg;
    for (int i = dados->inicio; i < dados->fim; i++) {
        for (int j = 0; j < dados->B->colunas; j++) {
            double sum = 0.0;
            for (int k = 0; k < dados->A->colunas; k++) {
                sum += dados->A->dados[i][k] * dados->B->dados[k][j];
            }
            dados->C->dados[i][j] = sum;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <ordem da matriz> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    matriz_t *A = matriz_criar(N, N);
    matriz_t *B = matriz_criar(N, N);
    matriz_t *C = matriz_criar(N, N);
    matriz_preencher_rand(A);
    matriz_preencher_rand(B);

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    thread_data_t *dados = malloc(sizeof(thread_data_t) * num_threads);
    int bloco = N / num_threads;

    double start = omp_get_wtime();
    for (int i = 0; i < num_threads; i++) {
        dados[i].A = A;
        dados[i].B = B;
        dados[i].C = C;
        dados[i].inicio = i * bloco;
        dados[i].fim = (i == num_threads - 1) ? N : (i + 1) * bloco;
        pthread_create(&threads[i], NULL, multiplica, &dados[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    double end = omp_get_wtime();

    printf("Pthreads (%d threads): %f segundos\n", num_threads, end - start);

    matriz_destruir(A);
    matriz_destruir(B);
    matriz_destruir(C);
    free(threads);
    free(dados);
    return 0;
}
