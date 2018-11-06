#ifndef SWEEP_HPP
#define SWEEP_HPP

#include <iostream>
#include <vector>
#include <climits>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>
#include <cstdio>
#include <queue>
#include <list>
#include <random>
#include <tuple>

#define PI 3.14159265358979323846

using namespace std;

typedef float Peso;

float cantDespuesDeComa(const float& x, const int& numDecimals) {
    int y = x;
    float z = x-y;
    float m = pow(10,numDecimals);
    float q = z*m;
    float r = round(q);

    return static_cast<double>(y)+(1.0/m)*r;
}

struct Nodo {
    float x,y;
    int indice;
    float demanda;

    Nodo() : indice(-1){}

    Nodo(float x, float y, int indice) {
        this->y = y;
        this->x = x;
        this->indice = indice;
    }

    //Nodo (Nodo &n) : x(n.x), y(n.y), indice(n.indice) {}
    bool operator==(Nodo &n) {
        return(this->x == n.x && this->y == n.y);
    }

    bool operator!=(Nodo& n) {
        return(this->x != n.x || this->y != n.y);
    }
};

struct AristaAd{
    Nodo adyacente;
    Peso peso;
    AristaAd(){}
    AristaAd(Nodo a, Peso p) : adyacente(a), peso(p){}
};

struct Arista {
    Nodo desde;
    Nodo hasta;
    long long int indice;
    Peso peso;
    Arista():indice(-1){}
    //Arista(Arista &ar) :desde(ar.desde), hasta(ar.hasta), peso(ar.peso), indice(ar.indice) {}
    Arista(Nodo d, Nodo h, Peso p) : desde(d), hasta(h), peso(p),indice(-1) {}
    Arista(Nodo d, Nodo h, Peso p, int i) : desde(d), hasta(h), peso(p),indice(i) {}
    Arista(pair<Nodo, Nodo> a, Peso p) : desde(a.first), hasta(a.second), peso(p), indice(-1) {}
    bool operator==(Arista &ar) {
        return((ar.hasta == (*this).hasta && ar.desde == (*this).desde) || (ar.desde == (*this).hasta && ar.hasta == (*this).desde));
        //return (ar.hasta == (*this).hasta && ar.desde == (*this).desde && (*this).indice == ar.indice);
    }

    bool operator<(Arista &ar){
        if(this->indice == -1) return false;
        if(ar.indice == -1) return true;
        return ((*this).peso < ar.peso);
    }

};

class ListaIncidencia;
class ListaAdyacencias;

class MatrizAdyacencias {
private:
    vector< vector<Peso> > rep;
    vector<Nodo> nodos;
public:
    MatrizAdyacencias(const MatrizAdyacencias& c) : rep(c.rep) {}
    MatrizAdyacencias(int cantidad_de_nodos) {
        rep.resize(cantidad_de_nodos);
        for (int i = 0; i < cantidad_de_nodos; ++i) {
            for (int j = 0; j < cantidad_de_nodos; ++j) {
                rep[i].push_back(-1);
            }
        }
        nodos.resize(cantidad_de_nodos);
    }
    Peso getArista(int nodoA,int nodoB) {
        return rep[nodoA][nodoB];
    }
    void convertirAListaInc(ListaIncidencia& li);
    void agregarArista(Arista ar){
        nodos[ar.desde.indice]=ar.desde;
        nodos[ar.hasta.indice]=ar.hasta;
        rep[ar.desde.indice][ar.hasta.indice] = ar.peso;
    }
    Nodo& getNodo(int i) {
        return nodos[i];
    }
    void sacarArista(int a, int b){
        rep[a][b] = -1;
        rep[b][a] = -1;
    }
    int size() { return (int)rep.size(); }
    vector<Peso>& operator[](int x) { return rep[x]; }
    void operator=(const MatrizAdyacencias& m) { rep = m.rep; }
    void mostrar(){
        int most = 0;
        for (int i = 0; i < rep.size(); ++i) {
            for (int j = 0; j < rep[0].size(); ++j) {
                if(rep[i][j] != -1) {

                    cout <<"ARISTA: "<<(most++)<<"("<<nodos[i].x<<","<<nodos[i].y <<")  "<<nodos[j].x<<","<<nodos[j].y<<endl;
                }
            }
        }
    }
    void convertirAListaAdy(ListaAdyacencias& la);

};

