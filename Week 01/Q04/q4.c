#include <stdio.h>
#include <stdlib.h>

void toh(int n, char src, char aux, char dst, long long *moves) {
    if (n == 0) {
        return;
    }
    toh(n - 1, src, dst, aux, moves);
    (*moves)++;
    toh(n - 1, aux, src, dst, moves);
}

int main(void) {
    printf("Calculating ToH moves and generating plot...\n");

    FILE *gp = popen("gnuplot -persistent", "w");
    if (gp != NULL) {
        fprintf(gp, "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n");
        fprintf(gp, "set output 'toh_moves.png'\n");
        fprintf(gp, "set title 'Towers of Hanoi: Moves Required vs Number of Discs' font ',14' \n");
        fprintf(gp, "set xlabel 'Number of Discs (n)' font ',12' \n");
        fprintf(gp, "set ylabel 'Total Moves (Log Scale)' font ',12' \n");
        fprintf(gp, "set logscale y 10\n");
        fprintf(gp, "set xrange [1:16]\n");
        fprintf(gp, "set xtics 1,1,16\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot '-' with linespoints linewidth 2 pointtype 7 pointsize 1.5 linecolor rgb 'blue' title 'Moves = 2^n - 1'\n");
    }

    for (int n = 1; n <= 16; n++) {
        long long moves = 0;
        toh(n, 'A', 'B', 'C', &moves);
        printf("n = %2d \t Moves = %lld\n", n, moves);
        if (gp != NULL) {
            fprintf(gp, "%d %lld\n", n, moves);
        }
    }

    if (gp != NULL) {
        fprintf(gp, "e\n");
        pclose(gp);
    }

    return 0;
}
