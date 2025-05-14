#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matriz.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <NUM_THREADS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int linhas = atoi(argv[1]);
    int colunas = linhas;
    int num_threads = atoi(argv[2]);

    matriz_t *A = matriz_criar(linhas, colunas);
    matriz_t *B = matriz_criar(linhas, colunas);
    matriz_t *C = NULL;
    matriz_t *D = NULL;

    if (!A || !B) {
        fprintf(stderr, "Erro ao criar matrizes A ou B\n");
        return EXIT_FAILURE;
    }

    matriz_preencher_rand(A);
    matriz_preencher_rand(B);

    // Soma paralela: C = A + B
    C = matriz_somar_com_threads(A, B, num_threads);
    if (!C) {
        fprintf(stderr, "Erro na soma paralela\n");
        matriz_destruir(A);
        matriz_destruir(B);
        return EXIT_FAILURE;
    }

    // Multiplicação paralela: D = C * A
    D = matriz_multiplicar_com_threads(C, A, num_threads);
    if (!D) {
        fprintf(stderr, "Erro na multiplicação paralela\n");
        matriz_destruir(A);
        matriz_destruir(B);
        matriz_destruir(C);
        return EXIT_FAILURE;
    }

    // Impressão descomentável para debug:
    // printf("Matriz A:\n"); matriz_imprimir(A);
    // printf("Matriz B:\n"); matriz_imprimir(B);
    // printf("Matriz C (A + B):\n"); matriz_imprimir(C);
    // printf("Matriz D (C * A):\n"); matriz_imprimir(D);

    matriz_destruir(A);
    matriz_destruir(B);
    matriz_destruir(C);
    matriz_destruir(D);

    return EXIT_SUCCESS;
}
