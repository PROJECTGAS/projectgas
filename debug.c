#include <stdio.h>
#include "debug.h"
#include "engine.h"
#include "vector.h"

void queue_print() {
    printf("---------- queue ----------\n");
    int i;
    for (i = 0; i < queue_length; i++) printf("%d: mol %d, coll_t %f, coll_obj %d\n",
                                          i, queue[i], mols[queue[i]].next_collision_time,
                                          mols[queue[i]].next_collision_obj);
    printf("-------- queue end --------\n");
}

int queue_check() {
    int i, fail = 0;
    for (i = 1; i < queue_length; i++) {
        if (mols[queue[i]].next_collision_time < mols[queue[(i - 1) / 2]].next_collision_time) {
            if (!fail) {
                printf("queue_check: wrong leaves:");
                fail = 1;
            }
            printf(" %d", i);
        }
    }
    if (fail) printf("\n");
    else printf("queue_check: ok\n");
    return fail;
}

int location_check() {
    int i, j, fail = 0;
    for (i = 0; i < mols_num; i++) {
        if (mols[i].r.x < mols[i].R) {
            printf("location_check: %d in wall 0\n", i);
            fail = 1;
        }
        if (mols[i].r.y < mols[i].R) {
            printf("location_check: %d in wall 1\n", i);
            fail = 1;
        }
        if (mols[i].r.z < mols[i].R) {
            printf("location_check: %d in wall 2\n", i);
            fail = 1;
        }
        if (mols[i].r.x > sx - mols[i].R) {
            printf("location_check: %d in wall 3\n", i);
            fail = 1;
        }
        if (mols[i].r.y > sy - mols[i].R) {
            printf("location_check: %d in wall 4\n", i);
            fail = 1;
        }
        if (mols[i].r.z > sz - mols[i].R) {
            printf("location_check: %d in wall 5\n", i);
            fail = 1;
        }

        for (j = i + 1; j < mols_num; j++)
            if (vect_abs(vect_diff(mols[j].r, mols[i].r)) < mols[j].R + mols[i].R) {
                printf("location_check: %d (%f, %f, %f) intersects with %d (%f, %f, %f)\n", i,
                       mols[i].r.x, mols[i].r.y, mols[i].r.z, j, mols[j].r.x, mols[j].r.y, mols[j].r.z);
                fail = 1;
            }
    }
    if (!fail) printf("location_check: ok\n");
    return fail;
}
