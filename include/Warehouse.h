#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Stack.h"
#include "utils.h"

class Package;

typedef struct n
{ // Represents a neighbor warehouse (section)
    int id;
    Stack section; // Stack of packages destined for this neighbor
} Neighbor;

class Warehouse
{
    private:
        int id, qtd_neighbors;
        Neighbor* neighbors; // Array of neighbor warehouses (sections)

    public:
        Warehouse();
        Warehouse(int _id, int _qtd_neighbors);
        ~Warehouse();

        int GetId ();      
        int GetQtdNeighbors ();
        Neighbor* FindNeighbor (int neighbor_id);

        void AddNeighbors (int* _neighbors_ids);
        void PrintNeighbors ();  

        void PushPackage(Package* p);
    
    friend class Package;
};

#include "Package.h"

#endif 
