#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <stdlib.h>
#include <math.h>

struct particl_
{
    double x, y, z;
    double vx, vy, vz;
};

typedef struct particl_ particl;

static int num;               // число молекул
static double tem;            // температура (в кельвинах)
static double mass;           // масса (любые удобные единицы ; давление будет в соответствующих единицах
                              // т.е для mass в КГ, давление в ПА, в мг -> давление в мПА и тд.

static short atoms;           // число атомов в молекуле
static double k_div_m;        // отношение постоянной Больцмана к массе (ВАЖНО! размерность ДЖ/(К*КГ) )
static double bx, by, bz;     // размеры сосуда (в метрах)
static particl* container;    // массив частиц


// инициализация ; возвращает 1 в случае ошибки
// запускать в самом начале
int init_sys(int num_, double mass_, short atoms_, double boltzman_);

// задается температура и размеры сосуда, вычисление скоростей молекул
// этой функцией можно изменить температуру и размер сосуда в любой момент программы
void set_params(double tem_, double bx_, double by_, double bz);

// устанавливает случайные положения молекул
void set_rand_pos(void);

// возвращает среднее давление за промежуток времени, пересчитывает скорости и координаты частиц
// время time в секундах
double update_sys(double time);

// освобождает выделенную память
void close_sys(void);







#endif // ENGINE_H_INCLUDED
