#include "../include/siman.hpp"
#include <time.h>
#include <math.h>

//Dependiendo el modo se devuelven distintos vecindarios
//Valores de vecindarios:
//  0 - Vecindario Swap
//  1 - Vecindario Interchange

//Hacemos el simulated annealing para cada camino
solucionProb simulatedAnnealingGeneral(solucionProb& solucionInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento){
    vector<vector<Nodo>> caminosSol;
    vector<vector<Nodo>> caminosIniciales = solucionInicial.getCaminos();

    //Encuentro el camino de costo mínimo para cada camino de la solucion
    for(int i=0; i < caminosIniciales.size(); i++){
        caminosIniciales[i] = simulatedAnnealingCamino(caminosIniciales[i], modo, temperaturaMax, temperaturaMin, coefEnfriamiento);
    }

    return solucionProb(caminosIniciales);
}

//Applica simulated annealing a un camino
vector<Nodo> simulatedAnnealingCamino(vector<Nodo>& caminoInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento){
    float tempActual = temperaturaMax;
    vector<Nodo> caminoSol = caminoInicial;
    vector<Nodo> caminoActual = caminoInicial;

    while(tempActual >= temperaturaMin){
        vector<vector<Nodo>> vecindario = generarVecindario(caminoActual, modo);

        //Elige al azar un indice
        std::random_device seed;
        //std::mt19937 engine(seed());
        std::default_random_engine generator;
        generator.seed(seed());
        std::uniform_int_distribution<int> choose(0, vecindario.size()-1);

        vector<Nodo> solTentativa = vecindario[choose(generator)];

        //Calculo la probabilidad de pasar a esta solucion
        srand (time(NULL));

        float r = (float) rand()/RAND_MAX;
        float prob = exp((-(calcularCostoCamino(solTentativa) - calcularCostoCamino(caminoActual)) / tempActual));

        if(prob >= r){
            caminoActual = solTentativa;
        }

        if(calcularCostoCamino(caminoActual) < calcularCostoCamino(caminoSol)){
            caminoSol = caminoActual;
        }

        tempActual -= coefEnfriamiento;
    }

    return caminoSol;
}

//Genera el vecindario de un camino
vector<vector<Nodo>> generarVecindario(vector<Nodo>& base, int modo){
    switch(modo){
        case VECINDARIO_SWAP:
            return generarVecindarioSWAP(base);
        case VECINDARIO_INTERCHANGE:
            return generarVecindarioINTERCHANGE(base);
    }
}

vector<vector<Nodo>> generarVecindarioSWAP(vector<Nodo>& base){
    //
}

vector<vector<Nodo>> generarVecindarioINTERCHANGE(vector<Nodo>& base){
    //
}
