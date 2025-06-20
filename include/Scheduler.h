#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>

#include "Package.h"
#include "Graph.h"
#include "utils.h"

class Scheduler
{
    private:
        int clock_time, heap_size, qtd_events;
        std::string* priority_queue; // min heap

        int GetLeftSucessor (int i);
        int GetRightSucessor (int i);
        int GetAncestral (int i);
        
        std::string GenerateKey(int event_time, int event_type, int package_id = -1, int origin = -1, int destination = -1);
        bool CompareKeys (std::string key1, std::string key2);
        void SwapKeys (std::string* key1, std::string* key2);
        void ResizePriorityQueue ();
        bool CheckKeyUniqueness (std::string key);

        void NewEvent (std::string key);
        void HeapifyUp (int new_key_index);
        void HeapifyDown (int root_index);

    public:
        Scheduler();
        Scheduler(int _heap_size);
        ~Scheduler();

        void NewPackageEvent (Package* p, States _new_state);
        std::string RemoveNextEvent ();
        void RunSimulation (Package*& packages, Graph*& system);
        void PrintEvent (std::string event_key, Package* p);
        void PrintPriorityQueue ();
};

#endif