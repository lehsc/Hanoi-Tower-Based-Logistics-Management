#include <iostream>

#include "../include/Queue.h"

Queue::Queue()
{
    front = nullptr;
    back = nullptr;
}

Queue::~Queue() { Clear(); }

void Queue::Enqueue(int _key)
//
{
    Cell* new_cel = new Cell;
    new_cel->key = _key;
    new_cel->prox = nullptr;

    // If the queue is not empty, link the current back to the new element
    if (back) back->prox = new_cel;
    back = new_cel;  // Move the back pointer to the new element

    // If the queue was empty (front is nullptr), set front to point to the new element
    if (!front) front = back;
}

int Queue::Dequeue()
//
{
    if (!front) 
        throw std::out_of_range("Queue is empty, cannot dequeue.");
    
    int removed_key = front->key;
    Cell* aux = front;
    front = front->prox;

    // If the queue becomes empty after the dequeue, reset back to nullptr
    if (!front)
        back = nullptr;

    delete aux;
    return removed_key;
}

void Queue::Clear() { while (front) Dequeue(); }

bool Queue::IsEmpty() { return !front; }