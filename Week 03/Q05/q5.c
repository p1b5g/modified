#include <stdio.h>
#include <stdlib.h>

int **alc_mat(int n) {
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = (int *)calloc(n, sizeof(int));
    }
    return mat;
}

void fre_mat(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void bld_rec(int **M, int *arr, int r, int c, int size) {
    if (size == 1) {
        M[r][c] = arr[0];
        return;
    }
    int half = size / 2;
    bld_rec(M, arr, r, c, half);
    bld_rec(M, arr, r + half, c + half, half);
    bld_rec(M, arr + half, r, c + half, half);
    bld_rec(M, arr + half, r + half, c, half);
}

void prt_mat(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void mult_dc(int **A, int **B, int **C, int rA, int cA, int rB, int cB, int rC, int cC, int size) {
    if (size == 1) {
        C[rC][cC] = A[rA][cA] * B[rB][cB];
        return;
    }

    int half = size / 2;

    int **sumA = alc_mat(half);
    int **sumB = alc_mat(half);
    int **diffA = alc_mat(half);
    int **diffB = alc_mat(half);
    int **P = alc_mat(half);
    int **Q = alc_mat(half);

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            sumA[i][j]  = A[rA + i][cA + j] + A[rA + i][cA + half + j];
            sumB[i][j]  = B[rB + i][cB + j] + B[rB + i][cB + half + j];
            diffA[i][j] = A[rA + i][cA + j] - A[rA + i][cA + half + j];
            diffB[i][j] = B[rB + i][cB + j] - B[rB + i][cB + half + j];
        }
    }

    mult_dc(sumA, sumB, P, 0, 0, 0, 0, 0, 0, half);
    mult_dc(diffA, diffB, Q, 0, 0, 0, 0, 0, 0, half);

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            int c1 = (P[i][j] + Q[i][j]) / 2;
            int c2 = (P[i][j] - Q[i][j]) / 2;

            C[rC + i][cC + j] = c1;
            C[rC + half + i][cC + half + j] = c1;

            C[rC + i][cC + half + j] = c2;
            C[rC + half + i][cC + j] = c2;
        }
    }

    fre_mat(sumA, half);
    fre_mat(sumB, half);
    fre_mat(diffA, half);
    fre_mat(diffB, half);
    fre_mat(P, half);
    fre_mat(Q, half);
}

int main(void) {
    int n = 4;
    int row_a[] = {1, 2, 3, 4};
    int row_b[] = {4, 3, 2, 1};

    int **A = alc_mat(n);
    int **B = alc_mat(n);
    int **C = alc_mat(n);

    bld_rec(A, row_a, 0, 0, n);
    bld_rec(B, row_b, 0, 0, n);

    printf("Matrix A:\n");
    prt_mat(A, n);
    printf("Matrix B:\n");
    prt_mat(B, n);

    mult_dc(A, B, C, 0, 0, 0, 0, 0, 0, n);

    printf("Resultant Matrix C (A * B):\n");
    prt_mat(C, n);

    fre_mat(A, n);
    fre_mat(B, n);
    fre_mat(C, n);
    return 0;
}
