#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "vector.h"

#define MAX_MOL_NUMBER 10000

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

// очередь столкновений (двоичная куча)
// queue[0] - номер молекулы, которая будет обработана при следующем вызове step()
int *queue;

// не совпадают только во время инициализации
int mols_num, queue_length;

double current_time, measurement_start_time;

// с момента вызова start_measurement()
double normal_momentum_to_walls;
int collisions_between_mols, collisions_with_walls;

// размеры сосуда
double sx, sy, sz;

// обработать следующее столкновение
// столкновение может быть "пустышкой", когда mols[queue[0]].next_collision_obj == NONE
// не работает, если все молекулы неподвижны
void step();

//инициализация общих параметров системы
void init(double _sx, double _sy, double _sz);

// инициализация с одинаковыми массами m и радиусами R
// возвращает 1 при ошибке
int set_molecules(int _num, double R, double m, double vx_max, Vector from, Vector to);

void free_all_memory();

#endif // ENGINE_H_INCLUDED
