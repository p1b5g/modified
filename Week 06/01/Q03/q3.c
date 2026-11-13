#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct {
    double real;
    double imag;
} Complex;

void fft(Complex *a, int n, int inv) {
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * PI / len * (inv ? -1 : 1);
        Complex wlen = {cos(ang), sin(ang)};

        for (int i = 0; i < n; i += len) {
            Complex w = {1.0, 0.0};
            for (int j = 0; j < len / 2; j++) {
                Complex u = a[i + j];
                Complex v = {
                    a[i + j + len / 2].real * w.real - a[i + j + len / 2].imag * w.imag,
                    a[i + j + len / 2].real * w.imag + a[i + j + len / 2].imag * w.real
                };

                a[i + j].real = u.real + v.real;
                a[i + j].imag = u.imag + v.imag;
                a[i + j + len / 2].real = u.real - v.real;
                a[i + j + len / 2].imag = u.imag - v.imag;

                Complex w_nxt = {
                    w.real * wlen.real - w.imag * wlen.imag,
                    w.real * wlen.imag + w.imag * wlen.real
                };
                w = w_nxt;
            }
        }
    }

    if (inv) {
        for (int i = 0; i < n; i++) {
            a[i].real /= n;
            a[i].imag /= n;
        }
    }
}

void conv_f(int A[], int B[], int m, int n, int res[]) {
    int size = 1;
    while (size < m + n - 1) size <<= 1;

    Complex *fa = (Complex *)malloc(size * sizeof(Complex));
    Complex *fb = (Complex *)malloc(size * sizeof(Complex));

    for (int i = 0; i < size; i++) {
        fa[i].real = 0; fa[i].imag = 0;
        fb[i].real = 0; fb[i].imag = 0;
    }

    for (int i = 0; i < m; i++) fa[i].real = A[i];
    for (int i = 0; i < n; i++) fb[i].real = B[i];

    fft(fa, size, 0);
    fft(fb, size, 0);

    for (int i = 0; i < size; i++) {
        Complex prod = {
            fa[i].real * fb[i].real - fa[i].imag * fb[i].imag,
            fa[i].real * fb[i].imag + fa[i].imag * fb[i].real
        };
        fa[i] = prod;
    }

    fft(fa, size, 1);

    for (int i = 0; i < m + n - 1; i++) {
        res[i] = (int)round(fa[i].real);
    }

    free(fa);
    free(fb);
}

void prt_arr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int A[] = {1, 2, 3};
    int B[] = {4, 5, 6, 7};
    int m = sizeof(A) / sizeof(A[0]);
    int n = sizeof(B) / sizeof(B[0]);

    int *res = malloc((m + n - 1) * sizeof(int));
    conv_f(A, B, m, n, res);

    printf("A: "); prt_arr(A, m);
    printf("B: "); prt_arr(B, n);
    printf("Convolution result: "); prt_arr(res, m + n - 1);

    free(res);
    return 0;
}
