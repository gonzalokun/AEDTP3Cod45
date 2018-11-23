#include <iostream>
#include "include/sweep.hpp"
#include "include/siman.hpp"
#include <time.h>
#include <fstream>
#include <chrono>

using namespace std;

void exportar_grafo(vector<Nodo>& camino, float costo,int ruteo, string in);
float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y);

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;

    srand(time(NULL));

    cargarDatos(lcp, vn, capacidad);

    cout << "------------------------------" << endl;

    lcp.mostrarLista();

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    cout << "SE CARGARON TODOS LOS DATOS" << "\n";

    cout << "------------------------------" << "\n";

    cout << "GENERANDO CLUSTERS" << "\n";

    auto timeStartSweep = chrono::steady_clock::now();

    clusters = generarClusters(lcp, vn, capacidad);

    cout << "------------------------------" << "\n";

    cout << "RESOLVIENDO CAMINOS POR TSP" << "\n";

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

    auto timeEndSweep = chrono::steady_clock::now();

    //Acá calculo lo que tardó en resolverlo el sweep
    double timeSweep = chrono::duration<double, milli>(timeEndSweep - timeStartSweep).count();

    cout << "------------------------------" << "\n";

    cout << "MOSTRANDO INDICES DE CADA CLUSTER" << "\n";

    for(int i = 0; i < clusters.size(); i++){
        cout << "CLUSTER " << i + 1 << ": [";
        for(int j = 0; j < clusters[i].size(); j++){
            cout << clusters[i][j].indice << ((j == clusters[i].size() - 1)? "" : ", ");
        }
        cout << "]" << endl;
    }

    cout << "------------------------------" << endl;

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

    //system("pause");

    auto timeStartSwap = chrono::steady_clock::now();
    solucionProb solSWAP = simulatedAnnealingGeneral(solActual, VECINDARIO_SWAP, 10000, 1, 2);
    auto timeEndSwap = chrono::steady_clock::now();

    double timeSwap = chrono::duration<double, milli>(timeEndSwap - timeStartSwap).count();

    auto timeStartExchange = chrono::steady_clock::now();
    solucionProb solEXCHANGE = simulatedAnnealingGeneral(solActual, VECINDARIO_EXCHANGE, 5000, 1, 10);
    auto timeEndExchange = chrono::steady_clock::now();

    double timeExchange = chrono::duration<double, milli>(timeEndExchange - timeStartExchange).count();

    cout << "EL COSTO DE LA SOL DESPUES DE SIMAN (SWAP): " << solSWAP.getCostoSol() << "\n";
    cout << "EL COSTO DE LA SOL DESPUES DE SIMAN (EXCHANGE): " << solEXCHANGE.getCostoSol() << "\n";

    cout << "------------------------------" << endl;

    cout << "SOLUCION DEL SIMAN (SWAP)" << endl;

    for(int i = 0; i < solSWAP.getCaminos().size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < solSWAP.getCaminos()[i].size(); j++) {
            cout << solSWAP.getCaminos()[i][j].indice << ((j == solSWAP.getCaminos()[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;

    cout << "SOLUCION DEL SIMAN (EXCHANGE)" << endl;

    for(int i = 0; i < solEXCHANGE.getCaminos().size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < solEXCHANGE.getCaminos()[i].size(); j++) {
            cout << solEXCHANGE.getCaminos()[i][j].indice << ((j == solEXCHANGE.getCaminos()[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;

    cout << "GENERANDO OUTPUTS CAMINOS" << endl;

    string swapExp = "SWAP";

    string exchangeExp = "EXCHANGE";

    convertir_para_tsp(solSWAP.getCaminos(), swapExp, max_x, max_y);

    convertir_para_tsp(solEXCHANGE.getCaminos(), exchangeExp, max_x, max_y);

    generarOutput(solSWAP.getCaminos(), swapExp, lcp.getNodoBase());

    generarOutput(solEXCHANGE.getCaminos(), exchangeExp, lcp.getNodoBase());

    ofstream arSalida;

    arSalida.open("../salida/" + swapExp + "/" + swapExp + ".txt", std::ios_base::app);
    arSalida << vn.size() << "," << solSWAP.getCostoSol() << "," << timeSwap << "\n";
    arSalida.close();

    arSalida.open("../salida/" + exchangeExp + "/" + exchangeExp + ".txt", std::ios_base::app);
    arSalida << vn.size() << "," << solEXCHANGE.getCostoSol() << "," << timeExchange << "\n";
    arSalida.close();

    arSalida.open("../salida/SWEEP/SWEEP.txt", std::ios_base::app);
    arSalida << vn.size() << "," << solActual.getCostoSol() << "," << timeSweep << "\n";
    arSalida.close();

    cout << "------------------------------" << endl;

    cout << "FIN DEL PROGRAMA" << endl;

    cout << "------------------------------" << endl;

    return 0;
}

float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y){
    float costo_viaje = 0;
    float ctsp2=0;
    vector<vector<Nodo> > mat(clusterizacion.size());

    for (int i = 0; i < clusterizacion.size(); ++i) {
        vector<Nodo> nodos(clusterizacion[i].size() - 1);

        for(int j=0; j<nodos.size(); j++){
            nodos[j] = clusterizacion[i][j];
        }

        ctsp2 += calcularCostoCamino(clusterizacion[i]);
        exportar_grafo(clusterizacion[i], costo_viaje, i, nombre_in);
    }
    cout <<"Costo total: "<<ctsp2<<endl;
    return ctsp2;
}

void exportar_grafo(vector<Nodo>& camino, float costo, int ruteo, string in){
    ofstream fout;
    auto s = std::to_string(ruteo);
    fout.open("../salida/" + in + "/"+ in + "-ruteo" + s + ".csv");
    for (int i = 0; i < camino.size()-1; ++i) {
        fout << camino[i].indice<<","<<camino[i].x<<","<< camino[i].y<<endl;//indice,posx,posy
    }
    for (int j = 0; j < camino.size()-1; ++j) {
        fout << camino[j].indice<<","<<camino[j+1].indice<<endl;//indice_nodo_a,indice_nodo_b
    }
    fout<<costo;
    fout.close();
}
