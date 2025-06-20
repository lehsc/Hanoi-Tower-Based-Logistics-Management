/*
---------------------------------------------------------------------------------------
  File        : Scheduler.cpp
  Description : Implementation of the Scheduler class, responsible for managing
                and executing events using a min-heap priority queue.
---------------------------------------------------------------------------------------
*/

#include <string>
#include <iostream>
#include <cstdlib> 

#include "../include/Scheduler.h"
#include "../include/utils.h"

// Default constructor
Scheduler::Scheduler() { priority_queue = nullptr; }

// Parameterized constructor
Scheduler::Scheduler(int _qtd_packages) 
: clock_time(-1), heap_size(_qtd_packages), qtd_events(0), qtd_packages(_qtd_packages) 
{
    priority_queue = new std::string[_qtd_packages];
}

// Destructor
Scheduler::~Scheduler() { delete[] priority_queue; }

// Heap helper functions
int Scheduler::GetLeftSucessor(int i) { return 2*i+1; }
int Scheduler::GetRightSucessor(int i) { return 2*i+2; }
int Scheduler::GetAncestral(int i) { return (i == 0) ? -1 : (i-1)/2; }

std::string Scheduler::GenerateKey(int event_time, int event_type, int package_id, int origin, int destination)
/* Generates a 13-character priority key based on event type:
    - Package event (type 1): [6-digit time][6-digit package_id][1]
    - Transport event (type 2): [6-digit time][3-digit origin][3-digit destination][2]
*/
{
  char buffer[14];

  if (event_type == 1) // Package event
    sprintf(buffer, "%06d%06d%d", event_time, package_id, event_type);
  else if (event_type == 2) // Transport event
    sprintf(buffer, "%06d%03d%03d%d", event_time, origin, destination, event_type);
  else
    std::cerr << "Erro: Invalid event type." << std::endl;
  
  return std::string(buffer); 
}

bool Scheduler::CompareKeys(std::string key1, std::string key2) 
//  Returns true if key1 has higher priority (i.e., is smaller) than key2
{
    int time1 = stoi(key1.substr(0,6)), time2 = stoi(key2.substr(0,6));
    int event1_data, event2_data, event1_type, event2_type;

    if (time1 < time2) return true;
    if (time1 > time2) return false;

    event1_data = stoi(key1.substr(6, 6));
    event2_data = stoi(key2.substr(6, 6));

    if (event1_data < event2_data) return true;
    if (event1_data > event2_data) return false;

    event1_type = stoi(key1.substr(12, 1));
    event2_type = stoi(key1.substr(12, 1));

    if (event1_type < event2_type) return true;
    if (event1_type > event2_type) return false;

    std::cerr << "Error: Identical keys detected!" << std::endl;
    exit(EXIT_FAILURE);
}

void Scheduler::SwapKeys(std::string* key1, std::string* key2)
// Swaps two keys in the priority queue
{
    std::string temp = *key1;
    *key1 = *key2;
    *key2 = temp;
}

void Scheduler::ResizePriorityQueue() 
// Doubles the size of the priority queue when needed
{
    std::string* new_priority_queue = new std::string[2*heap_size];

    for (int i = 0; i < heap_size; i++)
        new_priority_queue[i] = priority_queue[i];
    
    heap_size *= 2;
    delete[] priority_queue;   
    priority_queue = new_priority_queue;
}

bool Scheduler::CheckKeyUniqueness(std::string key) 
// Checks if the given key already exists in the queue
{
    for (int i = 0; i < qtd_events; i++)
        if (key == priority_queue[i]) return false;   

    return true; 
}

void Scheduler::NewEvent(std::string key) 
// insert a new event in the priority queue (min-heap)
{
    if (CheckKeyUniqueness(key))
    {
        priority_queue[qtd_events] = key;      

        if (qtd_events) HeapifyUp(qtd_events);
        qtd_events++;

        if (qtd_events >= heap_size) ResizePriorityQueue();
    }
}

void Scheduler::NewPackageEvent(Package* p, States _new_state)
// Creates a new package event and adds it to the priority queue
{
  std::string event_key;
  if (p)
  {
    event_key = GenerateKey(p->GetArrival(), 1, p->GetId());
    NewEvent(event_key);
  } else
      std::cerr << "Erro: Invalid package." << std::endl;
}

std::string Scheduler::RemoveNextEvent() 
// Removes and returns the highest priority event (root of the heap)
{
    std::string next_event = priority_queue[0];
    priority_queue[0] = priority_queue[qtd_events-1];
    qtd_events--;

    HeapifyDown(0);

    return next_event;
}

