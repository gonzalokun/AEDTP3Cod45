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

    cout << "------------------------------" << endl;

    lcp.mostrarLista();

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    cout << "SE CARGARON TODOS LOS DATOS" << endl;

    clusters = generarClusters(lcp, vn, capacidad);

    cout << "------------------------------" << endl;

    cout << "SE GENERARON LOS CLUSTERS" << endl;

    cout << "------------------------------" << endl;

    cout << "FIN DEL PROGRAMA" << endl;

    cout << "------------------------------" << endl;

    return 0;
}
