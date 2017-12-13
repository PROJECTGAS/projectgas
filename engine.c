#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "engine.h"
#include "vector.h"
#include "utils.h"

void queue_move_up(int m) {
    int i, j;
    for (i = mols[m].queue_pos; i > 0 &&
            mols[queue[j = (i - 1) / 2]].next_collision_time > mols[m].next_collision_time; i = j) {
        queue[i] = queue[j];
        mols[queue[j]].queue_pos = i;
    }
    queue[i] = m;
    mols[m].queue_pos = i;
}

void queue_move_down(int m) { // uses queue_length
    int i, j;
    for (i = mols[m].queue_pos; i * 2 + 2 < queue_length; i = j) {
        j = mols[queue[2 * i + 1]].next_collision_time < mols[queue[2 * i + 2]].next_collision_time ?
                2 * i + 1 : 2 * i + 2;
        if (mols[m].next_collision_time > mols[queue[j]].next_collision_time) {
            queue[i] = queue[j];
            mols[queue[j]].queue_pos = i;
        } else break;
    }
    if (i * 2 + 2 == queue_length &&
            mols[m].next_collision_time > mols[queue[j = 2 * i + 1]].next_collision_time) {
        queue[i] = queue[j];
        mols[queue[j]].queue_pos = i;
        i = j;
    }
    queue[i] = m;
    mols[m].queue_pos = i;
}

void queue_put(int m) {
    mols[m].queue_pos = queue_length;
    queue[queue_length++] = m;
    queue_move_up(m);
}

void queue_update(int m) {
    queue_move_up(m);
    queue_move_down(m);
}

void update_min_wall_collision_time
(double r, double v, double R, double s, int coord_id, int *wall_id, double *tmin) {
    if (v != 0) {
        double t;
        if (v > 0) {
            t = (s - r - R) / v;
            if (t < *tmin) {
                (*tmin) = t;
                (*wall_id) = coord_id + 3;
            }
        } else {
            t = (R - r) / v;
            if (t < *tmin) {
                (*tmin) = t;
                (*wall_id) = coord_id;
            }
        }
    }
}

void find_next_collision(int m) {
    // со стенками
    double wall_tmin = INF;
    int wall_id;
    update_min_wall_collision_time(mols[m].r.x, mols[m].v.x, mols[m].R, sx, 0, &wall_id, &wall_tmin);
    update_min_wall_collision_time(mols[m].r.y, mols[m].v.y, mols[m].R, sy, 1, &wall_id, &wall_tmin);
    update_min_wall_collision_time(mols[m].r.z, mols[m].v.z, mols[m].R, sz, 2, &wall_id, &wall_tmin);

    // с другими молекулами
    double mol_tmin = INF;
    int mol_id, i;
    for (i = 0; i < mols_num; i++) {
        Vector v = vect_diff(mols[i].v, mols[m].v);
        double v2 = vect_abs2(v);
        if (v2 != 0) {
            Vector r = vect_diff(mols[i].r, mols[m].r);
            double rv = vect_dot_prod(r, v);
            double d = sqr(rv) - v2 * (vect_abs2(r) - sqr(mols[i].R + mols[m].R));
            if (d > 0) {
                double t = (-rv - sqrt(d)) / v2;
                if (t > 0 && t < mol_tmin) {
                    mol_tmin = t;
                    mol_id = i;
                }
            }
        }
    }

    if (mol_tmin < wall_tmin) {
        mols[m].next_collision_obj = mol_id;
        mols[m].next_collision_time = mol_tmin + current_time;
    } else {
        mols[m].next_collision_obj = -1 - wall_id;
        mols[m].next_collision_time = wall_tmin + current_time;
    }
}

// не работает, если все молекулы неподвижны
void step() {
    int m = queue[0];

    double dt = mols[m].next_collision_time - current_time;
    int i;
    for (i = 0; i < mols_num; i++) mols[i].r = vect_sum(mols[i].r, vect_scale(mols[i].v, dt));
    current_time = mols[m].next_collision_time;

    if (mols[m].next_collision_obj >= 0) {
        collisions_between_mols++;

        int m2 = mols[m].next_collision_obj;
        Vector r = vect_diff(mols[m2].r, mols[m].r);

        // (изменение импульса 1 в проекции на r) / (масса 1 * масса 2 * модуль r)
        double delta = 2 * vect_dot_prod(vect_diff(mols[m2].v, mols[m].v), r) /
                    ((mols[m].m + mols[m2].m) * vect_abs2(r));

        mols[m].v = vect_sum(mols[m].v, vect_scale(r, delta * mols[m2].m));
        mols[m2].v = vect_sum(mols[m2].v, vect_scale(r, -delta * mols[m].m));

        for (i = 0; i < mols_num; i++)
            if (mols[i].next_collision_obj == m || mols[i].next_collision_obj == m2)
                mols[i].next_collision_obj = NONE;

        find_next_collision(m2);
        queue_update(m2);
    } else if (mols[m].next_collision_obj != NONE) {
        collisions_with_walls++;

        switch ((-1 - mols[m].next_collision_obj) % 3) {
        case 0:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.x);
            mols[m].v.x = -mols[m].v.x;
            break;
        case 1:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.y);
            mols[m].v.y = -mols[m].v.y;
            break;
        case 2:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.z);
            mols[m].v.z = -mols[m].v.z;
            break;
        }

        for (i = 0; i < mols_num; i++)
            if (mols[i].next_collision_obj == m) mols[i].next_collision_obj = NONE;
    }

    find_next_collision(m);
    queue_update(m);
}



