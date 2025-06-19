#ifndef CELL_H
#define CELL_H

typedef struct cell_c
{
    int key;
    cell_c* prox;
} Cell;

#endif