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
    switch(modo){
        case VECINDARIO_SWAP:
        case VECINDARIO_INTERCHANGE:{
            vector<vector<Nodo>> caminosSol;
            vector<vector<Nodo>> caminosIniciales = solucionInicial.getCaminos();

            //Encuentro el camino de costo mínimo para cada camino de la solucion
            for(int i=0; i < caminosIniciales.size(); i++){
                caminosIniciales[i] = simulatedAnnealingCamino(caminosIniciales[i], modo, temperaturaMax, temperaturaMin, coefEnfriamiento);
            }

            return solucionProb(solucionInicial.getCapacidad(), caminosIniciales);
        }
        case VECINDARIO_EXCHANGE:{
            return simulatedAnnealingExchange(solucionInicial, temperaturaMax, temperaturaMin, coefEnfriamiento);
        }
    }
}

//Applica simulated annealing a un camino
vector<Nodo> simulatedAnnealingCamino(vector<Nodo>& caminoInicial, int modo, float temperaturaMax, float temperaturaMin, float coefEnfriamiento){
    float tempActual = temperaturaMax;
    vector<Nodo> caminoSol = caminoInicial;
    vector<Nodo> caminoActual = caminoInicial;

    //VARIABLES DEBUG
    int cantCiclos = 0;
    int cantSaltos = 0;

    std::random_device seed;
    std::mt19937 engine(seed());
    //std::default_random_engine generator;
    //generator.seed(seed());

    while(tempActual >= temperaturaMin){
        cantCiclos++;
        vector<vector<Nodo>> vecindario = generarVecindario(caminoActual, modo);

        //Elige al azar un indice
        std::uniform_int_distribution<int> choose(0, vecindario.size()-1);

        int index = choose(engine);

        //cout << "SE ELIGIO EL VECINO: " << index << "\n";

        vector<Nodo> solTentativa = vecindario[index];

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

    //cout << "EN " << cantCiclos << " CICLOS SE HICIERON " << cantSaltos << " SALTOS\n";

    return caminoSol;
}

solucionProb simulatedAnnealingExchange(solucionProb& solucionInicial, float temperaturaMax, float temperaturaMin, float coefEnfriamiento){
    float tempActual = temperaturaMax;
    solucionProb solFinal = solucionInicial;
    solucionProb solActual = solucionInicial;

    //VARIABLES DEBUG
    //int cantCiclos = 0;
    //int cantSaltos = 0;

    //Elige al azar un indice
    std::random_device seed;
    std::mt19937 engine(seed());
    //std::default_random_engine generator(seed());
    //generator.seed(seed());

    while(tempActual >= temperaturaMin){
        //cantCiclos++;

        //cout << "COSTO SOL ACTUAL: " << solActual.getCostoSol() << "\n";

        vector<solucionProb> vecindario = generarVecindarioEXCHANGE(solActual);

        //cout << "EL TAMANO DE VECINDARIO DEL CICLO " << cantCiclos << " ES: " << vecindario.size() << "\n";


        std::uniform_int_distribution<int> choose(0, vecindario.size()-1);

        int index = choose(engine);

        //cout << "SE ELIGIO EL VECINO: " << index << "\n";

        solucionProb solTentativa = vecindario[index];

        //Calculo la probabilidad de pasar a esta solucion
        float r = (float) rand()/RAND_MAX;

        //float prob = exp((-(calcularCostoCamino(solTentativa) - calcularCostoCamino(caminoActual)) / tempActual));
        float prob;

        float energiaEstadoActual = solActual.getCostoSol();
        float energiaNuevoEstado = solTentativa.getCostoSol();

        cout << "COSTO ESTADO ACTUAL: " << energiaEstadoActual << "\n";
        cout << "COSTO ESTADO NUEVO: " << energiaNuevoEstado << "\n";

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
            //cantSaltos++;
            solActual = solTentativa;
        }

        //Guardamos la mejor
        if(solActual.getCostoSol() < solFinal.getCostoSol()){
            float costoFinal = solFinal.getCostoSol();
            float costoNuevo = solActual.getCostoSol();

            solFinal = solActual;

            cout << "SE REDUJO EL COSTO DE LA SOLUCION DE " << costoFinal << " A " << costoNuevo <<"\n";
        }

        tempActual -= coefEnfriamiento;
    }

    //cout << "EN " << cantCiclos << " CICLOS SE HICIERON " << cantSaltos << " SALTOS\n";

    return solFinal;
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

//Este vecindario toma un nodo de un camino y lo cambia con un nodo de otro camino
//Se puede dar que un nodo pase a un camino y ningun nodo vaya al otro camino
vector<solucionProb> generarVecindarioEXCHANGE(solucionProb& base){
    vector<solucionProb> sol;

    vector<vector<Nodo>> caminosBase = base.getCaminos();

    for(int i=0; i < caminosBase.size() - 1; i++){
        for(int j=1; j < caminosBase[i].size() - 1; j++){
            //de 1 a n-2 porque 0 y n-1 son el mismo nodo porque es el principio y el fin
            for(int k = i+1; k < caminosBase.size(); k++){
                for(int l=1; l < caminosBase[k].size() - 1; l++){

                    //(0,1); acá los otros caminos le "dan" nodos al camino i esimo
                    vector<vector<Nodo>> caminosAux = caminosBase;

                    caminosAux[i].insert(caminosAux[i].begin() + j, caminosAux[k][l]);
                    caminosAux[k].erase(caminosAux[k].begin() + l);

                    if(capacidadAlcanza(base.getCapacidad(), caminosAux[k]) && capacidadAlcanza(base.getCapacidad(), caminosAux[i])){
                        //cout << "ALCANZO LA CAPACIDAD!\n";

                        //Si el k esimo camino tiene dos elem, es un camino que empieza y termina en el mismo nodo
                        if(caminosAux[k].size() == 2)
                            caminosAux.erase(caminosAux.begin() + k);

                        solucionProb solActual(base.getCapacidad(), caminosAux);
                        sol.push_back(solActual);
                    }

                    //(1,0); acá el i esimo le da nodos a los otros
                    caminosAux = caminosBase;

                    caminosAux[k].insert(caminosAux[k].begin() + l, caminosAux[i][j]);
                    caminosAux[i].erase(caminosAux[i].begin() + j);

                    if(capacidadAlcanza(base.getCapacidad(), caminosAux[k]) && capacidadAlcanza(base.getCapacidad(), caminosAux[i])){
                        //cout << "ALCANZO LA CAPACIDAD!\n";

                        //Si el k esimo camino tiene dos elem, es un camino que empieza y termina en el mismo nodo
                        if(caminosAux[i].size() == 2)
                            caminosAux.erase(caminosAux.begin() + i);

                        solucionProb solActual(base.getCapacidad(), caminosAux);
                        sol.push_back(solActual);
                    }

                    //(1, 1)
                    //Guardo el nodo a cambiar
                    Nodo buffer = caminosBase[k][l];

                    //Hago el cambio de nodos
                    caminosBase[k][l] = caminosBase[i][j];
                    caminosBase[i][j] = buffer;

                    //Si las capacidades de los dos caminos alcanzan entonces es un vecino
                    if(capacidadAlcanza(base.getCapacidad(), caminosBase[k]) && capacidadAlcanza(base.getCapacidad(), caminosBase[i])){
                        //cout << "ALCANZO LA CAPACIDAD!\n";
                        solucionProb solActual(base.getCapacidad(), caminosBase);
                        sol.push_back(solActual);
                    }

                    //Devuelvo caminosBase a su estado original
                    caminosBase[i][j] = caminosBase[k][l];
                    caminosBase[k][l] = buffer;
                }
            }
        }
    }

    return sol;
}

bool capacidadAlcanza(float capacidad, vector<Nodo> camino){
    float demandaTotal = 0;

    for(int i=0; i < camino.size(); i++){
        demandaTotal += camino[i].demanda;
    }

    return (capacidad >= demandaTotal);
}

solucionProb simulatedAnnealingOsman(solucionProb& solucionInicial, float temperaturaMax, float temperaturaMin, float coefEnfriamiento){
    //Se inicializan los parametros de temperatura

    vector<solucionProb> vecindario = generarVecindarioEXCHANGE(solucionInicial);

    //Se obtiene la max diferencia de costos
    float maxDeltaCost = vecindario[0].getCostoSol() - solucionInicial.getCostoSol();
    float minDeltaCost = vecindario[0].getCostoSol() - solucionInicial.getCostoSol();

    for(solucionProb sol : vecindario){
        if(maxDeltaCost < sol.getCostoSol() - solucionInicial.getCostoSol()){
            //
        }
    }

}
