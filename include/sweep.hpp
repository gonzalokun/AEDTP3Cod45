#ifndef SWEEP_HPP
#define SWEEP_HPP

#include "basics3.h"

//Cargar los datos
void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn, float& capacidad);

//Con esto hacemos los clusters del sweep
vector<vector<Nodo>> generarClusters(ListaCordPol& lcp, const vector<Nodo>& vn, float capacidad);

//Resuelvo los clusters
//vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p);

void generarOutput(vector<vector<Nodo>>& clusters);

#endif // SWEEP_HPP
