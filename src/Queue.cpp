#include <iostream>

#include "../include/Queue.h"

// Default constructor
Queue::Queue()
{
    front = nullptr;
    back = nullptr;
}

// Destructor
Queue::~Queue() { Clear(); }

void Queue::Enqueue(int _key)
// Adds a new element at the end of the queue
{
    Cell* new_cel = new Cell;
    new_cel->key = _key;
    new_cel->prox = nullptr;

    if (back) back->prox = new_cel; // If queue is not empty
    back = new_cel;

    if (!front) front = back; // If queue was empty
}

int Queue::Dequeue()
// Removes and returns the element at the front
{
    if (!front) 
        throw std::out_of_range("Queue is empty, cannot dequeue.");
    
    int removed_key = front->key;
    Cell* aux = front;
    front = front->prox;

    if (!front) back = nullptr; // Queue became empty

    delete aux;
    return removed_key;
}

void Queue::Clear()
// Removes all elements
{ 
    while (front) Dequeue(); 
}

bool Queue::IsEmpty()
// Returns true if queue is empty
{ 
    return !front; 
}