void Scheduler::HeapifyUp(int new_key_index)
// Restores heap property after insertion
{
    int previous_event_index = GetAncestral(new_key_index);
    std::string new_event = priority_queue[new_key_index];

    if (previous_event_index != -1 && CompareKeys(new_event, priority_queue[previous_event_index]))
    {
        SwapKeys(&priority_queue[new_key_index], &priority_queue[previous_event_index]);
        HeapifyUp(previous_event_index);
    }
}

void Scheduler::HeapifyDown(int root_index)
// Restores heap property after removal
{
    int left = GetLeftSucessor(root_index), right = GetRightSucessor(root_index), smallest = root_index;

    if (left < qtd_events && CompareKeys(priority_queue[left], priority_queue[smallest])) smallest = left;
    if (right < qtd_events && CompareKeys(priority_queue[right], priority_queue[smallest])) smallest = right;

    if (smallest != root_index)
    {
        SwapKeys(&priority_queue[root_index], &priority_queue[smallest]);
        HeapifyDown(smallest);
    }
}

Package* Scheduler::SearchPackage(std::string event_key, Package*& packages) 
//
{
    int package_id = stoi(event_key.substr(6,6));
    for (int i = 0; i < qtd_packages; i++) //
    {
        if (package_id == packages[i].GetId())
            return &packages[i];
    }

    return nullptr;   
}

void Scheduler::RunSimulation(Package*& packages, Graph*& system)
/* Executes the event simulation loop until the queue becomes empty.
   Processes both package and transport events.
*/
{
    std::string current_event_key, new_event_key;
    Package* p;
    int prev_transport = -1; // time of the last scheduled transport
    int next_transport = -1; // time for the next scheduled transport
    
    int* packages_ids;  // array of package IDs removed from a section
    int id_arr_size;    // number of IDs in packages_ids

    char buffer[500];

    while (qtd_events > 0)
    {
        current_event_key = RemoveNextEvent();
        clock_time = stoi(current_event_key.substr(0,6));

        if (stoi(current_event_key.substr(12,1)) == 1) // Package event: package arrival
        {
            p = &packages[stoi(current_event_key.substr(6,6))];
            p->SetCurrentState(WAREHOUSE_ARRIVAL);

            // Remove the first warehouse from the route
            // If the route becomes empty after removal, the package has arrived at the destination
            // Otherwise, push the package onto the stack of the corresponding section
            if (!p->AdvanceInRoute()) 
            {
                // Push the package onto the section stack of the next warehouse
                system->StorePackage(p, p->GetCurrentLocation());
                p->SetCurrentState(SECTION_STORED);
                if (next_transport == -1) next_transport = clock_time + system->GetTransport().interval;

                // Adding transport event in min heap
                new_event_key = GenerateKey(next_transport, 2, -1, p->GetCurrentLocation(), p->GetNextStep());
                NewEvent(new_event_key);

                sprintf(buffer, "%07d pacote %03d armazenado em %03d na secao %03d", clock_time, p->GetId(), 
                    p->GetCurrentLocation(), p->GetNextStep());
            } else {
                p->SetCurrentState(DELIVERED);
                sprintf(buffer, "%07d pacote %03d entregue em %03d", clock_time, p->GetId(), 
                    p->GetDestinationId());
            }
            
            // Ensure that "prev_transport" is updated only when "next_transport" advances
            if (prev_transport != next_transport) prev_transport = next_transport;

            printf("%s\n", buffer);    
        }

        else // Transport event
        {
            // Remove up to 'c' packages from the section stack at the source warehouse (origin → destination)
            // The function returns an array with the IDs of the removed packages and updates id_arr_size
            packages_ids = system->RemovePackages(
                stoi(current_event_key.substr(6, 3)),  // Origin warehouse ID
                stoi(current_event_key.substr(9, 3)),  // Destination warehouse ID
                clock_time, 
                packages, 
                qtd_packages, 
                id_arr_size
            );
            
            if (next_transport <= prev_transport) // Schedule the next transport event if needed
                next_transport += system->GetTransport().interval;

            for (int i = 0; i < id_arr_size; i++) // For each removed package, decide what event to schedule next
            {
                p = &packages[packages_ids[i]];
                if (p->GetCurrentState() == SECTION_REMOVED) // package is in transit
                {
                    // Schedule its arrival at the next warehouse
                    new_event_key = GenerateKey(p->GetPTime() + system->GetTransport().latency, 1, packages_ids[i]);
                    NewEvent(new_event_key);
                } else if (p->GetCurrentState() == SECTION_STORED) // package was re-stored
                {
                    // Schedule a new transport event (same origin and destination)
                    new_event_key = GenerateKey(next_transport, 2, -1, p->GetCurrentLocation(), p->GetNextStep());
                    NewEvent(new_event_key);
                }
            }
        }
    }
}

void Scheduler::PrintPriorityQueue()
// Prints the current events in the priority queue 
{
    for (int i = 0; i < qtd_events; i++)
        std::cout << priority_queue[i] << std::endl; 
}