#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "vector.h"

enum CollisionObject {
    WALL_X1 = -1, WALL_Y1 = -2, WALL_Z1 = -3, WALL_X2 = -4, WALL_Y2 = -5, WALL_Z2 = -6, NONE = -7
};

typedef struct _Molecule {
    Vector r, v;
    double R, m;
    int next_collision_obj;
    double next_collision_time;
    int queue_pos;
} Molecule;

Molecule *mols;

// ������� ������������ (�������� ����)
// queue[0] - ����� ��������, ������� ����� ���������� ��� ��������� ������ step()
int *queue;

// �� ��������� ������ �� ����� �������������
int mols_num, queue_length;

double current_time, measurement_start_time;

// � ������� ������ start_measurement()
double normal_momentum_to_walls;
int collisions_between_mols, collisions_with_walls;

// ������� ������
double sx, sy, sz;

// ���������� ��������� ������������
// ������������ ����� ���� "���������", ����� mols[queue[0]].next_collision_obj == NONE
// �� ��������, ���� ��� �������� ����������
void step();

// ������������� � ����������� ������� m � ��������� R
// ���������� 1 ��� ������
int init(int _num, double R, double m, double vx_max, double _sx, double _sy, double _sz);

#endif // ENGINE_H_INCLUDED
