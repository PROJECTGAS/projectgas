#ifndef MEASUREMENT_H_INCLUDED
#define MEASUREMENT_H_INCLUDED

double measure_total_energy();

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

#endif // MEASUREMENT_H_INCLUDED
