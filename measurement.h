#ifndef MEASUREMENT_H_INCLUDED
#define MEASUREMENT_H_INCLUDED
#include "vector.h"

double measure_total_energy();

// измеряет энергию в данном объеме
double measure_energy_in_area(Vector from, Vector to);

double measure_vmean();

// распределение по модулю скорости
// посчитаны будут молекулы со скоростями, не превышающими alpha * (наибольшая скорость)
// n - количество столбцов гистограммы
// (количество молекул) = distrib_v()[номер столбца]
int *distrib_v(double alpha, int n);

// напечатать результат distrib_v()
// slen - макс. длина строки в выводе программы
void print_distrib_v(double alpha, int n, int slen);

// обнулить счётчики столкновений и т.п.
void start_measurement();

// число частиц в данном объеме
int number_of_mol_in_area(Vector from, Vector to);


#endif // MEASUREMENT_H_INCLUDED
