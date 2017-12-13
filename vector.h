#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef struct {
    double x, y, z;
} Vector;


Vector vect_make(double x, double y, double z);

double vect_abs2(Vector a);

double vect_abs(Vector a);

double vect_dot_prod(Vector a, Vector b);

Vector vect_sum(Vector a, Vector b);

Vector vect_diff(Vector a, Vector b);

//Vector vect_proj(Vector a, Vector b);

Vector vect_scale(Vector a, double k);

// лежит ли конец вектора a в данном объеме
int vect_in_area(Vector a, Vector from, Vector to);

#endif // VECTOR_H_INCLUDED
