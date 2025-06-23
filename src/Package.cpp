/*
---------------------------------------------------------------------------------------
  File        : Package.cpp
  Description : Implementation of the Package class, which models each package in 
                the simulation, including its state, route, and status transitions.
---------------------------------------------------------------------------------------
*/

#include <iostream>

#include "../include/Package.h"
#include "../include/Queue.h"

// Default constructor
Package::Package() { route = nullptr; }

// Parameterized constructor
Package::Package(int _id, int _arrival, int _origin_id, int _destination_id)
: id(_id), arrival_time(_arrival), origin_id(_origin_id), destination_id(_destination_id), current_state(NOT_POSTED) 
{ 
    route = new LinkedList(); 
}

Package::Package(const Package& other)
: id(other.id), arrival_time(other.arrival_time), origin_id(other.origin_id), destination_id(other.destination_id)
// Copy constructor
{
    route = new LinkedList(*other.route); // Deep copy of the route
}

// Destructor
Package::~Package() { delete route; }

Package& Package::operator=(const Package& other)
// Perform a deep copy of the object
{
    if (this == &other) return *this; // Prevents self-assignment

    // Copy primitive data members
    id = other.id;
    arrival_time = other.arrival_time;
    origin_id = other.origin_id;
    destination_id = other.destination_id;
    current_state = other.current_state;

    // Delete old route and deep copy the new one
    if (route != nullptr)
        delete route;

    route = new LinkedList(*other.route);

    return *this;
}

// Get methods
int Package::GetId() { return id; }
int Package::GetArrival() { return arrival_time; }
int Package::GetOriginId() { return origin_id; } 
int Package::GetDestinationId() { return destination_id; }
States Package::GetCurrentState() { return current_state; }
int Package::GetCurrentLocation() { return current_location_id; }
int Package::GetPTime() { return p_time; }

int Package::GetNextStep() 
// returns the next step (warehouse id) in route
{ 
    return route->Search(0); 
}

// Set methods
void Package::SetCurrentState(States new_state) { current_state = new_state; }
void Package::SetCurrentLocation(int id) { current_location_id = id; }
void Package::SetPTime(int _p_time) { p_time = _p_time; }


void Package::PrintRoute() 
// Prints the package route (warehouse sequence)
{
    printf("Package Id: %d, route: ", id); 
    route->Print(); 
}

void Package::CalcRoute(Graph* g)
/* Calculates the shortest path from origin to destination using BFS.
   The result is stored in the package route (LinkedList).
*/
{
    Queue q; // keeps the warehouses that must be visited
    int warehouse_id, neighbor_id, qtd_neighbors, current = destination_id;
    int* antecedent = new int[g->GetMaxWarehouses()]; // identifies the antecedent node of each warehouse node
    int* visited_nodes = new int[g->GetMaxWarehouses()]; // identifies if each warehouse node was (1) or not (0) visited 
    bool found = false;
    
    for (int i = 0; i < g->GetMaxWarehouses(); i++) {
        visited_nodes[i] = 0; // marks all warehouses nodes as unvisited
        antecedent[i] = -1;
    }

    q.Enqueue(origin_id);
    visited_nodes[origin_id] = 1; // marks the origin as visited 
    antecedent[origin_id] = -1; // marks the origin warehouse node antecedent as -1 to indicate it is the origin

    while (!q.IsEmpty())
    {
        warehouse_id = q.Dequeue();
        if (warehouse_id == destination_id) {
            found = true;
            break;
        }

        // Enqueue the neighbors
        qtd_neighbors = g->warehouses[warehouse_id]->GetQtdNeighbors();
        for (int i = 0; i < qtd_neighbors; i++)
        {
            neighbor_id = g->warehouses[warehouse_id]->neighbors[i].id;
            if (!visited_nodes[neighbor_id])
            {
                visited_nodes[neighbor_id] = 1; // marks the neighbor node as visited   
                antecedent[neighbor_id] = warehouse_id;  // save the neighbor's node antecedent
                
                q.Enqueue(neighbor_id);     
            }
        }        
    }
    
    if (found)
    {
        while (current != -1)
        {
            route->Insert(current, 0);
            current = antecedent[current];
        }   
    } else {
        std::cerr << "Erro: There is no route between the warehouses " 
                  << origin_id << " and " << destination_id << "." << std::endl;
        exit(EXIT_FAILURE);
    }

    delete[] antecedent;
    delete[] visited_nodes;
}

bool Package::AdvanceInRoute()
/* Removes the first node of the 'route' list, indicating that
   the package has arrived at the warehouse whose id matches the node's key value
   and is ready to proceed to the next warehouse.
   If the package has arrived at its final destination (empty route), returns true
*/
{ 
    current_location_id = route->Remove(0);
    if (route->GetQtdNodes() > 0) return false;
    return true;
}