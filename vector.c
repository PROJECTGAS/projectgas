#include <math.h>
#include "vector.h"

double vect_abs2(Vector a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

double vect_abs(Vector a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

double vect_dot_prod(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector vect_sum(Vector a, Vector b) {
    return (Vector) { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector vect_diff(Vector a, Vector b) {
    return (Vector) { a.x - b.x, a.y - b.y, a.z - b.z };
}

//Vector vect_proj(Vector a, Vector b);

Vector vect_scale(Vector a, double k) {
    return (Vector) { a.x * k, a.y * k, a.z * k };
}
