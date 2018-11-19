#ifndef SIMAN_H
#define SIMAN_H

#include "basics3.hpp"

#define VECINDARIO_SWAP 0
#define VECINDARIO_INTERCHANGE 1

float calcularCostoCamino(vector<Nodo> camino){
    float calculoCamino = 0;

    //calcula el costo del camino entre nodos
    for(int j=0; j < camino.size() - 1;j++){
        calculoCamino += distancia_euclidea(camino[j].x, camino[j].y, camino[j+1].x, camino[j+1].y);
    }

    //Agrego lo que cuesta llegar desde el final al principio
    calculoCamino = distancia_euclidea(camino[camino.size() - 1].x, camino[camino.size() - 1].y, camino[0].x, camino[0].y);

    return calculoCamino;
}

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
        this->caminos;
        calcularCosto();
    }

    float getCostoSol() const{
        return costoTotal;
    }

    vector<vector<Nodo>> getCaminos() const{
        return caminos;
    }

    solucionProb& operator=(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->caminos = sp.getCaminos();
        return *this;
    }
};

solucionProb simulatedAnnealingGeneral(solucionProb& solucionInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<Nodo> simulatedAnnealingCamino(vector<Nodo>& caminoInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento);

vector<vector<Nodo>> generarVecindario(vector<Nodo>& base, int modo);

vector<vector<Nodo>> generarVecindarioSWAP(vector<Nodo>& base);

vector<vector<Nodo>> generarVecindarioINTERCHANGE(vector<Nodo>& base);

#endif // SIMAN_H