void step_inf_area(int nowall) {
    int m = queue[0];

    double dt = mols[m].next_collision_time - current_time;
    int i;
    for (i = 0; i < mols_num; i++) mols[i].r = vect_sum(mols[i].r, vect_scale(mols[i].v, dt));
    current_time = mols[m].next_collision_time;

    if (mols[m].next_collision_obj >= 0) {
        collisions_between_mols++;

        int m2 = mols[m].next_collision_obj;
        Vector r = vect_diff(mols[m2].r, mols[m].r);

        // (изменение импульса 1 в проекции на r) / (масса 1 * масса 2 * модуль r)
        double delta = 2 * vect_dot_prod(vect_diff(mols[m2].v, mols[m].v), r) /
                    ((mols[m].m + mols[m2].m) * vect_abs2(r));

        mols[m].v = vect_sum(mols[m].v, vect_scale(r, delta * mols[m2].m));
        mols[m2].v = vect_sum(mols[m2].v, vect_scale(r, -delta * mols[m].m));

        for (i = 0; i < mols_num; i++)
            if (mols[i].next_collision_obj == m || mols[i].next_collision_obj == m2)
                mols[i].next_collision_obj = NONE;

        find_next_collision(m2);
        queue_update(m2);
    } else if (mols[m].next_collision_obj != NONE) {
        collisions_with_walls++;

        switch ((-1 - mols[m].next_collision_obj) % 3) {
        case 0:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.x);
            if (nowall != 1)
                mols[m].v.x = -mols[m].v.x;
            else if (mols[m].next_collision_obj == -1)
                    mols[m].r.x = sx;
                else
                    mols[m].r.x = 0;
            break;
        case 1:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.y);
            if (nowall != 2)
                mols[m].v.y = -mols[m].v.y;
            else if (mols[m].next_collision_obj == -2)
                    mols[m].r.y = sy;
                else
                    mols[m].r.y = 0;
            break;
        case 2:
            normal_momentum_to_walls += 2.0 * absd(mols[m].v.z);
            if (nowall != 3)
                mols[m].v.z = -mols[m].v.z;
            else if (mols[m].next_collision_obj == -3)
                    mols[m].r.z = sz;
                else
                    mols[m].r.z = 0;
            break;
        }

        for (i = 0; i < mols_num; i++)
            if (mols[i].next_collision_obj == m) mols[i].next_collision_obj = NONE;
    }

    find_next_collision(m);
    queue_update(m);
}




// TODO change zone filling
int set_molecules(int _num, double R, double m, double vx_max, Vector from, Vector to) {

    if (to.x > sx || to.y > sy || to.z > sz)
    {
        LOGE("init error: going out of container\n");
        return 1;
    }

    if (queue)
        free(queue);
    queue = malloc(MAX_MOL_NUMBER * sizeof(int));
    queue_length = 0;

    mols_num += _num;
    double _sx = to.x - from.x;
    double _sy = to.y - from.y;
    double _sz = to.z - from.z;

    double a = pow(_sz * _sy * _sz / _num, 1.0 / 3.0);

    int nzones_x = ceil(_sx / a);
    int nzones_y = ceil(_sy / a);
    int nzones_z = ceil(_sz / a);

    if (_sx / nzones_x < 2 * R || _sy / nzones_y < 2 * R || _sz / nzones_z < 2 * R) {
        LOGE("init error: too big R\n");
        return 1;
    }

    int nzones = nzones_x * nzones_y * nzones_z;
    char *zones = calloc(nzones, sizeof(char));

    if (!(mols && queue && zones)) {
        LOGE("init error: memory is not allocated\n");
        return 1;
    }

    LOGV("init: locating molecules...\n");

    int i;
    srand(time(0));
    for (i = mols_num - _num; i < mols_num; i++) {
        int zx, zy, zz, j;
        do {
            zx = rand() % nzones_x;
            zy = rand() % nzones_y;
            zz = rand() % nzones_z;
            j = zx * nzones_y * nzones_z + zy * nzones_z + zz;
        } while (zones[j]);
        zones[j] = 1;

        mols[i].r.x = from.x + (1 + 2 * zx) * _sx / (2 * nzones_x) + (_sx / (2 * nzones_x) - R) *
                ((2.0 * rand()) / RAND_MAX - 1.0);
        mols[i].r.y = from.y + (1 + 2 * zy) * _sy / (2 * nzones_y) + (_sy / (2 * nzones_y) - R) *
                ((2.0 * rand()) / RAND_MAX - 1.0);
        mols[i].r.z = from.z + (1 + 2 * zz) * _sz / (2 * nzones_z) + (_sz / (2 * nzones_z) - R) *
                ((2.0 * rand()) / RAND_MAX - 1.0);

        mols[i].v.x = ((2.0 * rand()) / RAND_MAX - 1.0) * vx_max;
        mols[i].v.y = ((2.0 * rand()) / RAND_MAX - 1.0) * vx_max;
        mols[i].v.z = ((2.0 * rand()) / RAND_MAX - 1.0) * vx_max;

        mols[i].m = m;
        mols[i].R = R;
    }
    LOGV("init: finding collisions...\n");
    for (i = 0; i < mols_num; i++) {
        find_next_collision(i);
        queue_put(i);
    }
    LOGV("init: ok\n");
    free(zones);
    return 0;
}


void init(double _sx, double _sy, double _sz){
    sx = _sx;
    sy = _sy;
    sz = _sz;
    mols_num = 0;
    current_time = 0;
    mols = malloc(MAX_MOL_NUMBER * sizeof(Molecule));
    return;
}

void set_drift_speed(Vector spd, Vector from, Vector to){
    int i;
    for (i = 0; i < mols_num && vect_in_area(mols[i].r, from, to); i++)
        mols[i].v = vect_sum(mols[i].v, spd);
}



void free_all_memory(){
    if (mols)
        free(mols);
    if (queue)
        free(queue);
}



