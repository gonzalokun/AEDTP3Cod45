#include <iostream>
#include "sweep.hpp"

using namespace std;

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;

    cargarDatos(lcp, vn, capacidad);

    //system("pause");

    cout << "------------------------------" << endl;

    lcp.mostrarLista();

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    system("pause");

    cout << "SE CARGARON TODOS LOS DATOS" << endl;

    clusters = generarClusters(lcp, vn, capacidad);

    system("pause");

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

    system("pause");

    cout << "------------------------------" << endl;

    cout << "GENERANDO OUTPUT PARA GRAFICAR" << endl;

    generarOutput(clusters);

    cout << "------------------------------" << endl;

    system("pause");

    cout << "RESOLVIENDO CAMINOS POR TSP" << endl;

    vector<vector<Nodo>> caminosSol;

    cout << "------------------------------" << endl;

    system("pause");

    cout << "FIN DEL PROGRAMA" << endl;

    cout << "------------------------------" << endl;

    return 0;
}
