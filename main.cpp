#include <iostream>
#include "include/sweep.hpp"
#include "include/siman.hpp"
#include <time.h>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

void exportar_grafo(vector<Nodo>& camino, float costo,int ruteo, string in);
float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y);

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;
    string nombreCaso;
    int kOpt;
    float costoOpt;

    srand(time(NULL));

    auto timeStartSweep = chrono::steady_clock::now();

    cargarDatos(lcp, vn, capacidad, nombreCaso, kOpt, costoOpt);

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    cout << "SE CARGARON TODOS LOS DATOS" << "\n";

    cout << "------------------------------" << "\n";

    //CALCULO EL X e Y MAXIMOS
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

    cout << "GENERANDO CLUSTERS" << "\n";

    clusters = generarClusters(lcp, vn, capacidad);

//    cout << "------------------------------" << "\n";
//
//    cout << "MOSTRANDO INDICES DE CADA CLUSTER" << "\n";
//
//    int nodosAsignados = 0;
//
//    for(int i = 0; i < clusters.size(); i++){
//        float demandaCamino = 0;
//
//        cout << "CAMINO " << i + 1 << ": [";
//        for(int j = 0; j < clusters[i].size(); j++){
//            cout << clusters[i][j].indice << ((j == clusters[i].size() - 1)? "" : ", ");
//            demandaCamino += clusters[i][j].demanda;
//            nodosAsignados++; //CUENTA EL DEPOSITO VARIAS VECES
//        }
//
//        cout << "] Demanda del camino: " << demandaCamino << " de " << capacidad << "\n";
//    }
//
//    nodosAsignados -= clusters.size() - 1; //EL DEPO SE REPITE CANT. CAMINOS - 1 VECES
//
//    cout << "CANT. NODOS ASIGNADOS: " << nodosAsignados << " DE " << vn.size() << endl;

    cout << "------------------------------" << "\n";

    cout << "RESOLVIENDO CAMINOS POR TSP" << "\n";

    vector<vector<Nodo>> caminosSol;

    //Voy solucionando todos los caminos
    for(int i = 0; i < clusters.size(); i++){
        float costoSol = 0;
        caminosSol.push_back(tsp_con_grasp(clusters[i], 0, costoSol, 1, max_x, max_y));
    }

    auto timeEndSweep = chrono::steady_clock::now();

    //Acá calculo lo que tardó en resolverlo el sweep
    double timeSweep = chrono::duration<double, milli>(timeEndSweep - timeStartSweep).count();

//    cout << "------------------------------" << endl;
//
//    cout << "MOSTRANDO CAMINOS" << endl;
//
//    for(int i = 0; i < caminosSol.size(); i++) {
//        cout << "CAMINO " << i << ": [";
//        for (int j = 0; j < caminosSol[i].size(); j++) {
//            cout << caminosSol[i][j].indice << ((j == caminosSol[i].size() - 1)? ("]") : (", "));
//        }
//        cout << endl;
//    }

    cout << "------------------------------" << endl;

    solucionProb solActual(capacidad, caminosSol);

    //Guardo el archivo
    ofstream arSalida;

    arSalida.open("../salida/clusteringSWEEP.csv", std::ios_base::app);
    arSalida << nombreCaso << "," << vn.size() << "," << solActual.getCapacidad() << "," << kOpt << "," << costoOpt << "," << "SWEEP" << "," << solActual.getCostoSol() << "," << solActual.getCaminos().size() << "," << timeSweep << "\n";
    arSalida.close();

    cout << "EL COSTO DE LA SOL ANTES DE SIMAN: " << solActual.getCostoSol() << endl;

    system("pause");

    //----------EMPIEZA SIMAN----------//

    cout << "APLICANDO SIMAN A LA SOLUCION PARA VER SI MEJORA" << endl;

    auto timeStartSimAn = chrono::steady_clock::now();
    solucionProb solSimAn = simulatedAnnealing(solActual, 5, 0.001, 0.99);
    auto timeEndSimAn = chrono::steady_clock::now();

    double timeSimAn = chrono::duration<double, milli>(timeEndSimAn - timeStartSimAn).count();

    cout << "EL COSTO DE LA SOL DESPUES DE SIMAN (EXCHANGE): " << solSimAn.getCostoSol() << "\n";

    cout << "------------------------------" << endl;

    cout << "SOLUCION DEL SIMULATED ANNEALING" << endl;

    for(int i = 0; i < solSimAn.getCaminos().size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < solSimAn.getCaminos()[i].size(); j++) {
            cout << solSimAn.getCaminos()[i][j].indice << ((j == solSimAn.getCaminos()[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;

    cout << "GENERANDO OUTPUTS CAMINOS" << endl;

    string exchangeExp = "SIMAN";

    convertir_para_tsp(solSimAn.getCaminos(), exchangeExp, max_x, max_y);

    generarOutput(solSimAn.getCaminos(), exchangeExp, lcp.getNodoBase());

    arSalida.open("../salida/" + exchangeExp + "/" + exchangeExp + ".txt", std::ios_base::app);
    arSalida << vn.size() << "," << solSimAn.getCostoSol() << "," << timeSimAn << "\n";
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
    //auto s = std::to_string(ruteo);
    std::ostringstream ss;

    ss << ruteo;

    auto s = ss.str();

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
