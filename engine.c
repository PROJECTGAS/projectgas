#include "engine.h"

// параметры генератора случайных чисел
#define MAX_SEED 10000
#define STEP 100
// -----------------------------------//



// вспомогательные функции


static long count_collid(double* pos, double* v, double border)
{
    long collids = 0;
    while  ((*pos < 0) || (*pos > border))
    {
        if (*pos < 0)
            *pos = -(*pos);
        else
            *pos = 2 * border - (*pos);
        *v = -(*v);
        collids++;
    }
    return collids;
}


static double absd(double x)
{
    return (x > 0) ? x : -x;
}



static double rand_speed(void)
{
    static seed = 1;

    srand(seed);
    seed += STEP;
    if (seed > MAX_SEED)
        seed = 1;
    long double randpar = 2 * rand() / RAND_MAX - 1.0;

    double i;
    switch (atoms)
    {
    case 1:
        i = 3.0;
        break;
    case 2:
        i = 5.0;
        break;
    default:
        i = 6.0;
        break;
    }

    double avrg_spd = sqrt(i * k_div_m * tem); // средняя скорость
    double speed = avrg_spd * (1 - 1 / sqrt(2) + sqrt(2) * absd(randpar));

    return (randpar > 0) ? speed : -speed;
}


// основные функции

int init_sys(int num_, double mass_, short atoms_, double k_div_mass)
{
    //extern particl* container;
    //extern int num;
    //extern double mass;
    //extern double k_div_m;

    num = num_;
    mass = mass_;
    k_div_m = k_div_mass;
    atoms = atoms_;
    container = (particl*) malloc(num * sizeof(particl));

    if (num < 0 || mass < 0 || k_div_m < 0 || atoms < 1)
        return 1;
    else
        return 0;
}



void set_params(double tem_, double bx_, double by_, double bz_)
{
    //extern num;
    //extern particl* container;
    //extern double tem;
    //extern double bx, bx, bz;
    tem = tem_;
    bx = bx_;
    by = by_;
    bz = bz_;

    for (int i = 0; i < num; i++)
    {
        container[i].vx = rand_speed();
        container[i].vy = rand_speed();
        container[i].vz = rand_speed();
    }
}



void set_rand_pos(void)
{
    //extern num;
    //extern particl* container;
    //extern double bx, by, bz;
    static seed = 1;
    srand(seed);
    seed += STEP;
    if (seed > MAX_SEED)
        seed = 1;
    long double randpar;

    for (int i = 0; i < num; i++)
    {
        randpar = rand() / RAND_MAX;
        container[i].x = bx * randpar;
        randpar = rand() / RAND_MAX;
        container[i].y = by * randpar;
        randpar = rand() / RAND_MAX;
        container[i].z = bz * randpar;
    }
}




double update_sys(double time)
{
    //extern int num;
    //extern particl* container;
    //extern double bx, by, bz;
    long c_x, c_y, c_z;
    double f_x, f_y, f_z;


    f_x = f_y = f_z = 0;
    for (int i = 0; i < num; i++)
    {
        container[i].x += container[i].vx * time;
        container[i].y += container[i].vy * time;
        container[i].z += container[i].vz * time;

        c_x = count_collid(&(container[i].x), &(container[i].vx), bx);
        c_y = count_collid(&(container[i].y), &(container[i].vy), by);
        c_z = count_collid(&(container[i].z), &(container[i].vz), bz);

        f_x += (c_x * absd(container[i].vx) * mass) / time;
        f_y += (c_y * absd(container[i].vy) * mass) / time;
        f_z += (c_z * absd(container[i].vz) * mass) / time;
    }

    return f_x / (by * bz) + f_y / (bx * bz) + f_z / (bx * by);
}




void close_sys(void)
{
    free(container);
}


