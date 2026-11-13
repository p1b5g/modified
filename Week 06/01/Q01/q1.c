#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void prt_arr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int fnd_max(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

void fnd_12(int arr[], int n, int *first, int *second) {
    *first = arr[0];
    *second = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] > *first) {
            *second = *first;
            *first = arr[i];
        } else if (arr[i] > *second && arr[i] != *first) {
            *second = arr[i];
        }
    }
}

double fnd_mn(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return (double)sum / n;
}

void srt_arr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}

double fnd_med(int arr[], int n) {
    int *temp = calloc(n, sizeof(int));
    if (!temp) return 0.0;
    for (int i = 0; i < n; i++) temp[i] = arr[i];
    srt_arr(temp, n);

    double med;
    if (n % 2 == 0) {
        med = (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    } else {
        med = temp[n / 2];
    }
    free(temp);
    return med;
}

double fnd_std(int arr[], int n) {
    double mean = fnd_mn(arr, n);
    double var = 0.0;

    for (int i = 0; i < n; i++) {
        var += (arr[i] - mean) * (arr[i] - mean);
    }

    var /= n;
    return sqrt(var);
}

int fnd_mod(int arr[], int n) {
    int *temp = calloc(n, sizeof(int));
    int *freq = calloc(n, sizeof(int));
    if (!temp || !freq) { free(temp); free(freq); return 0; }
    int unq_cnt = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unq_cnt; j++) {
            if (temp[j] == arr[i]) { found = 1; break; }
        }
        if (!found) temp[unq_cnt++] = arr[i];
    }

    for (int i = 0; i < unq_cnt; i++) freq[i] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < unq_cnt; j++) {
            if (temp[j] == arr[i]) { freq[j]++; break; }
        }
    }

    int mode = temp[0];
    int max_frq = freq[0];
    for (int i = 1; i < unq_cnt; i++) {
        if (freq[i] > max_frq) { max_frq = freq[i]; mode = temp[i]; }
    }

    free(temp); free(freq);
    return mode;
}

int rm_dups(int arr[], int n, int res[]) {
    int unq_cnt = 0;
    for (int i = 0; i < n; i++) {
        int exists = 0;
        for (int j = 0; j < unq_cnt; j++) {
            if (res[j] == arr[i]) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            res[unq_cnt++] = arr[i];
        }
    }
    return unq_cnt;
}

void rev_arr(int arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        swap(&arr[i], &arr[n - 1 - i]);
    }
}

int part_pv(int arr[], int low, int high, int pivot) {
    int piv_idx = -1;
    for (int k = low; k <= high; k++) {
        if (arr[k] == pivot) { piv_idx = k; break; }
    }
    if (piv_idx != -1) {
        swap(&arr[piv_idx], &arr[high]);
    }

    int i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] >= arr[high]) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

int main(void) {
    int arr[] = {15, 8, 22, 8, 31, 14, 9, 22, 8, 19};
    int n = sizeof(arr) / sizeof(arr[0]);
    int pivot = 15;

    int *copy = malloc(n * sizeof(int));
    if (!copy) return 0;

    printf("Original array: ");
    prt_arr(arr, n);

    printf("Maximum: %d\n", fnd_max(arr, n));

    int first, second;
    fnd_12(arr, n, &first, &second);
    printf("First largest: %d\n", first);
    printf("Second largest: %d\n", second);

    printf("Mean: %.2f\n", fnd_mn(arr, n));
    printf("Median: %.2f\n", fnd_med(arr, n));
    printf("Standard deviation: %.2f\n", fnd_std(arr, n));
    printf("Mode: %d\n", fnd_mod(arr, n));

    int unq_cnt = rm_dups(arr, n, copy);
    printf("Array after removing duplicates: ");
    prt_arr(copy, unq_cnt);

    int *rev = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) rev[i] = arr[i];
    rev_arr(rev, n);
    printf("Reversed array: ");
    prt_arr(rev, n);

    int *part_a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) part_a[i] = arr[i];
    int p_idx = part_pv(part_a, 0, n - 1, pivot);
    printf("Partitioned array around pivot %d: ", pivot);
    prt_arr(part_a, n);
    printf("Pivot position: %d\n", p_idx);

    free(copy);
    free(rev);
    free(part_a);
    return 0;
}
