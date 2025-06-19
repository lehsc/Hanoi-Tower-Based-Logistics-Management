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

int* Warehouse::PopPackages(int section, int _clock_time, int _removal_cost, 
        Package*& packages, int qtd_packages, int& qtd_ids, Transport* t)
// Removes packages from the given section, handling transport capacity and state updates
{
    Stack temp; // Auxiliary stack
    Neighbor* neighbor = FindNeighbor(section);
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
            _clock_time += _removal_cost;
            printf("%07d pacote %03d removido de %03d na secao %03d\n", _clock_time, package_id, id, section);

            temp.Push(package_id); // Store the removed package in the auxiliary stack
            packages_ids[i] = package_id; // Array to store the IDs of all packages removed from the section
            i++;
        }
        
        // Process packages considering transport capacity
        while (temp.GetQtdCells() > 0)
        {           
            package_id = temp.Pop(); 
            p = FindPackage(package_id, packages, qtd_packages);
            
            if (p)
            {
                if (qtd_removed_packages < t->capacity)
                {
                    printf("%07d pacote %03d em transito de %03d para %03d\n", _clock_time, package_id, id, section);
                    p->SetCurrentState(SECTION_REMOVED); // update package status
                    qtd_removed_packages++;
                } else {
                    neighbor->section.Push(package_id); // recolocar o pacote na pilha
                    printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", _clock_time, package_id, id, section);
                    p->SetCurrentState(SECTION_STORED);
                }

            } else {
                std::cerr << "Erro: Pacote não encontrado!" << std::endl;
                exit(EXIT_FAILURE);
            }      
        }

        // Update each package's latest processing time
        for (int i = 0; i < qtd_ids; i++)
            packages[packages_ids[i]].SetPTime(_clock_time);
        

        return packages_ids;
    }

    return nullptr;
}