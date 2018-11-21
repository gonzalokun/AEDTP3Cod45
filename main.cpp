#include <iostream>
#include "include/sweep.hpp"
#include "include/siman.hpp"
#include <time.h>

using namespace std;

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;

    srand(time(NULL));

    cargarDatos(lcp, vn, capacidad);

    //system("pause");

    cout << "------------------------------" << endl;

    lcp.mostrarLista();

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    //system("pause");

    cout << "SE CARGARON TODOS LOS DATOS" << endl;

    clusters = generarClusters(lcp, vn, capacidad);

    //system("pause");

    cout << "------------------------------" << endl;

    cout << "SE GENERARON LOS CLUSTERS" << endl;

    cout << "------------------------------" << endl;

    cout << "MOSTRANDO INDICES DE CADA CLUSTER" << endl;

    for(int i = 0; i < clusters.size(); i++){
        cout << "CLUSTER " << i + 1 << ": [";
        for(int j = 0; j < clusters[i].size(); j++){
            cout << clusters[i][j].indice << ((j == clusters[i].size() - 1)? "" : ", ");
        }
        cout << "]" << endl;
    }

    //system("pause");

    cout << "------------------------------" << endl;

    cout << "GENERANDO OUTPUT PARA GRAFICAR" << endl;

    generarOutput(clusters);

    cout << "------------------------------" << endl;

    //system("pause");

    cout << "RESOLVIENDO CAMINOS POR TSP (Sin GRASP)" << endl;

    vector<vector<Nodo>> caminosSol;

    float max_x, max_y;
    max_x = vn[0].x;
    max_y = vn[0].y;

    for(int i = 1; i < vn.size(); i++){
        if(vn[i].x > max_x){
            max_x = vn[i].x;
        }
        if(vn[i].y > max_y){
            max_y = vn[i].y;
        }
    }

    //Voy solucionando todos los caminos
    for(int i = 0; i < clusters.size(); i++){
        float costoSol = 0;
        caminosSol.push_back(tsp2(clusters[i], 0, costoSol, 1, max_x, max_y));
    }

    cout << "------------------------------" << endl;

    //system("pause");

    cout << "MOSTRANDO CAMINOS" << endl;

    for(int i = 0; i < caminosSol.size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < caminosSol[i].size(); j++) {
            cout << caminosSol[i][j].indice << ((j == caminosSol[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;

    //system("pause");

    cout << "APLICANDO SIMAN A LA SOLUCION PARA VER SI MEJORA" << endl;

    solucionProb solActual(capacidad, caminosSol);

    cout << "EL COSTO DE LA SOL ANTES DE SIMAN: " << solActual.getCostoSol() << endl;

    system("pause");

    solucionProb solSWAP = simulatedAnnealingGeneral(solActual, VECINDARIO_SWAP, 10000, 1, 5);
    solucionProb solEXCHANGE = simulatedAnnealingGeneral(solActual, VECINDARIO_EXCHANGE, 10000, 1, 5);

    cout << "EL COSTO DE LA SOL DESPUES DE SIMAN (SWAP): " << solSWAP.getCostoSol() << "\n";
    cout << "EL COSTO DE LA SOL DESPUES DE SIMAN (EXCHANGE): " << solEXCHANGE.getCostoSol() << "\n";

    cout << "------------------------------" << endl;

    cout << "SOLUCION DEL SIMAN" << endl;

    for(int i = 0; i < solActual.getCaminos().size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < solActual.getCaminos()[i].size(); j++) {
            cout << solActual.getCaminos()[i][j].indice << ((j == solActual.getCaminos()[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;

    cout << "FIN DEL PROGRAMA" << endl;

    cout << "------------------------------" << endl;

    return 0;
}
