#include "utils.h"

double sqr(double x) {
    return x * x;
}

double absd(double x) {
    return x >= 0 ? x : -x;
}

int doublecmp(double *a, double *b) {
    return *a > *b ? 1 : (*a < *b ? -1 : 0);
}
