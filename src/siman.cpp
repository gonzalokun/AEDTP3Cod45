#include "../include/siman.hpp"
#include <time.h>
#include <math.h>

//Dependiendo el modo se devuelven distintos vecindarios
//Valores de vecindarios:
//  0 - Vecindario Swap
//  1 - Vecindario Interchange

float calcularCostoCamino(vector<Nodo>& camino){
    float calculoCamino = 0;

    //calcula el costo del camino entre nodos
    for(int j=0; j < camino.size() - 1;j++){
        calculoCamino += distancia_euclidea(camino[j].x, camino[j].y, camino[j+1].x, camino[j+1].y);
    }

    //Agrego lo que cuesta llegar desde el final al principio
    //calculoCamino += distancia_euclidea(camino[camino.size() - 1].x, camino[camino.size() - 1].y, camino[0].x, camino[0].y);

    return calculoCamino;
}

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

    //VARIABLES DEBUG
    int cantCiclos = 0;
    int cantSaltos = 0;

    while(tempActual >= temperaturaMin){
        cantCiclos++;
        vector<vector<Nodo>> vecindario = generarVecindario(caminoActual, modo);

        //Elige al azar un indice
        std::random_device seed;
        //std::mt19937 engine(seed());
        std::default_random_engine generator;
        generator.seed(seed());
        std::uniform_int_distribution<int> choose(0, vecindario.size()-1);

        vector<Nodo> solTentativa = vecindario[choose(generator)];

        //Calculo la probabilidad de pasar a esta solucion
        float r = (float) rand()/RAND_MAX;

        //float prob = exp((-(calcularCostoCamino(solTentativa) - calcularCostoCamino(caminoActual)) / tempActual));
        float prob;

        float energiaEstadoActual = calcularCostoCamino(caminoActual);
        float energiaNuevoEstado = calcularCostoCamino(solTentativa);

        if(energiaNuevoEstado < energiaEstadoActual){
            prob = 1.0;
        }
        else{
            //prob = exp(((energiaEstadoActual - energiaNuevoEstado) / tempActual));
            prob = exp(-((energiaNuevoEstado - energiaEstadoActual) / tempActual));
        }

        if(prob >= r){
            //cout << "VALOR r: " << r << "\n";
            //cout << "VALOR prob: " << prob << "\n";
            //cout << "energiaEstadoActual: " << energiaEstadoActual << "\n";
            //cout << "energiaNuevoEstado: " << energiaNuevoEstado << "\n";
            //cout << "----------------------------------------" << "\n";
            cantSaltos++;
            caminoActual = solTentativa;
        }

        //Guardamos la mejor
        if(calcularCostoCamino(caminoActual) < calcularCostoCamino(caminoSol)){
            caminoSol = caminoActual;
        }

        tempActual -= coefEnfriamiento;
    }

    cout << "EN " << cantCiclos << " CICLOS SE HICIERON " << cantSaltos << " SALTOS\n";

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
    vector<vector<Nodo>> sol;
    for(int i=1; i < base.size() - 2; i++){
        vector<Nodo> nuevo = base;
        nuevo[i] = base[i+1];
        nuevo[i+1] = base[i];
        sol.push_back(nuevo);
    }
    return sol;
}

vector<vector<Nodo>> generarVecindarioINTERCHANGE(vector<Nodo>& base){
    vector<vector<Nodo>> sol;
    for(int i=1; i < base.size() - 1; i++){
        for(int j=i+1; j < base.size() - 1; j++){
            vector<Nodo> nuevo = base;
            nuevo[i] = base[j];
            nuevo[j] = base[i];
            sol.push_back(nuevo);
        }
    }
    return sol;
}
