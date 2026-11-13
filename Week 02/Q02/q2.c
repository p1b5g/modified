#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void merge2(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void m_sort2(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        m_sort2(arr, l, m);
        m_sort2(arr, m + 1, r);
        merge2(arr, l, m, r);
    }
}

void merge3(int arr[], int l, int mid1, int mid2, int r) {
    int n = r - l + 1;
    int *dest = (int *)malloc(n * sizeof(int));
    int i = l, j = mid1 + 1, k = mid2 + 1;
    int idx = 0;

    while (i <= mid1 && j <= mid2 && k <= r) {
        if (arr[i] <= arr[j]) {
            if (arr[i] <= arr[k])
                dest[idx++] = arr[i++];
            else
                dest[idx++] = arr[k++];
        } else {
            if (arr[j] <= arr[k])
                dest[idx++] = arr[j++];
            else
                dest[idx++] = arr[k++];
        }
    }

    while (i <= mid1 && j <= mid2) {
        if (arr[i] <= arr[j])
            dest[idx++] = arr[i++];
        else
            dest[idx++] = arr[j++];
    }

    while (j <= mid2 && k <= r) {
        if (arr[j] <= arr[k])
            dest[idx++] = arr[j++];
        else
            dest[idx++] = arr[k++];
    }

    while (i <= mid1 && k <= r) {
        if (arr[i] <= arr[k])
            dest[idx++] = arr[i++];
        else
            dest[idx++] = arr[k++];
    }

    while (i <= mid1) dest[idx++] = arr[i++];
    while (j <= mid2) dest[idx++] = arr[j++];
    while (k <= r) dest[idx++] = arr[k++];

    for (int p = 0; p < n; p++) arr[l + p] = dest[p];
    free(dest);
}

void m_sort3(int arr[], int l, int r) {
    if (l >= r) return;

    if (r - l == 1) {
        if (arr[l] > arr[r]) {
            int temp = arr[l];
            arr[l] = arr[r];
            arr[r] = temp;
        }
        return;
    }

    int n = r - l + 1;
    int size1 = n / 3;
    int size2 = n / 3;

    int mid1 = l + size1 - 1;
    int mid2 = mid1 + size2;

    m_sort3(arr, l, mid1);
    m_sort3(arr, mid1 + 1, mid2);
    m_sort3(arr, mid2 + 1, r);

    merge3(arr, l, mid1, mid2, r);
}

void gen_arr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

int main(void) {
    FILE *file = fopen("merge_sort_results.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(file, "Array_Size(N), 2-Way_Merge_Time(s), 3-Way_Merge_Time(s)\n");

    int max_n = 2000000;
    int step = 200000;

    for (int n = step; n <= max_n; n += step) {
        int *arr1 = (int *)malloc(n * sizeof(int));
        int *arr2 = (int *)malloc(n * sizeof(int));

        gen_arr(arr1, n);
        memcpy(arr2, arr1, n * sizeof(int));

        clock_t start2 = clock();
        m_sort2(arr1, 0, n - 1);
        clock_t end2 = clock();
        double time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;

        clock_t start3 = clock();
        m_sort3(arr2, 0, n - 1);
        clock_t end3 = clock();
        double time3 = ((double)(end3 - start3)) / CLOCKS_PER_SEC;

        fprintf(file, "%d, %f, %f\n", n, time2, time3);

        free(arr1);
        free(arr2);
    }

    fclose(file);
    printf("Benchmark results written to merge_sort_results.csv\n");
    return 0;
}
