#include <stdio.h>
#include <stdlib.h>

int nxt_pw2(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

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

void add_mat(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void sub_mat(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void strass(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int new_sz = n / 2;

    int **A11 = alc_mat(new_sz), **A12 = alc_mat(new_sz);
    int **A21 = alc_mat(new_sz), **A22 = alc_mat(new_sz);
    int **B11 = alc_mat(new_sz), **B12 = alc_mat(new_sz);
    int **B21 = alc_mat(new_sz), **B22 = alc_mat(new_sz);
    int **C11 = alc_mat(new_sz), **C12 = alc_mat(new_sz);
    int **C21 = alc_mat(new_sz), **C22 = alc_mat(new_sz);

    int **M1 = alc_mat(new_sz), **M2 = alc_mat(new_sz);
    int **M3 = alc_mat(new_sz), **M4 = alc_mat(new_sz);
    int **M5 = alc_mat(new_sz), **M6 = alc_mat(new_sz);
    int **M7 = alc_mat(new_sz);
    int **temp1 = alc_mat(new_sz), **temp2 = alc_mat(new_sz);

    for (int i = 0; i < new_sz; i++) {
        for (int j = 0; j < new_sz; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + new_sz];
            A21[i][j] = A[i + new_sz][j];
            A22[i][j] = A[i + new_sz][j + new_sz];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + new_sz];
            B21[i][j] = B[i + new_sz][j];
            B22[i][j] = B[i + new_sz][j + new_sz];
        }
    }

    add_mat(A11, A22, temp1, new_sz);
    add_mat(B11, B22, temp2, new_sz);
    strass(temp1, temp2, M1, new_sz);

    add_mat(A21, A22, temp1, new_sz);
    strass(temp1, B11, M2, new_sz);

    sub_mat(B12, B22, temp1, new_sz);
    strass(A11, temp1, M3, new_sz);

    sub_mat(B21, B11, temp1, new_sz);
    strass(A22, temp1, M4, new_sz);

    add_mat(A11, A12, temp1, new_sz);
    strass(temp1, B22, M5, new_sz);

    sub_mat(A21, A11, temp1, new_sz);
    add_mat(B11, B12, temp2, new_sz);
    strass(temp1, temp2, M6, new_sz);

    sub_mat(A12, A22, temp1, new_sz);
    add_mat(B21, B22, temp2, new_sz);
    strass(temp1, temp2, M7, new_sz);

    add_mat(M1, M4, temp1, new_sz);
    sub_mat(temp1, M5, temp2, new_sz);
    add_mat(temp2, M7, C11, new_sz);

    add_mat(M3, M5, C12, new_sz);

    add_mat(M2, M4, C21, new_sz);

    sub_mat(M1, M2, temp1, new_sz);
    add_mat(temp1, M3, temp2, new_sz);
    add_mat(temp2, M6, C22, new_sz);

    for (int i = 0; i < new_sz; i++) {
        for (int j = 0; j < new_sz; j++) {
            C[i][j] = C11[i][j];
            C[i][j + new_sz] = C12[i][j];
            C[i + new_sz][j] = C21[i][j];
            C[i + new_sz][j + new_sz] = C22[i][j];
        }
    }

    fre_mat(A11, new_sz); fre_mat(A12, new_sz);
    fre_mat(A21, new_sz); fre_mat(A22, new_sz);
    fre_mat(B11, new_sz); fre_mat(B12, new_sz);
    fre_mat(B21, new_sz); fre_mat(B22, new_sz);
    fre_mat(C11, new_sz); fre_mat(C12, new_sz);
    fre_mat(C21, new_sz); fre_mat(C22, new_sz);
    fre_mat(M1, new_sz);  fre_mat(M2, new_sz);
    fre_mat(M3, new_sz);  fre_mat(M4, new_sz);
    fre_mat(M5, new_sz);  fre_mat(M6, new_sz);
    fre_mat(M7, new_sz);
    fre_mat(temp1, new_sz); fre_mat(temp2, new_sz);
}

void prt_mat(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n = 4;
    int m = nxt_pw2(n);

    int **A = alc_mat(m);
    int **B = alc_mat(m);
    int **C = alc_mat(m);

    int init_A[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 1, 2, 3},
        {4, 5, 6, 7}
    };
    int init_B[4][4] = {
        {8, 7, 6, 5},
        {4, 3, 2, 1},
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = init_A[i][j];
            B[i][j] = init_B[i][j];
        }
    }

    printf("Matrix A:\n");
    prt_mat(A, n);

    printf("\nMatrix B:\n");
    prt_mat(B, n);

    strass(A, B, C, m);

    printf("\nResultant Matrix C (A * B):\n");
    prt_mat(C, n);

    fre_mat(A, m);
    fre_mat(B, m);
    fre_mat(C, m);

    return 0;
}
