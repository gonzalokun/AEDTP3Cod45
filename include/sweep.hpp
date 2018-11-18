#ifndef SWEEP_HPP
#define SWEEP_HPP

#include "basics3.hpp"

//Cargar los datos
void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn, float& capacidad);

//Con esto hacemos los clusters del sweep
vector<vector<Nodo>> generarClusters(ListaCordPol& lcp, const vector<Nodo>& vn, float capacidad);

//Resuelvo caminos con TSP
vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y);

vector<Nodo> tsp_con_grasp(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y);

void generarOutput(vector<vector<Nodo>>& clusters);

#endif // SWEEP_HPP
