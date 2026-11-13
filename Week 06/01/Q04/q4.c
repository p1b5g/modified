#include <stdio.h>
#include <stdlib.h>

void rev_seg(int arr[], int left, int right) {
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

void srt_rev(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int pos = -1;
        for (int j = i; j < n; j++) {
            if (arr[j] == i + 1) {
                pos = j;
                break;
            }
        }

        if (pos != -1 && pos != i) {
            rev_seg(arr, i, pos);
            printf("Reverse segment [%d, %d]\n", i, pos);
        }
    }
}

void prt_arr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int p[] = {1, 4, 3, 2, 5};
    int n = sizeof(p) / sizeof(p[0]);

    printf("Permutation before sort: ");
    prt_arr(p, n);

    srt_rev(p, n);

    printf("Sorted permutation: ");
    prt_arr(p, n);
    return 0;
}
