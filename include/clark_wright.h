#ifndef CLARK_WRIGHT_CLARK_WRIGHT_H
#define CLARK_WRIGHT_CLARK_WRIGHT_H
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include "System.h"
#include "Respuesta.h"
#include <stdlib.h>
using namespace std;



Respuesta pesoCaminoTotalCW2(System& s);
void mergeRoute(Route& a, Route& b);
Respuesta heuristicaGolosa(System& s);
Respuesta heuristicaGolosa2(System& s);
Respuesta heuristicaGolosa3(System& s);
static Customer customerToCompare = Customer(0);

struct comparator {
    bool operator()(Customer* i, Customer* j) {
        return EuclideanDist(customerToCompare,*i) > EuclideanDist(customerToCompare,*j);
    }
};
#endif //CLARK_WRIGHT_CLARK_WRIGHT_H
