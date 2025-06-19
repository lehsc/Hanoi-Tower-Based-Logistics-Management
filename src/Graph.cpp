/*
---------------------------------------------------------------------------------------
  File        : Graph.cpp
  Description : Implementation of the Graph class, which models the network of warehouses.
---------------------------------------------------------------------------------------
*/

#include <iostream>

#include "../include/Graph.h"
#include "../include/Package.h"

// Default constructor
Graph::Graph() : max_warehouses(0) {}

// Parameterized constructor
Graph::Graph(int _max_warehouses, int removal_cost, Transport _t) 
: qtd_warehouses(0), max_warehouses(_max_warehouses), package_removal_cost(removal_cost), t(_t) 
{ 
    warehouses = new Warehouse*[_max_warehouses]; 
}

// Destructor
Graph::~Graph() { delete[] warehouses; }

// Get methods
int Graph::GetMaxWarehouses() { return max_warehouses; }
int Graph::GetRemovalCost() { return package_removal_cost; }
Transport Graph::GetTransport() { return t; }

Warehouse* Graph::GetWarehouse (int warehouse_id) 
// Returns a pointer to the warehouse with the given ID
{
    for (int i = 0; i < qtd_warehouses; i++)
    {
        if (warehouses[i]->GetId() == warehouse_id)
            return warehouses[i];
    }
    
    return nullptr;
}

void Graph::AddWarehouse(int _id, int _qtd_neighbors, int* _neighbors)
// Adds a new warehouse to the graph with its neighbors (sections)
{
    warehouses[qtd_warehouses] = new Warehouse(_id, _qtd_neighbors);
    warehouses[qtd_warehouses]->AddNeighbors(_neighbors);
    qtd_warehouses++;
}

void Graph::PrintWarehouses() 
// Prints the warehouses and their neighbors 
{
    for (int i = 0; i < qtd_warehouses; i++){
        std::cout << "Id: " << warehouses[i]->GetId();
        std::cout << ", Neighbors: ";
        warehouses[i]->PrintNeighbors();        
    }    
}

void Graph::StorePackage(Package* p, int warehouse_id)
// Stores a package in the specified warehouse
{ 
    warehouses[warehouse_id]->PushPackage(p); 
}

int* Graph::RemovePackages(int org_warehouse_id, int dst_warehouse_id, int& clock_time, 
            Package*& packages, int qtd_packages, int& qtd_ids)
// Removes packages from a specific warehouse section
{ 
    return warehouses[org_warehouse_id]->PopPackages(dst_warehouse_id, clock_time, 
        package_removal_cost, packages, qtd_packages, qtd_ids, &t); 
}