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
            Package*& packages, int& qtd_ids)
// Removes packages from a specific warehouse section
{ 
    Stack temp; // Auxiliary stack
    Neighbor* neighbor = warehouses[org_warehouse_id]->FindNeighbor(dst_warehouse_id);
    Package* p;
    int package_id, i = 0, qtd_removed_packages = 0;

    if (neighbor)
    {
        qtd_ids = neighbor->section.GetQtdCells();
        int* packages_ids = new int[qtd_ids];

        // Remove all packages from the section stack
        while (neighbor->section.GetQtdCells() > 0)
        {
            package_id = neighbor->section.Pop();
            clock_time += package_removal_cost;
            printf("%07d pacote %03d removido de %03d na secao %03d\n", clock_time, 
                package_id, org_warehouse_id, dst_warehouse_id);

            temp.Push(package_id); // Store the removed package in the auxiliary stack
            packages_ids[i] = package_id; // Array to store the IDs of all packages removed from the section
            i++;
        }
        
        // Process packages considering transport and warehouse capacities
        while (temp.GetQtdCells() > 0)
        {           
            package_id = temp.Pop();
            p = &packages[package_id]; 

            if (qtd_removed_packages < t.capacity)
            {
                printf("%07d pacote %03d em transito de %03d para %03d\n", clock_time, 
                    package_id, org_warehouse_id, dst_warehouse_id);
                p->SetCurrentState(SECTION_REMOVED); // update package status
                qtd_removed_packages++;
            } else {
                neighbor->section.Push(package_id); // recolocar o pacote na pilha
                printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", clock_time, 
                    package_id, org_warehouse_id, dst_warehouse_id);
                p->SetCurrentState(SECTION_STORED);
            }     
        }

        // Update each package's latest processing time
        for (int i = 0; i < qtd_ids; i++)
            packages[packages_ids[i]].SetPTime(clock_time);

        return packages_ids;
    }

    return nullptr;
}