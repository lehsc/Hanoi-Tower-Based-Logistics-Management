//---------------------------------------------------------------------
// Arquivo      : main.cpp
// Conteudo     : Arquivo principal ... 
//---------------------------------------------------------------------

#include <iostream>
#include <string>

#include "../include/utils.h"
#include "../include/Graph.h"
#include "../include/Package.h"
#include "../include/Scheduler.h"

int main (int argc, char ** argv) 
{
  // Verificar se o número de parâmetros passados é suficiente
  // if (argc < 2) {
  //   std::cerr << "Erro: Nenhum arquivo foi fornecido como parâmetro." << std::endl;
  //   std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
  //   return 1;
  // }

  // std::string file = argv[1];
  std::string file = "Entradas/ex2";
  Graph* g = new Graph();
  Package* packages;
  Scheduler* s;
  int qtd_packages;

  if (ExistFile(file)) // If the file exits, read it
    packages = ReadFile(file, g, &qtd_packages);
  else {
    std::cerr << "Erro: O arquivo '" << file << "' não existe." << std::endl;
    return 1;
  }  

  s = new Scheduler(qtd_packages);

  // Add the first events in the scheduler
  for (int i = 0; i < qtd_packages; i++)
    s->NewPackageEvent(&packages[i], WAREHOUSE_ARRIVAL);

  s->RunSimulation(packages, g);

  delete g;
  delete[] packages;

  return 0;
}