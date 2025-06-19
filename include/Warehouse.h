#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Stack.h"
#include "utils.h"

class Package;

typedef struct n
{
    int id;
    Stack section;
} Neighbor;

class Warehouse
{
    private:
        int id, qtd_neighbors;
        Neighbor* neighbors; // Array of the current warehouse neighbors

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
        int* PopPackages(int section, int _clock_time, int _removal_cost, Package*& packages, int qtd_packages, int& qtd_ids, Transport* t);
    
    friend class Package;
};

#include "Package.h"

#endif 
