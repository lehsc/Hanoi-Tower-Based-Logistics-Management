#include <iostream>

#include "../include/Stack.h"

Stack::Stack() : qtd_cells(0) { top = nullptr; }

Stack::~Stack() { Clear(); }

int Stack::GetQtdCells() { return qtd_cells; }

void Stack::Push(int _key)
//
{
    Cell* new_cel = new Cell;
    new_cel->key = _key;
    new_cel->prox = top;
    top = new_cel;
    qtd_cells++;
}

int Stack::Pop()
//
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

void Stack::Clear() { while (top) Pop(); }