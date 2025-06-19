#ifndef PACKAGE_H
#define PACKAGE_H

#include "LinkedList.h"

class Graph;

enum States
{
    NOT_POSTED,
    WAREHOUSE_ARRIVAL,
    SECTION_STORED,
    SECTION_REMOVED,
    DELIVERED
};

class Package
{
    private:
        int id, arrival_time, origin_id, destination_id, current_location_id, p_time;
        States current_state; // one of the enum (0-4)
        LinkedList* route;

    public:
        Package();
        Package(int _id, int arrival, int _origin_id, int _destination_id);
        Package(const Package& other);
        ~Package();

        Package& operator=(const Package& other);

        int GetId();
        int GetArrival();
        int GetOriginId();
        int GetDestinationId();
        States GetCurrentState();
        int GetCurrentLocation();
        int GetPTime();
        int GetNextStep();
        
        void SetCurrentState(States new_state);
        void SetCurrentLocation(int id);
        void SetPTime(int _p_time);

        void PrintRoute();
        void CalcRoute(Graph* g);
        bool AdvanceInRoute();
};

#include "Graph.h"

#endif