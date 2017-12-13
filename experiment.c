#include <stdio.h>
#include "engine.h"
#include "measurement.h"
#include "experiment.h"
#include "utils.h"
#include "vector.h"


void Exper_Diffusion(void){
    int i, n1, n2;
    double _sx, _sy, _sz;
    double _m, _r, _mspeed;

    printf("\n///// Experiment: Diffusion factor /////\n");

    printf("Container size: ");
    scanf("%lf%lf%lf", &_sx, &_sy, &_sz);
    printf("mass, radius, max speed: ");
    scanf("%lf%lf%lf", &_m, &_r, &_mspeed);

    //число молекул в первой половине
    printf("molecul num1: ");
    scanf("%d", &n1);
    //число молекул во второй роловине
    printf("molecul num2: ");
    scanf("%d", &n2);

    //printf("molecul num2: ");

    Vector from1 = {0, 0, 0}, to1 = {_sx, _sy, _sz / 2};
    Vector from2 = {0, 0, _sz / 2}, to2 = {_sx, _sy, _sz};

    init(_sx, _sy, _sz);
    start_measurement();

    set_molecules(n1, _r, _m, _mspeed, from1, to1);
    set_molecules(n2, _r, _m, _mspeed, from2, to2);

    double n12, grad_n, D = 0;
    int mes = 0;

    for (i = 0; i < TOTAL_STEPS; i++){
        step();
        if (i % MEASURE_STEP == 0 && i > 0){
            n12 = number_of_mol_in_area(from1, to1);
            grad_n = 2.0 * abs(n1 + n2 - 2 * n12) / _sx / _sy / sqr(_sz);
            D += _m * abs(n12 - n1) / current_time / (_sx * _sy) / grad_n;
            mes++;
        }
    }

    printf("Diffusion factor: D = %lf\n", D / mes);
    printf("///// Experiment finished /////\n\n");
    free_all_memory();
    return;
}



void Exper_Thermal_conductivity(void){

    printf("\n///// Experiment: Thermal conductivity factor /////\n");

    int i, _n;
    double _sx, _sy, _sz;
    double _m, _r, max_spd1, max_spd2;

    printf("Container size: ");
    scanf("%lf%lf%lf", &_sx, &_sy, &_sz);
    printf("mass, radius: ");
    scanf("%lf%lf", &_m, &_r);
    printf("Total number of molecules: ");
    scanf("%d", &_n);

    //максимальная скорость в 1-й половине
    printf("max speed 1: ");
    scanf("%lf", &max_spd1);
    //максимальная скорость в 1-й половине
    printf("max speed 2: ");
    scanf("%lf", &max_spd2);

    Vector from1 = {0, 0, 0}, to1 = {_sx, _sy, _sz / 2};
    Vector from2 = {0, 0, _sz / 2}, to2 = {_sx, _sy, _sz};

    init(_sx, _sy, _sz);
    start_measurement();

    _n /= 2;
    set_molecules(_n, _r, _m, max_spd1, from1, to1);
    set_molecules(_n, _r, _m, max_spd2, from2, to2);

    {
    // Debug
    int n1, n2;
    n1 = number_of_mol_in_area(from1, to1);
    n2 = number_of_mol_in_area(from2, to2);
    printf("Debug: %d %d", n1, n2);
    //
    }

    double e10 = measure_energy_in_area(from1, to1);
    double e1, e2, grad_T, K = 0;
    int n1, n2, mes = 0;

    for (i = 0; i < TOTAL_STEPS; i++){
        step();
        if (i % MEASURE_STEP == 0 && i > 0){
            e1 = measure_energy_in_area(from1, to1);
            n1 = number_of_mol_in_area(from1, to1);
            e2 = measure_energy_in_area(from2, to2);
            n2 = number_of_mol_in_area(from2, to2);
            grad_T = 2.0 * absd(e1 / n1 - e2 / n2) / _sz;
            K += absd(e1 - e10) / (current_time * (_sx * _sy) * grad_T);
            mes++;

        }
    }

    printf("Thermal conductivity factor: K = %lf\n", K / mes);
    printf("///// Experiment finished /////\n\n");
    free_all_memory();
}


void Exper_Viscosity(void){
    printf("\n///// Experiment: Viscosity factor /////\n");

    double _sx, _sy, _sz;
    double _n, _m, _r, max_spd, drift_spd;

    printf("Container size: ");
    scanf("%lf%lf%lf", &_sx, &_sy, &_sz);
    printf("mass, radius: ");
    scanf("%lf%lf", &_m, &_r);
    printf("Total number of molecules: ");
    scanf("%d", &_n);
    //Максимальная хаотическая скорость
    printf("Max chaotic speed: ");
    scanf("%lf", &max_spd);
    //скорость дрейфа половины газа
    printf("Drift speed: ");
    scanf("%lf", &drift_spd);

    Vector from1 = {0, 0, 0}, to1 = {_sx, _sy, _sz / 2};
    Vector from2 = {0, 0, _sz / 2}, to2 = {_sx, _sy, _sz};

    init(_sx, _sy, _sz);
    start_measurement();

    _n /= 2;
    set_molecules(_n, _r, _m, max_spd, from1, to1);
    set_molecules(_n, _r, _m, max_spd, from2, to2);

    set_drift_speed(vect_make(drift_spd, 0, 0), from1, to1);

    double p0 = measure_area_impulse(1, from1, to1);
    double grad_v, p1, p2, ETA = 0;
    int i, n1, n2, mes = 0;

    for (i = 0; i < TOTAL_STEPS; i++){
        step_inf_area(1);
        if (i % MEASURE_STEP == 0 && i > 0){
            p1 = measure_area_impulse(1, from1, to1);
            p2 = p0 - p1;
            n1 = number_of_mol_in_area(from1, to1);
            n2 = 2 * _n - n1;
            grad_v = 2 * absd(p1 / n1 - p2 / n2) / _m / _sz;
            ETA += absd(p1 - p0) / current_time / grad_v / _sx / _sy;
            mes++;
        }
    }

    printf("Viscosity factor: ETA = %lf\n", ETA / mes);
    printf("///// Experiment finished /////\n\n");

}









