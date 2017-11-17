#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>

#define LOGV(x...) printf(x)
#define LOGE(x...) printf(x)

#define INF (1.0 / 0.0)

// вспомогательные функции
double sqr(double x);
double absd(double x);
int doublecmp(double *a, double *b);

#endif // UTILS_H_INCLUDED
