#ifndef SIMAN_H
#define SIMAN_H

#include "basics3.hpp"

#define VECINDARIO_SWAP 0
#define VECINDARIO_INTERCHANGE 1

float calcularCostoCamino(vector<Nodo>& camino);

//Con esto represento a las soluciones
class solucionProb{
private:
    float costoTotal;
    vector<vector<Nodo>> caminos;

    void calcularCosto(){
        float calculoTotal = 0;

        for(int i=0; i < caminos.size(); i++){
            calculoTotal += calcularCostoCamino(caminos[i]);
        }

        costoTotal = calculoTotal;
    }

public:

    solucionProb(float costoTotal, vector<vector<Nodo>> caminos){
        this->costoTotal = costoTotal;
        this->caminos = caminos;
    }

    solucionProb(vector<vector<Nodo>> caminos){
        this->caminos = caminos;
        calcularCosto();
    }

    solucionProb(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->caminos = sp.getCaminos();
    }

    float getCostoSol() const{
        return costoTotal;
    }

    vector<vector<Nodo>> getCaminos() const{
        return caminos;
    }
};

solucionProb simulatedAnnealingGeneral(solucionProb& solucionInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<Nodo> simulatedAnnealingCamino(vector<Nodo>& caminoInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<vector<Nodo>> generarVecindario(vector<Nodo>& base, int modo);

vector<vector<Nodo>> generarVecindarioSWAP(vector<Nodo>& base);

vector<vector<Nodo>> generarVecindarioINTERCHANGE(vector<Nodo>& base);

#endif // SIMAN_H
