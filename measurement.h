#ifndef MEASUREMENT_H_INCLUDED
#define MEASUREMENT_H_INCLUDED
#include "vector.h"

double measure_total_energy();

// �������� ������� � ������ ������
double measure_energy_in_area(Vector from, Vector to);

//�������� �������� ������� (������ �������) �� ��� direct
double measure_area_impulse(int direct, Vector from, Vector to);

double measure_vmean();

// ������������� �� ������ ��������
// ��������� ����� �������� �� ����������, �� ������������ alpha * (���������� ��������)
// n - ���������� �������� �����������
// (���������� �������) = distrib_v()[����� �������]
int *distrib_v(double alpha, int n);

// ���������� ��������� distrib_v()
// slen - ����. ����� ������ � ������ ���������
void print_distrib_v(double alpha, int n, int slen);

// �������� �������� ������������ � �.�.
void start_measurement();

// ����� ������ � ������ ������
int number_of_mol_in_area(Vector from, Vector to);


#endif // MEASUREMENT_H_INCLUDED
