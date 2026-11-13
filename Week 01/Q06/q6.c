#include <stdio.h>
#include <stdlib.h>

int cnt_dup(int arr[], int n) {
    if (n <= 1) return 0;

    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    int *freq = (int *)calloc(max_val + 1, sizeof(int));
    if (freq == NULL) {
        return -1;
    }

    int dup_cnt = 0;

    for (int i = 0; i < n; i++) {
        if (freq[arr[i]] > 0) {
            dup_cnt++;
        }
        freq[arr[i]]++;
    }

    free(freq);

    return dup_cnt;
}

int main(void) {
    int arr[] = {14, 7, 23, 14, 9, 42, 7, 18, 14, 5, 23, 31, 9, 60, 11};
    int sz_arr = sizeof(arr) / sizeof(arr[0]);

    int tot_dup = cnt_dup(arr, sz_arr);

    printf("Result: Found %d duplicate(s) in the array.\n", tot_dup);

    return 0;
}
