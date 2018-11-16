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
            estado = DEFINIENDO_DEMANDAS;
            continue;
        }

        if(line.compare("Dep") == 0){
            estado = DEFINIENDO_PUNTO_CENTRAL;
            continue;
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
            std::stringstream ss(line);

            cout << "LA LINEA DE TEXTO ES " << line << endl;

            int id;
            float demanda;

            ss >> id >> demanda;

            cout << "ID LEIDO ES " << id << endl;
            cout << "DEMANDA LEIDA ES " << demanda << endl;

            vn[id - 1].demanda = demanda;

            cout << "DEMANDA DE NODO " << id << " ES " << vn[id - 1].demanda << endl;
            } break;

        case DEFINIENDO_PUNTO_CENTRAL:{
            //ESTO SOLO DEBERIA EJECUTARSE UNA VEZ
            std::stringstream ss(line);

            int id;

            ss >> id;

            cout << "PUNTO CENTRAL LEIDO ES " << id << endl;

            if(id != -1){
                lcp.setearNodoBase(vn[id - 1]);
            }

            cout << "SE SETEO COMO PUNTO CENTRAL EL NODO CON ID " << lcp.getNodoBase().indice << endl;

            } break;
        }
    }

    entrada.close();

    cout << "AHORA SE CARGA LA LISTA DE COORD. POLARES" << endl;

    //Ya esta cargado el vector
    //Ahora cargo la lista Coord. Polares
    for(int i = 0; i < vn.size(); i++){
        //cout << "Voy a agregar al nodo con id: " << i << endl;
        //Agrego todos los puntos menos el de depósito, que sabemos que siempre va a estar en el camino
        if(i + 1 != lcp.getNodoBase().indice){
            cout << "SE AGREGA A LA LISTA EL NODO CON ID: " << vn[i].indice << endl;
            lcp.agregarNodo(vn[i]);
        }
    }

    cout << "TERMINO LA CARGA DE DATOS" << endl;
    //Debería estar cargada la lista
}

//Ahora hacemos el sweep
vector<vector<Nodo>> generarClusters(ListaCordPol& lcp, const vector<Nodo>& vn, float capacidad){
    vector<vector<Nodo>> vecClusters;

    float capActual = 0;
    vector<Nodo> vecActual;
    vecActual.push_back(lcp.getNodoBase());

    int cont = 0;

    while(!lcp.vacia()){
        cout << "Ciclo: " << cont << endl;
        cout << "Tam. Lista: " << lcp.tam() << endl;
        if(capActual + lcp.siguiente().demanda <= capacidad){
            cout << "ID DE NODO A AGREGAR: " << lcp.siguiente().id << endl;
            cout << "DEMANDA DEL NODO: " << lcp.siguiente().demanda << endl;
            capActual += lcp.siguiente().demanda;
            vecActual.push_back(vn[lcp.siguiente().id - 1]);
            lcp.pop();
        }
        else{
            //Inserto el conjunto y reseteo las variables
            capActual = 0;
            vecClusters.push_back(vecActual);

            vecActual.clear();
            vecActual.push_back(lcp.getNodoBase());
        }

        cont++;
    }

    //Agrego el ultimo cluster que siempre se forma
    vecClusters.push_back(vecActual);

//    if(!vecActual.empty()){
//        vecClusters.push_back(vecActual);
//    }

    return vecClusters;
}

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p){
    vector<Nodo> camino;

     //contiene los nodos del cluster
    vector<vector<float> > distancias_entre_nodos;
    vector<bool> visitados(nodos.size(),false);
    distancias_entre_nodos.resize(nodos.size());

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> proba_p(0,1);

    for (int i = 0; i < nodos.size(); i++) {
        distancias_entre_nodos[i].resize(nodos.size());
        for (int j = 0; j < nodos.size(); ++j) {
            if(i!=j)
                distancias_entre_nodos[i][j] = distancia_euclidea(nodos[i].x,nodos[i].y,nodos[j].x,nodos[j].y);
            else
                distancias_entre_nodos[i][j] = MAX_Y*MAX_X;
        }
    }

    bool hay_nodos = true;
    int act = nodo_comienzo;
    int posible_nodo = -1;
    camino.push_back(nodos[nodo_comienzo]);
    while(hay_nodos){
        visitados[act] = true;
        float minDist = MAX_X*MAX_Y;
        int posMin = -1;
        for (int i = 0; i < nodos.size(); ++i) {
            if(i!= act && visitados[i]==false){
                posible_nodo = i;
            }
            if( i!= act && visitados[i]==false && minDist > distancias_entre_nodos[act][i] && proba_p(rng)<=p){
                if(proba_p(rng) <= p) {
                    minDist = distancias_entre_nodos[act][i];
                    posMin = i;
                }else{
                    if(posible_nodo == -1) { //significa que no hay todavia no no visitado al que pueda ir, entonces decido ir al minimo (porque a priori no tengo otra pc)
                        minDist = distancias_entre_nodos[act][i];
                        posMin = i;
                    }else {
                        minDist = distancias_entre_nodos[act][posible_nodo];
                        posMin = posible_nodo;
                    }
                }

            }
        }
        if(posMin == -1) { //Significa que no hay mas que visitar
            hay_nodos= false;
            costo_viaje += distancias_entre_nodos[act][nodo_comienzo];
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[0].indice<<endl;
            camino.push_back(nodos[nodo_comienzo]);
        }else{
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[posMin].indice<<endl;
            camino.push_back(nodos[posMin]);
            costo_viaje+=minDist;
            act = posMin;
        }
    }

    return camino;
}

void generarOutput(vector<vector<Nodo>>& clusters){
    int cantClusters = 1;

    ofstream salida;
    salida.open("output.csv");

    for(int i = 0; i < clusters.size(); i++){
        for(int j = 0 ; j < clusters[i].size(); j++){
            salida << clusters[i][j].indice << ",";
            salida << clusters[i][j].x << ",";
            salida << clusters[i][j].y << ",";
            salida << cantClusters << endl;
        }
        cantClusters++;
    }

    salida.close();
}
