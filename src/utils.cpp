/*
---------------------------------------------------------------------------------------
  File        : utils.cpp
  Description : Implementation of auxiliary functions used by the main program 
                for reading input files, processing warehouses and packages, 
                and utility tasks.
---------------------------------------------------------------------------------------
*/

#include <iostream>
#include <string>
#include <fstream>

#include "../include/utils.h"
#include "../include/Graph.h"
#include "../include/Package.h"
#include "../include/Scheduler.h"

void PrintPackages(Package* packages, int qtd_packages)
// Prints all packages' information (id, arrival time, origin, destination)
{
  for (int i = 0; i < qtd_packages; i++)
    printf("%d %d %d %d\n", packages[i].GetId(), packages[i].GetArrival(), packages[i].GetOriginId(), packages[i].GetDestinationId());
}

bool ExistFile(const std::string f)
// Verifies if the input file exists and can be opened
{
  std::ifstream file(f);
  return file.is_open();
}

void ProcessWarehouse(std::string file_line, Graph* g, int index)
// Processes a warehouse line from the input file and adds the warehouse to the graph 'g'
{
  int k = 0, j = 0; // k - number of neighbors, j - warehouse indices
  int* neighbors_indices = new int[g->GetMaxWarehouses() - 1];

  for (int i = 0; (unsigned)i <= file_line.size(); i++)
  {
    if (file_line[i] != ' ')
    {
      if (file_line[i] == '1') neighbors_indices[k++] = j;
      j++;
    }
  }

  g->AddWarehouse(index, k, neighbors_indices);
  delete[] neighbors_indices;
}

void ProcessPackage(std::string file_line, Package* packages, int i, Graph* g) 
// Processes a package line from the input file and creates the corresponding Package object
{
  int k = 0;
  int* data = new int[4];
  std::string entry = "";

  for (int j = 0; (unsigned)j <= file_line.size(); j++){
    if (file_line[j] != ' ')
      entry += file_line[j]; // Concatenate characters until a space is found
    else {
      if (entry != "pac" && entry != "org" && entry != "dst") data[k++] = stoi(entry);
      entry = "";
    }
  }

  data[k] = stoi(entry);

  packages[i] = Package(i, data[0], data[2], data[3]);
  packages[i].CalcRoute(g);

  delete[] data;
}

Package* ReadFile(const std::string f, Graph*& g, int* qtd_packages)
// Reads the input file and initializes the graph and packages
{
  std::ifstream file(f);
  Package* packages = nullptr;

  if (!file.is_open()) {
    std::cerr << "Ocorreu um erro ao tentar abrir o arquivo '" + f + "'." << std::endl;
    return packages;
  }

  std::string line{};
  int i = 1, max_wh, removal_cost;
  Transport t;

  while(getline(file, line))
  // Read each line from the input file
  {
    if (i > 4)
    {
      if (i == 5) {
        g = new Graph(stoi(line), removal_cost, t);
        max_wh = g->GetMaxWarehouses();
      } else {
        if (i-6 < max_wh)
          ProcessWarehouse(line, g, i-6);
        else if (i == max_wh + 6) {
          *qtd_packages = stoi(line);
          packages = new Package[*qtd_packages];
        }
        else ProcessPackage(line, packages, i-(7+max_wh), g);
      } 
    }
    
    else 
    // First four lines contain transport info and removal cost
    {
      switch (i)
      { 
        case 1:
          t.capacity = stoi(line);
          break;
        case 2:
          t.latency = stoi(line);
          break;
        case 3:
          t.interval = stoi(line);
          break;
        case 4:
          removal_cost = stoi(line);
          break;
        default:
          break;
      }
    }

    i++;
  }

  return packages;  
}