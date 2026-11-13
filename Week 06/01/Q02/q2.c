#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 3

void prt_mat(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", A[i][j]);
        }
        printf("\n");
    }
}

void mat_add(double A[N][N], double B[N][N], double C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void mat_mul(double A[N][N], double B[N][N], double C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int is_zero(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(A[i][j]) > 1e-9) return 0;
        }
    }
    return 1;
}

int is_sym(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(A[i][j] - A[j][i]) > 1e-9) return 0;
        }
    }
    return 1;
}

double det_mat(double A[N][N], int n) {
    if (n == 1) return A[0][0];
    if (n == 2) {
        return A[0][0] * A[1][1] - A[0][1] * A[1][0];
    }

    double det = 0;
    int sign = 1;
    for (int col = 0; col < n; col++) {
        double minor[N][N];
        int r = 0;
        for (int i = 1; i < n; i++) {
            int c = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                minor[r][c++] = A[i][j];
            }
            r++;
        }
        det += sign * A[0][col] * det_mat(minor, n - 1);
        sign *= -1;
    }
    return det;
}

void trn_inp(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            double temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;
        }
    }
}

void mul_vm(double A[N][N], double x[N], double out[N]) {
    for (int i = 0; i < N; i++) {
        out[i] = 0;
        for (int j = 0; j < N; j++) {
            out[i] += A[i][j] * x[j];
        }
    }
}

void pow_itr(double A[N][N], double *eig_val, double eig_vec[N]) {
    double x[N] = {1.0, 1.0, 1.0};
    const int iters = 100;

    for (int iter = 0; iter < iters; iter++) {
        double y[N];
        mul_vm(A, x, y);

        double norm = 0.0;
        for (int i = 0; i < N; i++) norm += y[i] * y[i];
        norm = sqrt(norm);

        for (int i = 0; i < N; i++) x[i] = y[i] / norm;
    }

    double y[N];
    mul_vm(A, x, y);
    double num = 0.0;
    double den = 0.0;
    for (int i = 0; i < N; i++) {
        num += x[i] * y[i];
        den += x[i] * x[i];
    }

    *eig_val = num / den;
    for (int i = 0; i < N; i++) eig_vec[i] = x[i];
}

int main(void) {
    double A[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    double B[N][N] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    double C[N][N];
    double D[N][N];
    double eig_vec[N];
    double eig_val;

    printf("Matrix A:\n");
    prt_mat(A);
    printf("\nMatrix B:\n");
    prt_mat(B);

    mat_add(A, B, C);
    printf("\nA + B:\n");
    prt_mat(C);

    mat_mul(A, B, D);
    printf("\nA * B:\n");
    prt_mat(D);

    printf("\nIs zero matrix? %s\n", is_zero(A) ? "Yes" : "No");
    printf("Is symmetric? %s\n", is_sym(A) ? "Yes" : "No");
    printf("Determinant: %.2f\n", det_mat(A, N));

    trn_inp(A);
    printf("\nTranspose in place:\n");
    prt_mat(A);

    double S[N][N] = {
        {2, 1, 0},
        {1, 3, 1},
        {0, 1, 4}
    };
    pow_itr(S, &eig_val, eig_vec);
    printf("\nLargest eigenvalue (approx): %.4f\n", eig_val);
    printf("Eigenvector: ");
    for (int i = 0; i < N; i++) printf("%.4f ", eig_vec[i]);
    printf("\n");

    return 0;
}
