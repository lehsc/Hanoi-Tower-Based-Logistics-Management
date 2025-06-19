#include <iostream>

#include "../include/LinkedList.h"

LinkedList::LinkedList() : qtd_nodes(0) { head = nullptr; }

LinkedList::LinkedList(const LinkedList& other) : head(nullptr), qtd_nodes(0) 
// Copy constructor
{
    // If the original list is empty, there's nothing to copy
    if (other.head == nullptr) return;

    // Create the first node
    head = new Cell{other.head->key, nullptr};
    qtd_nodes++;

    // Pointers to traverse the source list and build the new one
    Cell* current_src = other.head->prox;
    Cell* current_dest = head;

    // Copy remaining nodes
    while (current_src != nullptr) {
        Cell* new_cell = new Cell{current_src->key, nullptr};
        current_dest->prox = new_cell;
        current_dest = new_cell;

        current_src = current_src->prox;
        qtd_nodes++;
    }
}

LinkedList::~LinkedList() { Clear(); }

int LinkedList::GetQtdNodes() { return qtd_nodes; }

void LinkedList::InsertAtFront(Cell*& new_cell)
//
{
    if (head != nullptr) 
        new_cell->prox = head;
    head = new_cell;
}

Cell* LinkedList::FindCell(int key, int& i) 
// Return the cell with the same key in the parameter and updates its position i
{
    Cell* aux = head;
    for (i = 0; i < qtd_nodes; i++) {
        if (aux->key == key) return aux;
        aux = aux->prox;
    }

    return nullptr;
}

Cell* LinkedList::FindAntecedent(int pos) 
//
{
    if (pos == qtd_nodes) return nullptr;
    
    int i = 0;
    Cell* aux = head;
    while (i != pos-1) {
        aux = aux->prox;
        i++;
    }
    
    return aux;
}

int LinkedList::RemoveHead()
//
{
    Cell* aux = head;
    int removed_key = aux->key;
    head = head->prox;
    delete aux;
    qtd_nodes--;
    return removed_key;
}

void LinkedList::Insert(int _key, int pos)
//
{
    if (pos > qtd_nodes || pos < 0)
        throw std::out_of_range("ERROR: Invalid index.");

    Cell* _new_cell = new Cell;
    _new_cell->key = _key;
    _new_cell->prox = nullptr;

    if (pos == 0) InsertAtFront(_new_cell);
    else {
        Cell* antecedent = FindAntecedent(pos);
        _new_cell->prox = antecedent->prox;
        antecedent->prox = _new_cell;
    }    

    qtd_nodes++;
}

int LinkedList::Remove(int pos) 
//
{
    if (pos < 0 || pos >= qtd_nodes)
        throw std::out_of_range("ERROR: Invalid index!");

    if (pos == 0) return RemoveHead();
    
    Cell* antecedent = FindAntecedent(pos);
    Cell* to_delete = antecedent->prox;
    int removed_key = to_delete->key;

    antecedent->prox = to_delete->prox;
    delete to_delete;

    qtd_nodes--;
    return removed_key;
}

int LinkedList::Search(int pos) 
//
{
    if (pos >= qtd_nodes || pos < 0)
        throw std::out_of_range("ERROR: Invalid index.");
    
    int count = 0;
    Cell* current_cell = head; 
    while (count != pos)
    {
        current_cell = current_cell->prox;
        count++;
    }   

    return current_cell->key;
}

void LinkedList::Clear() { while (head) RemoveHead(); }

void LinkedList::Print()
//
{
    Cell* current = head;
    while (current)
    {
        printf("%d ", current->key);
        current = current->prox;
    }    

    printf("\n");
}