#include <string>
#include <iostream>
#include <cstdlib> 

#include "../include/Scheduler.h"
#include "../include/utils.h"

Scheduler::Scheduler() { priority_queue = nullptr; }

Scheduler::Scheduler(int _qtd_packages) : clock_time(-1), heap_size(_qtd_packages), qtd_events(0), 
qtd_packages(_qtd_packages), next_transport(-1) { priority_queue = new std::string[_qtd_packages]; }

Scheduler::~Scheduler() { delete[] priority_queue; }

int Scheduler::GetLeftSucessor(int i) { return 2*i+1; }
int Scheduler::GetRightSucessor(int i) { return 2*i+2; }
int Scheduler::GetAncestral(int i) { 
    if (!i) return -1;
    return (i-1)/2; 
}

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
// Returns true if key1 is less than key 2 and false otherwise
{
    int time1 = stoi(key1.substr(0,6)), time2 = stoi(key2.substr(0,6));
    int event1_data, event2_data, event1_type, event2_type;

    if (time1 < time2)
        return true;
    else if (time1 > time2)
        return false;
    else {
        event1_data = stoi(key1.substr(6, 6));
        event2_data = stoi(key2.substr(6, 6));

        if (event1_data < event2_data)
            return true;
        else if (event1_data > event2_data)
            return false;
        else {
            event1_type = stoi(key1.substr(12, 1));
            event2_type = stoi(key1.substr(12, 1));

            if (event1_type < event2_type)
                return true;
            else if (event1_type > event2_type)
                return false;
            else {
                std::cerr << "Erro: Chaves idênticas!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }        
    }
}

void Scheduler::SwapKeys(std::string* key1, std::string* key2)
//
{
    std::string temp = *key1;
    *key1 = *key2;
    *key2 = temp;
}

void Scheduler::ResizePriorityQueue() 
// Duplicates the min heap (priority queue) size/capacity
{
    std::string* new_priority_queue = new std::string[2*heap_size];

    for (int i = 0; i < heap_size; i++)
        new_priority_queue[i] = priority_queue[i];
    
    heap_size *= 2;
    delete[] priority_queue;   
    priority_queue = new_priority_queue;
}

bool Scheduler::CheckKeyUniqueness(std::string key) 
//
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
// Cria um novo evento representado por uma chave específica e adiciona o evento criado na fila de prioridade (escalonador)
{
  std::string event_key;
  if (p)
  {
    event_key = GenerateKey(p->GetArrival(), 1, p->GetId());
    NewEvent(event_key); // add event to priority queue
  } else
      std::cerr << "Erro: Invalid package." << std::endl;
}

std::string Scheduler::RemoveNextEvent() 
//
{
    std::string next_event = priority_queue[0];
    priority_queue[0] = priority_queue[qtd_events-1];
    qtd_events--;

    HeapifyDown(0);

    return next_event;
}

void Scheduler::HeapifyUp(int new_key_index)
//
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
//
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
//
{
    std::string current_event_key, new_event_key;
    int id_arr_size, new_t_sec = 0;
    int* packages_ids;
    Package* p;
    char buffer[500];

    PrintPriorityQueue();

    while (qtd_events > 0)
    {
        current_event_key = RemoveNextEvent();
        
        // if (clock_time < stoi(current_event_key.substr(0,6)))
            clock_time = stoi(current_event_key.substr(0,6));

        if (stoi(current_event_key.substr(12,1)) == 1) 
        {
            p = SearchPackage(current_event_key, packages);

            if (p)
            {
                switch (p->GetCurrentState())
                {
                    case NOT_POSTED: // package has just arrived at the first warehouse
                        p->SetCurrentState(WAREHOUSE_ARRIVAL);

                        // remover primeiro armazém da rota
                        // se a rota após remoção ficar vazia, pacote chegou ao dst
                        // caso contrário, adicionar pacote na pilha da seção correspondente
                        if (!p->AdvanceInRoute())
                        {
                            // empilhar pacote na seção do próximo armazém
                            system->StorePackage(p, p->GetOriginId());
                            p->SetCurrentState(SECTION_STORED);
                        } else {
                            p->SetCurrentState(DELIVERED);
                            sprintf(buffer, "%07d pacote %03d entregue em %03d", clock_time, p->GetId(), 
                                p->GetDestinationId());
                            break;
                        }
                        
                        if (next_transport == -1) next_transport = clock_time + system->GetTransport().interval;

                        // Adding transport event in min heap
                        new_event_key = GenerateKey(next_transport, 2, -1, p->GetCurrentLocation(), p->GetNextStep());
                        NewEvent(new_event_key);

                        sprintf(buffer, "%07d pacote %03d armazenado em %03d na secao %03d", clock_time, p->GetId(), 
                            p->GetCurrentLocation(), p->GetNextStep());
                        
                        break;

                    case SECTION_REMOVED: // package was being transported and just arrived
                        p->SetCurrentState(WAREHOUSE_ARRIVAL);
                        new_t_sec = 0;

                        // remover primeiro armazém da rota
                        // se a rota após remoção ficar vazia, pacote chegou ao dst
                        // caso contrário, adicionar pacote na pilha da seção correspondente
                        if (!p->AdvanceInRoute())
                        {
                            // empilhar pacote na seção do próximo armazém
                            system->StorePackage(p, p->GetCurrentLocation());
                            p->SetCurrentState(SECTION_STORED);
                        } else {
                            p->SetCurrentState(DELIVERED);
                            sprintf(buffer, "%07d pacote %03d entregue em %03d", clock_time, p->GetId(), 
                                p->GetDestinationId());
                            break;
                        }

                        // next_transport = (p->GetPTime()-system->GetRemovalCost()) + system->GetTransport().interval;

                        // Adding transport event in min heap
                        new_event_key = GenerateKey(next_transport, 2, -1, p->GetCurrentLocation(), p->GetNextStep());
                        NewEvent(new_event_key);

                        //printf("%s\n", current_event_key.c_str());

                        sprintf(buffer, "%07d pacote %03d armazenado em %03d na secao %03d", clock_time, p->GetId(), 
                            p->GetCurrentLocation(), p->GetNextStep());
                        
                        break;

                    default:
                        break;
                }

                printf("%s\n", buffer);
            } else {
                std::cerr << "Erro: Pacote não encontrado!" << std::endl;
                exit(EXIT_FAILURE);
            }     
        }

        else 
        {
            packages_ids = system->RemovePackages(stoi(current_event_key.substr(6,3)), stoi(current_event_key.substr(9,3)), 
                clock_time, packages, qtd_packages, id_arr_size);

            for (int i = 0; i < id_arr_size; i++)
            {
                p = FindPackage(packages_ids[i], packages, qtd_packages);
                if (p)
                {
                    if (p->GetCurrentState() == SECTION_REMOVED)
                    {
                        if (!new_t_sec){
                            next_transport = (p->GetPTime()-system->GetRemovalCost()) + system->GetTransport().interval;
                            printf("rc = %d, time = %d\n", system->GetRemovalCost(), p->GetPTime());
                            new_t_sec = 1;
                        }
                        new_event_key = GenerateKey(p->GetPTime() + system->GetTransport().latency, 1, packages_ids[i]);
                        NewEvent(new_event_key);
                        //printf("%s\n", new_event_key.c_str());
                    } else if (p->GetCurrentState() == SECTION_STORED) // rearmazenado
                    {
                        new_event_key = GenerateKey(p->GetPTime() + system->GetTransport().interval, 2, -1, p->GetCurrentLocation(), p->GetNextStep());
                        NewEvent(new_event_key);
                    }
                } else
                    continue;
            }
        }
        //PrintPriorityQueue();
    }
}

void Scheduler::PrintEvent(std::string event_key, Package* p)
//
{
    std::cout << event_key << std::endl;
}

void Scheduler::PrintPriorityQueue()
//
{
    for (int i = 0; i < qtd_events; i++)
        std::cout << priority_queue[i] << std::endl; 
}