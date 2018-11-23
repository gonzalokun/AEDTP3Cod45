#ifndef SIMAN_H
#define SIMAN_H

#include "basics3.hpp"

#define VECINDARIO_SWAP 0
#define VECINDARIO_INTERCHANGE 1
#define VECINDARIO_EXCHANGE 2

float calcularCostoCamino(vector<Nodo>& camino);

//Con esto represento a las soluciones
class solucionProb{
private:
    float costoTotal;
    float capacidad;
    vector<vector<Nodo>> caminos;

    void calcularCosto(){
        float calculoTotal = 0;

        for(int i=0; i < caminos.size(); i++){
            calculoTotal += calcularCostoCamino(caminos[i]);
        }

        costoTotal = calculoTotal;
    }

public:

    solucionProb(float costoTotal, float capacidad, vector<vector<Nodo>> caminos){
        this->costoTotal = costoTotal;
        this->capacidad = capacidad;
        this->caminos = caminos;
    }

    solucionProb(float capacidad, vector<vector<Nodo>> caminos){
        this->capacidad = capacidad;
        this->caminos = caminos;
        calcularCosto();
    }

    solucionProb(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->capacidad = sp.getCapacidad();
        this->caminos = sp.getCaminos();
    }

    float getCostoSol() const{
        return costoTotal;
    }

    float getCapacidad() const{
        return capacidad;
    }

    vector<vector<Nodo>> getCaminos() const{
        return caminos;
    }

    solucionProb operator=(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->capacidad = sp.getCapacidad();
        this->caminos = sp.getCaminos();
        return *this;
    }
};

solucionProb simulatedAnnealingGeneral(solucionProb& solucionInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<Nodo> simulatedAnnealingCamino(vector<Nodo>& caminoInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

solucionProb simulatedAnnealingExchange(solucionProb& solucionInicial, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<vector<Nodo>> generarVecindario(vector<Nodo>& base, int modo);

vector<vector<Nodo>> generarVecindarioSWAP(vector<Nodo>& base);

vector<vector<Nodo>> generarVecindarioINTERCHANGE(vector<Nodo>& base);

vector<solucionProb> generarVecindarioEXCHANGE(solucionProb& base);

bool capacidadAlcanza(float capacidad, vector<Nodo> camino);

#endif // SIMAN_H
