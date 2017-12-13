#include <math.h>
#include "vector.h"

Vector vect_make(double x, double y, double z){
    Vector a = {x, y, z};
    return a;
}


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


int vect_in_area(Vector a, Vector from, Vector to){
    if (a.x > to.x || a.x < from.x) return 0;
    if (a.y > to.y || a.x < from.y) return 0;
    if (a.x > to.z || a.x < from.z) return 0;
    return 1;
}