class ListaIncidencia;
class ListaAdyacencias {
private:
    //vector< vector<Nodo> > rep;
    //vector< vector<std::pair<Nodo, Peso>> > rep;
    vector< list< AristaAd > > rep;
    vector<Nodo> nodos;
public:
    ListaAdyacencias() {}
    ListaAdyacencias(const ListaAdyacencias& c) : rep(c.rep) {}
    ListaAdyacencias(int cantidad_de_nodos) {
        rep.resize(cantidad_de_nodos);
        nodos.resize(cantidad_de_nodos);
    }
    void convertirAListaInc(ListaIncidencia& l);
    void sacarArista(Arista ar) { //O(grado(ar.desde)
        list<AristaAd >::iterator it = rep[ar.desde.indice].begin();
        while(it != rep[ar.desde.indice].end() && (*it).adyacente.indice!=ar.hasta.indice) {
            it++;
        }
        if((*it).adyacente.indice== ar.hasta.indice) rep[ar.desde.indice].erase(it);

        it = rep[ar.hasta.indice].begin();
        while(it != rep[ar.hasta.indice].end() && (*it).adyacente.indice!=ar.desde.indice) {
            it++;
        }
        if((*it).adyacente.indice == ar.desde.indice) rep[ar.hasta.indice].erase(it);
    }
    vector<Nodo>& getNodos(){
        return nodos;
    }
    int size(){
        return nodos.size();
    }
    void sacarArista(int na, int nb) {
        list<AristaAd >::iterator it = rep[na].begin();
        while(it != rep[na].end() && (*it).adyacente.indice!=nb) {
            it++;
        }
        if((*it).adyacente.indice== nb) rep[na].erase(it);

        it = rep[nb].begin();
        while(it != rep[nb].end() && (*it).adyacente.indice!=na) {
            it++;
        }
        if((*it).adyacente.indice == na) rep[nb].erase(it);
    }

    list<AristaAd >& operator[](Nodo x) {
        return rep[x.indice];
    }
    bool esAdy(int i, int j){
        list<AristaAd>::iterator it = (rep[i]).begin();
        while(it != rep[i].end() && (*it).adyacente != getNodo(j)){
            it++;
        }
        if((it != rep[i].end()))
            return true;
        else return false;
    }

    //void operator=(ListaAdyacencias& m) { rep(m.rep); }
    void agregarArista(Arista& ar) {
        AristaAd p(ar.hasta,ar.peso);
        AristaAd p2(ar.desde, ar.peso);
        nodos[ar.desde.indice] = ar.desde;
        rep[ar.hasta.indice].push_back(p2);
        nodos[ar.hasta.indice] = ar.hasta;
        rep[ar.desde.indice].push_back(p);
    }

    void mostrar(){
        for (int i = 0; i < rep.size(); ++i) {
            list<AristaAd>::iterator it = rep[i].begin();
            while (it!= rep[i].end()) {
                cout <<"Arista: ("<< nodos[i].x<<","<<nodos[i].y<<")   (" <<(*it).adyacente.x<<","<<(*it).adyacente.y<<")"<<endl;
                it++;
            }
        }
    }
    Nodo& getNodo(int i){
        return nodos[i];
    }

