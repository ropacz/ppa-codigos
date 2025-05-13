#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "matriz.h"

double tempo_execucao(matriz_t *A, matriz_t *B, char *schedule_type, int chunk_size, int num_threads) {
    double start, end;
    matriz_t *R = matriz_criar(A->linhas, B->colunas);
    int i, j, k;

    omp_sched_t tipo;
    if (strcmp(schedule_type, "static") == 0) tipo = omp_sched_static;
    else if (strcmp(schedule_type, "dynamic") == 0) tipo = omp_sched_dynamic;
    else tipo = omp_sched_guided;

    omp_set_schedule(tipo, chunk_size);
    omp_set_num_threads(num_threads);

    start = omp_get_wtime();
    #pragma omp parallel for private(i,j,k) schedule(runtime)
    for (i = 0; i < A->linhas; i++) {
        for (j = 0; j < B->colunas; j++) {
            double sum = 0.0;
            for (k = 0; k < A->colunas; k++) {
                sum += A->dados[i][k] * B->dados[k][j];
            }
            R->dados[i][j] = sum;
        }
    }
    end = omp_get_wtime();

    matriz_destruir(R);
    return end - start;
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
    matriz_preencher_rand(A);
    matriz_preencher_rand(B);

    const char *schedules[] = {"static", "dynamic", "guided"};
    int chunk_size = 16;

    for (int i = 0; i < 3; i++) {
        double tempo = tempo_execucao(A, B, (char *)schedules[i], chunk_size, num_threads);
        printf("OpenMP (%s, %d threads): %f segundos\n", schedules[i], num_threads, tempo);
    }

    matriz_destruir(A);
    matriz_destruir(B);
    return 0;
}
