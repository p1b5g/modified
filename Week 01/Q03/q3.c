#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void bs_std(int arr[], int n, long long *cmps) {
    *cmps = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*cmps)++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bs_opt(int arr[], int n, long long *cmps) {
    *cmps = 0;
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            (*cmps)++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

int main(void) {
    int min_sz = 100;
    int max_sz = 2000;
    int step = 100;
    int trials = 5;

    int num_pts = (max_sz - min_sz) / step + 1;

    int *sizes = (int *)malloc(num_pts * sizeof(int));
    long long *std_dat = (long long *)malloc(num_pts * sizeof(long long));
    long long *opt_dat = (long long *)malloc(num_pts * sizeof(long long));

    srand((unsigned int)time(NULL));
    printf("Running simulations and buffering data...\n");

    int idx = 0;
    for (int n = min_sz; n <= max_sz; n += step) {
        long long tot_std = 0;
        long long tot_opt = 0;

        for (int t = 0; t < trials; t++) {
            int *arr_std = (int *)malloc(n * sizeof(int));
            int *arr_opt = (int *)malloc(n * sizeof(int));

            int split = n / 2;
            for (int i = 0; i < split; i++) {
                int val = rand() % 10000;
                arr_std[i] = val;
                arr_opt[i] = val;
            }

            int cur_val = 10000;
            for (int i = split; i < n; i++) {
                cur_val += (rand() % 10) + 1;
                arr_std[i] = cur_val;
                arr_opt[i] = cur_val;
            }

            long long c_std = 0, c_opt = 0;
            bs_std(arr_std, n, &c_std);
            bs_opt(arr_opt, n, &c_opt);

            tot_std += c_std;
            tot_opt += c_opt;

            free(arr_std);
            free(arr_opt);
        }

        sizes[idx] = n;
        std_dat[idx] = tot_std / trials;
        opt_dat[idx] = tot_opt / trials;
        idx++;
    }

    printf("Simulation complete. Piping data directly to gnuplot...\n");

    FILE *gp = popen("gnuplot -persistent", "w");
    if (gp) {
        fprintf(gp, "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n");
        fprintf(gp, "set output 'bubble_sort_performance.png'\n");
        fprintf(gp, "set title 'Performance Analysis: Bubble Sort Comparisons'\n");
        fprintf(gp, "set xlabel 'Array Size (n)'\n");
        fprintf(gp, "set ylabel 'Number of Comparisons'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set key left top\n");

        fprintf(gp, "plot '-' using 1:2 with linespoints lw 2 pt 7 title 'Standard (Unoptimized)', \\\n");
        fprintf(gp, "     '-' using 1:2 with linespoints lw 2 pt 7 title 'Optimized (Early Termination)'\n");

        for (int i = 0; i < num_pts; i++) {
            fprintf(gp, "%d %lld\n", sizes[i], std_dat[i]);
        }
        fprintf(gp, "e\n");

        for (int i = 0; i < num_pts; i++) {
            fprintf(gp, "%d %lld\n", sizes[i], opt_dat[i]);
        }
        fprintf(gp, "e\n");

        pclose(gp);
        printf("Success! Graph saved as 'bubble_sort_performance.png'.\n");
    }

    free(sizes);
    free(std_dat);
    free(opt_dat);

    return 0;
}