    //devuelvo el vector con los pesos de los ejes de los vecinos de k pasos de noda
    vector<Peso> pesosVecinosDeKPasosDesdeNodo(Nodo &nodo, Nodo &b, int k){//O(k * (grado(nodo)-1), O(k*m)
        vector<Peso> pesos, aux;
        queue<Nodo> colaDeNodos;
        Nodo nodoactual;
        //pongo en cola todos los nodos que se llegan en k pasos desde nodo
        colaDeNodos.push(nodo);
        while(k > 1 && colaDeNodos.size()!=0){
            nodoactual = colaDeNodos.front();
            for(auto l : rep[nodoactual.indice]){ //O(m)+O(1)
                if( l.adyacente!=b) {
                    colaDeNodos.push(l.adyacente);
                }
            }
            //tengo que remover el primero en haber ingresado
            //pero antes tengo que ver sus vecinos
            aux = pesosDeVecinosDesdeNodo(nodoactual, b);
            for(auto p: aux) {
                pesos.push_back(cantDespuesDeComa(p,3));
            }
            //elimino el primero de la cola
            colaDeNodos.pop();
            k--;
        }
        //vacio la cola
        while(colaDeNodos.size() > 0) {

            nodoactual = colaDeNodos.front();
            aux = pesosDeVecinosDesdeNodo(nodoactual,b);
            nodoactual = colaDeNodos.front();
            for(auto p: aux) {
                pesos.push_back(cantDespuesDeComa(p,3));
            }
            colaDeNodos.pop();
        }
        return pesos;
    }

    vector<Peso> pesosDeVecinosDesdeNodo(Nodo& nodo, Nodo& b) { //O(m)
        vector<Peso> p;
        //Aca chequeamos que los pesos que agregamos sean desde el "lado" del vecindario que NO pertenece b
        for(auto l : rep[nodo.indice]){
            if (not(l.adyacente==b) ) {
                //cout <<"meto peso de: "<<nodo.x<<","<<nodo.y<<" "<<l.adyacente.x<<","<<l.adyacente.y<<" sin pertenecer a :" << b.x<<","<<b.y;
                p.push_back(cantDespuesDeComa(l.peso,3));
            }
        }
        return p;
    }
};

class ListaIncidencia {
private:
    vector< Arista> rep;
    long long int aristasActuales;
    long long int aristasTotales;
public:

    ListaIncidencia(){
        aristasTotales = 0;
        aristasActuales = 0;
    }
    /*~ListaIncidencia(){
        for (int i = 0; i < rep.size(); ++i) {
            delete rep[i];
        }
    }*/
    ListaIncidencia(const ListaIncidencia &c) : rep(c.rep){
        this->aristasActuales = c.aristasActuales;
        this->aristasTotales = c.aristasTotales;
    };
    ListaIncidencia(long long int cantidad_de_aristas) {

        for (int i = 0; i < cantidad_de_aristas; ++i) {
            rep.push_back(Arista());
        }

        this->aristasActuales = 0;
        this->aristasTotales = cantidad_de_aristas;
    }
    void convertirALista(ListaAdyacencias& la) {//O(m * m)
        for (int i = 0; i < rep.size(); ++i) {//recorro aristas de la lista de incidencia
            if(rep[i].indice!=-1) {
                la.agregarArista(rep[i]);
            }//O(grado de rep[i].desde)
        }
    }
    void agregarArista(Arista& a) { //pre: al iniciar la lista de incidencia, ya sabemos la cantidad de aristas.
        //if(!existeArista(a.desde,a.hasta)) {
            if(aristasTotales > aristasActuales) {
                a.indice = aristasActuales;
                rep[aristasActuales] = a;
                aristasActuales++;
            }else{
                a.indice = aristasActuales;
                rep.push_back(a);
                aristasActuales++;
                aristasTotales++;
            }
    }
    Arista& getArista(int in) {
        return rep[in];
    }
    bool existeArista(Nodo a, Nodo b){ //O(m)
        Arista ar;
        ar.desde = a;
        ar.hasta = b;
        for (int i = 0; i < cantidad_aristas(); ++i) {
            if(rep[i] == ar) return true;
        }
        return false;
    }
    Arista& getArista(Nodo a, Nodo b) {
        return getArista(indiceArista(a,b));
    }
    Nodo& getPrimerNodo(Arista& ar){
        return ar.desde;
    }
    Nodo& getSegundoNodo(Arista& ar) {
        return ar.hasta;
    }
    Nodo& getPrimerNodo(int ar){
        return rep[ar].desde;
    }
    Nodo& getSegundoNodo(int ar) {
        return rep[ar].hasta;
    }
    int indiceArista(Nodo a, Nodo b) {
        for (int i = 0; i < aristasTotales; ++i) {
            if(a == rep[i].desde || a == rep[i].hasta){ //si existe la arista de nodos que ingrese
                //sacarArista(ar);
                return i;
            }
        }
        return -1;
    }
    void sacarArista(Nodo a , Nodo b) {
        sacarArista(getArista(indiceArista(a,b)));
    }

