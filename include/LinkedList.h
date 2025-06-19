#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Cell.h"

class LinkedList
{
    private:
        Cell* head;
        int qtd_nodes;
        void InsertAtFront(Cell*& new_cell);
        Cell* FindCell(int key, int& i); 
        Cell* FindAntecedent(int pos);
        int RemoveHead();

    public:
        LinkedList();
        LinkedList(const LinkedList& other);
        ~LinkedList();

        int GetQtdNodes();
        
        void Insert(int _key, int pos);
        int Remove(int pos);
        int Search(int pos);
        void Clear();
        void Print();
};

#endif