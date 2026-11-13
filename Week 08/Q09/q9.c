#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef unsigned long long ull;

#define CAP_INI 1024
#define MAX_STP 1000000

ull col_stp(ull n) {
    if (n % 2 == 0) return n / 2;

    if (n > (ULLONG_MAX - 1) / 3) {
        fprintf(stderr, "Overflow detected at n = %llu\n", n);
        return ULLONG_MAX;
    }
    return 3 * n + 1;
}

ull *sgl_trj(ull start, int *steps, ull *max_val) {
    int cap   = CAP_INI;
    ull *traj = (ull *)malloc(cap * sizeof(ull));
    if (!traj) return NULL;

    int count = 0;
    ull cur   = start;
    *max_val  = start;

    while (cur != 1 && count < MAX_STP) {
        if (count >= cap - 1) {
            cap *= 2;
            ull *tmp = (ull *)realloc(traj, cap * sizeof(ull));
            if (!tmp) { free(traj); return NULL; }
            traj = tmp;
        }

        traj[count++] = cur;
        if (cur > *max_val) *max_val = cur;

        ull next = col_stp(cur);
        if (next == ULLONG_MAX) { free(traj); return NULL; }
        cur = next;
    }

    traj[count++] = 1;
    *steps = count - 1;
    return traj;
}

void anl_int(ull a, ull b) {
    ull lng_n   = a;
    int max_stp = 0;
    ull glb_max = 0;
    ull glb_n   = a;

    printf("\n%-12s %-12s %-20s\n", "n", "Steps", "Max Value Reached");

    for (ull n = a; n <= b; n++) {
        int steps   = 0;
        ull max_val = n;
        ull cur     = n;

        while (cur != 1 && steps < MAX_STP) {
            ull next = col_stp(cur);
            if (next == ULLONG_MAX) break;
            cur = next;
            if (cur > max_val) max_val = cur;
            steps++;
        }

        printf("%-12llu %-12d %-20llu\n", n, steps, max_val);

        if (steps > max_stp) {
            max_stp = steps;
            lng_n   = n;
        }
        if (max_val > glb_max) {
            glb_max = max_val;
            glb_n   = n;
        }
    }

    printf("\nInterval Statistics [%llu, %llu]:\n", a, b);
    printf("  Longest trajectory : starts at n = %llu (%d steps)\n",
           lng_n, max_stp);
    printf("  Highest value seen : %llu  (starting from n = %llu)\n",
           glb_max, glb_n);
}

int main(void) {
    ull start = 19;
    int steps;
    ull max_val;
    ull *traj = sgl_trj(start, &steps, &max_val);

    if (traj) {
        printf("Trajectory for n = %llu\n", start);
        printf("Steps to reach 1 : %d\n", steps);
        printf("Maximum value    : %llu\n", max_val);
        for (int i = 0; i <= steps; i++) {
            printf("  Step %-3d : %llu\n", i, traj[i]);
        }
        free(traj);
    }

    ull a = 1, b = 15;
    anl_int(a, b);

    return 0;
}
