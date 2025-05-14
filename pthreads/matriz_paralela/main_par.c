#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matriz.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <NUM_THREADS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int tamanho = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    matriz_t *A = matriz_criar(tamanho, tamanho);
    matriz_t *B = matriz_criar(tamanho, tamanho);

    matriz_preencher_rand(A);
    matriz_preencher_rand(B);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    matriz_t *C = matriz_somar_com_threads(A, B, num_threads);
    matriz_t *D = matriz_multiplicar_com_threads(A, B, num_threads);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double tempo_total = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Tempo paralelo interno: %f segundos\n", tempo_total);

    matriz_destruir(A);
    matriz_destruir(B);
    matriz_destruir(C);
    matriz_destruir(D);

    return EXIT_SUCCESS;
}
