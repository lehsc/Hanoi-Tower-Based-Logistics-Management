#ifndef GRAPH_H
#define GRAPH_H

class Warehouse;
class Package;

typedef struct transport_t
{
    int capacity;  // Maximum number of packages the transport can carry at once
    int latency;   // Time required for the transport to travel between two warehouses
    int interval;  // Time interval between consecutive transport departures
} Transport;

class Graph
{
    private:
        int qtd_warehouses, max_warehouses, package_removal_cost;
        Warehouse** warehouses; 
        Transport t;

    public:
        Graph ();
        Graph (int _max_warehouses, int removal_cost, Transport _t);
        ~Graph ();

        int GetMaxWarehouses ();
        int GetRemovalCost ();
        Transport GetTransport();
        Warehouse* GetWarehouse (int warehouse_id);

        void AddWarehouse (int _id, int _qtd_neighbors, int* _neighbors);
        void PrintWarehouses ();
        void StorePackage(Package* p, int warehouse_id);
        int* RemovePackages(int org_warehouse_id, int dst_warehouse_id, int& clock_time, 
            Package*& packages, int qtd_packages, int& qtd_ids);

    friend class Package;
};

#include "Warehouse.h"
#include "Package.h"

#endif