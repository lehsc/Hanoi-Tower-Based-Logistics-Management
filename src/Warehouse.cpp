/*
---------------------------------------------------------------------------------------
  File        : Warehouse.cpp
  Description : Implementation of the Warehouse class, which manages package storage, 
                neighbor sections, and package removal from sections.
---------------------------------------------------------------------------------------
*/

#include <iostream>

#include "../include/Warehouse.h"
#include "../include/Package.h"
#include "../include/Scheduler.h"
#include "../include/utils.h"

// Default constructor
Warehouse::Warehouse() : id(-1), qtd_neighbors(0) { neighbors = nullptr; }

// Parameterized constructor
Warehouse::Warehouse(int _id, int _qtd_neighbors) : id(_id), qtd_neighbors(_qtd_neighbors) 
{ 
    neighbors = new Neighbor[qtd_neighbors]; 
}

// Destructor
Warehouse::~Warehouse() { delete[] neighbors; }

// Get methods
int Warehouse::GetId() { return id; }
int Warehouse::GetQtdNeighbors() { return qtd_neighbors; }

Neighbor* Warehouse::FindNeighbor(int neighbor_id) 
// Returns a pointer to the neighbor with the given ID
{
    for (int i = 0; i < qtd_neighbors; i++)
    {
        if (neighbor_id == neighbors[i].id)
            return &neighbors[i];   
    }

    return nullptr;
}

void Warehouse::AddNeighbors(int* _neighbors_ids)
// Sets the neighbors IDs
{
    for (int i = 0; i < qtd_neighbors; i++) 
        neighbors[i].id = _neighbors_ids[i];
}

void Warehouse::PrintNeighbors()
// Prints the neighbor IDs
{
    for (int i = 0; i < qtd_neighbors; i++) std::cout << neighbors[i].id << " ";
    printf("\n");
}

void Warehouse::PushPackage(Package* p) 
// Stores a package in the section corresponding to its next step in route
{
    int section = p->GetNextStep();
    for (int i = 0; i < qtd_neighbors; i++)
    {
        if (section == neighbors[i].id) {
            neighbors[i].section.Push(p->GetId());
            break;
        }
    }   
}