/*
---------------------------------------------------------------------------------------
  File        : main.cpp
  Description : Main execution file for the discrete event simulation system.
                It initializes the main data structures used in the simulation
                (Graph, Warehouse, Package and Scheduler).
---------------------------------------------------------------------------------------
*/

#include <iostream>
#include <string>

#include "../include/utils.h"
#include "../include/Graph.h"
#include "../include/Package.h"
#include "../include/Scheduler.h"

int main (int argc, char ** argv) 
{
  // Check if the required input file was provided as a command-line argument
  if (argc < 2) {
    std::cerr << "Erro: Nenhum arquivo foi fornecido como parâmetro." << std::endl;
    std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
    return 1;
  }

  std::string file = argv[1];
  Graph* g = new Graph();
  Package* packages;
  Scheduler* s;
  int qtd_packages;

  // Check if the input file exists and read its contents
  if (ExistFile(file))
    packages = ReadFile(file, g, &qtd_packages);
  else {
    std::cerr << "Erro: O arquivo '" << file << "' não existe." << std::endl;
    return 1;
  }  

  s = new Scheduler(qtd_packages);

  // Schedule the first package arrival events in the scheduler (at their origin warehouses)
  for (int i = 0; i < qtd_packages; i++)
    s->NewPackageEvent(&packages[i], WAREHOUSE_ARRIVAL);

  // Run the discrete event simulation
  s->RunSimulation(packages, g);

  delete g;
  delete[] packages;
  delete s;

  return 0;
}