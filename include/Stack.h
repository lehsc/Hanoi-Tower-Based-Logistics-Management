#ifndef STACK_H
#define STACK_H

#include "Cell.h"

class Stack
{
    private:
        Cell* top;
        int qtd_cells;

    public:
        Stack();
        ~Stack();

        int GetQtdCells();

        void Push(int _key);
        int Pop();
        void Clear();
};

#endif