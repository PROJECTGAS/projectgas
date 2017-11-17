#include <stdio.h>
#include <stdlib.h>
#include "measurement.h"
#include "engine.h"
#include "vector.h"
#include "utils.h"

double measure_total_energy() {
    int i;
    double energy = 0;
    for (i = 0; i < mols_num; i++) energy += 0.5 * mols[i].m * vect_abs2(mols[i].v);
    return energy;
}

double measure_vmean() {
    int i;
    double vsum = 0;
    for (i = 0; i < mols_num; i++) vsum += vect_abs(mols[i].v);
    return vsum / mols_num;

}

int *distrib_v(double alpha, int n) {
    int i;
    double *vs = malloc(mols_num * sizeof(double));
    for (i = 0; i < mols_num; i++) vs[i] = vect_abs(mols[i].v);
    qsort(vs, mols_num, sizeof(double), &doublecmp);
    int *dist = calloc(n, sizeof(int));
    double vmax = alpha * vs[mols_num - 1], dv = vmax / n;
    for (i = 0; i < mols_num && vs[i] <= vmax; i++) {
        int k = vs[i] / dv;
        dist[k != n ? k : n -1]++;
    }
    free(vs);
    return dist;
}

void print_distrib_v(double alpha, int n, int slen) {
    int *dist = distrib_v(alpha, n);
    int i, max = 0;
    for (i = 0; i < n; i++) if (dist[i] > max) max = dist[i];
    for (i = 0; i < n; i++) {
        int k = ((double) slen) * dist[i] / max + 0.5, j;
        if (k > slen) k = slen;
        for (j = 0; j < k; j++) printf("#");
        printf("\n");
    }
    free(dist);
}

void start_measurement() {
    measurement_start_time = current_time;
    normal_momentum_to_walls = 0;
    collisions_with_walls = 0;
    collisions_between_mols = 0;
}
