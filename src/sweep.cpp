#include "sweep.hpp"

#include <fstream>
#include <sstream>

#define LEYENDO_PUNTOS 0
#define DEFINIENDO_DEMANDAS 1
#define DEFINIENDO_PUNTO_CENTRAL 2

void ListaAdyacencias::convertirAListaInc(ListaIncidencia &l) {
        Arista ar;
        list<AristaAd>::iterator it;
        for (int i = 0; i < nodos.size(); ++i) {
            it = rep[i].begin();
            ar.desde = nodos[i];
            while(it!=rep[i].end()){
                ar.peso = (*it).peso;
                ar.hasta = (*it).adyacente;
                l.agregarArista(ar);
                it++;
            }
        }
}

void MatrizAdyacencias::convertirAListaInc(ListaIncidencia& li) {
    Arista ar;
    int cantAg=0;
    for (int i = 0; i < rep.size(); ++i) {
        for (int j = 0; j < rep[0].size(); ++j) {
            if(rep[i][j] !=-1) {//si hay arista que los une
                ar.peso=rep[i][j];
                ar.desde = nodos[i];

                ar.hasta = nodos[j];
                ar.indice=cantAg++;
                li.agregarArista(ar);
            }
        }
    }
}

void MatrizAdyacencias::convertirAListaAdy(ListaAdyacencias& la) {
    Arista ar;
    int cantAg=0;
    for (int i = 0; i < (rep.size()); ++i) {
        //cout <<i<<endl;
        for (int j = 0; j < rep[0].size(); ++j) {
            if(rep[i][j] !=-1) {//si hay arista que los une
                ar.peso=rep[i][j];
                ar.desde = nodos[i];
                //cout <<"agreggo arista: "<<cantAg<<" "<< nodos[i].x<<","<<nodos[i].y<<"  "<< nodos[j].x<<","<<nodos[j].y<<endl;
                ar.hasta = nodos[j];
                ar.indice=cantAg++;
                la.agregarArista(ar);
            }
        }
    }
}

//////////////////

void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn, float& capacidad){
    //Hay que leer el archivo y llenar las estructuras
    ifstream entrada;
    entrada.open("inputCPP.txt");

    int tam = 0;
    entrada >> tam;
    cout << "El tam es: " << tam << endl;
    vn = vector<Nodo>(tam);

    entrada >> capacidad;
    cout << "La capacidad es: " << capacidad << endl;

    std::string line;
    int estado = LEYENDO_PUNTOS; //0 - ingreso puntos, 1 - ingreso demandas, 2 - ingreso punto central

    while(std::getline(entrada, line)){
        //Esto es para asegurar que la primera linea leida no sea vacia
        if(line.compare("") == 0)
            continue;

        if(line.compare("Dem") == 0){
            estado = 1;
        }

        if(line.compare("Dep") == 0){
            estado = 2;
        }

        switch(estado){
        case LEYENDO_PUNTOS: {
            std::stringstream ss(line);

            int id;
            float x;
            float y;

            ss >> id >> x >> y;

            vn[id - 1] = Nodo(x, y, id);

            cout << "LEIDO ID: " << id << ", X: " << x << ", Y: " << y << endl;
            } break;

        case DEFINIENDO_DEMANDAS:{
            } break;

        case DEFINIENDO_PUNTO_CENTRAL:{
            } break;
        }
    }

    entrada.close();

    //Ya esta cargado el vector
}
