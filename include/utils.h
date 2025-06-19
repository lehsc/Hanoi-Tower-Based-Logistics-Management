#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "../include/Graph.h"
#include "../include/Package.h"

Package* FindPackage(int id, Package* packages, int qtd_packages);
void PrintPackages(Package* packages, int qtd_packages);
Package* ReadFile (const std::string f, Graph*& g, int* qtd_packages);
bool ExistFile(const std::string f);

#endif