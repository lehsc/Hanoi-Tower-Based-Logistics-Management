#ifndef QUEUE_H
#define QUEUE_H

#include "Cell.h"

class Queue
{
    private:
        Cell* front;
        Cell* back;
    
    public:
        Queue();
        ~Queue();

        void Enqueue(int _key);
        int Dequeue();
        void Clear();
        bool IsEmpty();
};

#endif