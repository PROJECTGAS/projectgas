#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef struct {
    double x, y, z;
} Vector;

double vect_abs2(Vector a);

double vect_abs(Vector a);

double vect_dot_prod(Vector a, Vector b);

Vector vect_sum(Vector a, Vector b);

Vector vect_diff(Vector a, Vector b);

//Vector vect_proj(Vector a, Vector b);

Vector vect_scale(Vector a, double k);

#endif // VECTOR_H_INCLUDED
