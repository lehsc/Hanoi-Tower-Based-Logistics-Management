#include <iostream>

#include "../include/Stack.h"

// Default constructor
Stack::Stack() : qtd_cells(0) { top = nullptr; }

// Destructor
Stack::~Stack() { Clear(); }

int Stack::GetQtdCells()
// Returns number of elements in the stack
{ 
    return qtd_cells; 
}

void Stack::Push(int _key)
// Pushes a new element onto the stack
{
    Cell* new_cel = new Cell;
    new_cel->key = _key;
    new_cel->prox = top;
    top = new_cel;
    qtd_cells++;
}

int Stack::Pop()
// Removes and returns the top element
{   
    if (!top) 
    {
        std::cerr << "Erro: Pilha vazia!" << std::endl;
        exit(EXIT_FAILURE);
    }  

    int removed_key = top->key; 
    Cell* aux = top;
    top = top->prox;
    delete aux;
    qtd_cells--;
    return removed_key;
}

void Stack::Clear()
// Removes all elements from the stack
{ 
    while (top) Pop(); 
}