    //Pre:existe la arista
    void sacarArista(Arista a) {
        int i;
        rep[a.indice].indice=-1; //deshabilitamos esa arista
        aristasActuales--;
        aristasTotales--;
        /*Arista aux = rep[a.indice];
        rep[a.indice] = rep[rep.size()-1];
        rep[rep.size()-1] = aux;
        rep.pop_back();
        aristasTotales=rep.size();
        aristasActuales--;*/
    }
    int cantidad_aristas() {
        return aristasTotales;
    }

    void operator=(ListaIncidencia& m) { rep = m.rep; }

    void ordenarPorPeso(){//ordena las aristas por pes de menor a mayor
        sort(rep.begin(),rep.end());//O(LOG M * M)
        for (int i = 0; i < cantidad_aristas(); ++i) {//O(M)
            if(rep[i].indice !=-1) {
                rep[i].indice = i;//actualizo indices de aristas
            }
        }
    }

    void mostrar(){
        for (int i = 0; i < rep.size(); ++i) {
            if(rep[i].indice!=-1) cout<<"Arista " << i <<" : [" <<rep[i].desde.indice<<"(" << rep[i].desde.x << " , " <<  rep[i].desde.y << ") , "<<rep[i].hasta.indice << "("<<rep[i].hasta.x <<" , "<<rep[i].hasta.y <<")]"<< " peso: "<< rep[i].peso<<endl;
        }
    }

};

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

//Ac� pongo lo del Sweep

struct NodoPol{
    int id;
    float distacia;
    float angulo;

//    float x;
//    float y;

    NodoPol(){
        this->id = -1;
        this->distacia = 0;
        this->angulo = 0;
    }

    NodoPol(int id, float distacia, float angulo){
        this->id = id;
        this->distacia = distacia;
        this->angulo = angulo;
    }

    bool operator<(NodoPol& n){
//        if(this->angulo < n.angulo || this->distacia < n.distacia || this->id < n.id){
//            return true;
//        }
//
//        return false;
        return (this->angulo < n.angulo || this->distacia < n.distacia || this->id < n.id);
    }
};

class ListaCordPol{
private:
    //Guarda el id, el �ngulo y la distancia de cada punto
    //Ordenado por �ngulo, distancia, id
    priority_queue<NodoPol> listaCP;
    Nodo centro;
public:
    ListaCordPol(){
        //Por ahora nada
    }

    void setearNodoBase(Nodo n){
        centro = n;
    }

    void agregarNodo(Nodo n){
        NodoPol np;
        np.id = n.indice;

        //Se calcula la distancia y el �ngulo en base al punto del centro
        float newX = n.x - centro.x;
        float newY = n.y - centro.y;

        np.distacia = sqrt(pow(newX) + pow(newY));
        np.angulo = atan2(n.y, newX) * 180 / PI;

        listaCP.push(np);
    }

    NodoPol siguiente(){
        listaCP.top();
    }

    void pop(){
        listaCP.pop();
    }
};

void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn);

#endif // SWEEP_HPP
