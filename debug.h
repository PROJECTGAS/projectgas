#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

// напечатать очередь столкновений
void queue_print();

// проверка, что очередь правильно работает
int queue_check();

// проверка на пересечение молекул между собой и со стенками сосуда
int location_check();

#endif // DEBUG_H_INCLUDED
