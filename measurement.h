#ifndef MEASUREMENT_H_INCLUDED
#define MEASUREMENT_H_INCLUDED

double measure_total_energy();

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

#endif // MEASUREMENT_H_INCLUDED
