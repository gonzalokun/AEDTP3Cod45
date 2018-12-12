#include <iostream>
#include "include/sweep.hpp"
#include "include/siman.hpp"
#include "include/System.h"
#include "include/clark_wright.h"
#include "include/basics3.hpp"
#include <time.h>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

float MAX_X, MAX_Y;

class cluster{
public:

    cluster(){
        x = 0;
        y = 0;
        demanda_total = 0;
        carga_actual = 0;
        cant_nodos = 0;
    }
    void setCantidadNodos(int cn){
        nodos.clear();
        nodos.resize(cn);
        Nodo n;
        for (int i = 0; i < nodos.size(); ++i) {
            nodos[i] = n;
        }
    }
    cluster(const cluster& cl) : nodos(cl.nodos) {    }

    void operator=(const cluster& cl){
        this->x=cl.x;
        this->y =cl.y;
        this->nodos = (cl.nodos);
        this->deposito = cl.deposito;
        this->demanda_total = cl.demanda_total;
        this->cant_nodos = cl.cant_nodos;
        this->carga_actual = cl.carga_actual;
    }
    int getCantNodos(){
        return cant_nodos;
    }
    float getDemandaTotal(){
        return demanda_total;
    }
    float getCargaActual(){
        return carga_actual;
    }
    void setDeposito(Nodo& dep) {
        this->deposito = dep;
    }
    void setDemanda(float d) {
        this->demanda_total = d;
    }
    float getX(){
        return x;
    }
    float getY(){
        return y;
    }
    void setX(float x){
        this->x=x;
    }
    void setY(float y){
        this->y=y;
    }
    float getAvgX(){
        return (acum_pos_x_nodos/cant_nodos);
    }
    float getAvgY(){
        return (acum_pos_y_nodos/cant_nodos);
    }
    Nodo& dame_nodo(int& desde) {
        while (nodos[desde].indice==-1) {
            desde++;
        }
        return nodos[desde];
    }

    int nodo_mas_lejano(float demanda){ //devuelvo el nodo que esta más alejado del centro del cluster y que tiene demanda igual o mayor a la del nodo que quiero agregar
        float maxDist = 0, dem;
        int posMax = -1;

        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice!=-1&& nodos[i].demanda <= demanda && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                maxDist = distancia_de_nodo_al_centro(nodos[i]);
                posMax = i;
            }
        }
        if(posMax==-1){
            maxDist = 0;
            for (int i = 2; i <nodos.size(); ++i) {
                if(nodos[i].indice!=-1 && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                    maxDist = distancia_de_nodo_al_centro(nodos[i]);
                    posMax = i;
                }
            }
        }
        return posMax;
    }


    int nodo_mas_lejano() {

        float maxDist = 0, dem;
        int posMax = -1;

        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice!=-1 && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                maxDist = distancia_de_nodo_al_centro(nodos[i]);
                posMax = i;
            }
        }
        return posMax;
    }

    void agregar_nodo(Nodo &n){
        if(nodos[n.indice].indice == -1){
            nodos[n.indice] = n;
            cant_nodos++;
            carga_actual+=n.demanda;
            acum_pos_x_nodos+= n.x;
            acum_pos_y_nodos+=n.y;
        }
    }
    bool tiene_exceso(){
        return ((demanda_total < carga_actual)? true : false);
    }
    void eliminar_nodo(Nodo &n){
        if(nodos[n.indice].indice!=-1){
            carga_actual-=n.demanda;
            acum_pos_x_nodos-=n.x;
            acum_pos_y_nodos-=n.y;
            Nodo a;
            a.indice=-1;
            nodos[n.indice]=a;
            cant_nodos--;
        }

    }
    bool existeNodo(Nodo& nodo) {

        if( nodos[nodo.indice].indice!=-1) {
            return true;
        }else return false;
    }
    float distancia_de_nodo_al_centro(Nodo &n) {
        if(nodos[n.indice].indice == -1) return MAX_Y * MAX_X;
        return distancia_euclidea(x,y,n.x,n.y);
    }

    bool operator<(cluster &c){
        if(distancia_euclidea(0,0,this->x,this->y) < distancia_euclidea(0,0,c.x,c.y)){
            return true;
        }else
            return false;
    }

    vector<Nodo> getNodos(){
        vector<Nodo> aux;
        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice != -1)
                aux.push_back(nodos[i]);
        }
        return aux;
    }

    void clear_cluster() {
        for (int i = 0; i < nodos.size(); ++i) {
            eliminar_nodo(nodos[i]);
        }

        x = 0;
        y = 0;
        demanda_total = 0;
        acum_pos_x_nodos=0;
        acum_pos_y_nodos=0;
        carga_actual = 0;
        cant_nodos = 0;
    }

private:
    float acum_pos_x_nodos, acum_pos_y_nodos;
    float demanda_total, carga_actual;
    float x, y;
    int id_nodo_mas_cercano;
    Nodo deposito;
    int cant_nodos;
    vector<Nodo> nodos;
};

class k_means{
public:
    k_means(){
    }
    k_means(int cn, int cc, Nodo dep, float cap_total){
        cantidad_de_nodos = cn;
        cluster_de_nodo.resize(cn+1);
        for (int i = 0; i < cluster_de_nodo.size(); ++i) {
            cluster_de_nodo[i] = -1;
        }
        deposito = dep;
        cantidad_de_clusters = cc;
        clusters.resize(cc);
        capacidad = cap_total;
    }
    //pre: tiene cluster asigado
    int getClusterDeNodo(int i){
        return cluster_de_nodo[i];
    }
    cluster& getClusterIesimo(int i){
        return clusters[i];
    }
    cluster& getClusterDeNodoIesimo(int i) {
        return clusters[cluster_de_nodo[i]];
    }

    void operator=(const k_means& c){
        cluster_de_nodo = c.cluster_de_nodo;
        clusters.resize(c.cantidad_de_clusters);

        for (int i = 0; i < clusters.size(); ++i) {
            clusters[i] = c.clusters[i];
        }
        deposito = c.deposito;
        cantidad_de_nodos = c.cantidad_de_nodos;
        capacidad = c.capacidad;
        cantidad_de_clusters = c.cantidad_de_clusters;
    }
//    void generar_csv(string nombre){
//        ofstream fout;
//        fout.open("../salida/" + nombre+"/"+nombre +".csv");
//        fout <<deposito.indice<<","<<deposito.x<<","<<deposito.y<<","<<cantidad_de_clusters+1<<endl;
//        for (int i = 0; i < cantidad_de_clusters; ++i) {
//            vector<Nodo> nodos = getClusterIesimo(i).getNodos();
//            for(int j = 0; j < nodos.size(); j++) {
//                if(nodos[j].indice!=-1) fout <<nodos[j].indice<<","<<nodos[j].x<<","<<nodos[j].y<<","<<i<<endl;
//
//            }
//        }
//        fout.close();
//    }
//    void eliminar_cluster(int i){
//
//    }

    int cluster_mas_cercano(int c1) {//Devuelvo el cluster más cercano y con menos demanda posible
        float minDist = MAX_X*MAX_Y;
        int minInd = 0;
        for (int i = 0; i < clusters.size(); ++i) {
            if(i != c1) {
                if(minDist>distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY())){
                    minDist = distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY());
                    minInd = i;
                }
            }
        }
        float minDist2 = MAX_Y*MAX_X;
        int minInd2 = 0;
        for (int i = 0; i < clusters.size(); ++i) {
            if(i != c1) {
                if(distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY()) > minDist && minDist2 > distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY())){
                    minDist2 = distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY());
                    minInd2 = i;
                }
            }
        }
        return ( (clusters[minInd].getCargaActual() > clusters[minInd2].getCargaActual())? minInd2 : minInd);
    }

    void juntar_clusters(int c1, int c2){
        if(getClusterIesimo(c1).getCantNodos()> getClusterIesimo(c2).getCantNodos()){
            for (int i = 0; i < getClusterIesimo(c1).getNodos().size(); ++i) {
                if(getClusterIesimo(c1).getNodos()[i].indice!=-1) {
                    agregar_nodo_a_cluster(getClusterIesimo(c1).getNodos()[i],c2);
                }
            }
        }
    }
    void agregar_nodo_a_cluster(Nodo &n, int icluster){
        if(cluster_de_nodo[n.indice] != -1) {
            clusters[cluster_de_nodo[n.indice]].eliminar_nodo(n);
        }
        cluster_de_nodo[n.indice] = icluster;
        clusters[icluster].agregar_nodo(n);
    }

    void agregar_nodo_a_cluster_mas_cercano (Nodo &n){
        int posClusterMin = -1;
        float minDist = MAX_X*MAX_Y;
        if(cluster_de_nodo[n.indice]==-1) {
            for (int i = 0; i < clusters.size(); ++i) {
                if(clusters[i].getCargaActual() + n.demanda <= capacidad && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                    minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                    posClusterMin = i;
                }
            }
            if(posClusterMin==-1) {
                for (int i = 0; i < clusters.size(); ++i) {
                    if(minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                        minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                        posClusterMin = i;
                    }
                }
            }
        }else {
            for (int i = 0; i < clusters.size(); ++i) {
                if(cluster_de_nodo[n.indice]!=i && clusters[i].getCargaActual() + n.demanda <= capacidad && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                    minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                    posClusterMin = i;
                }
            }
            if(posClusterMin==-1) {
                for (int i = 0; i < clusters.size(); ++i) {
                    if(cluster_de_nodo[n.indice]!=i && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                        minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                        posClusterMin = i;
                    }
                }
            }
        }
        cluster_de_nodo[n.indice] = posClusterMin;
        clusters[posClusterMin].agregar_nodo(n);
    }

    void eliminar_nodo_de_cluster(Nodo &n, int icluster){
        cluster_de_nodo[n.indice] = -1;
        clusters[icluster].eliminar_nodo(n);
    }

    bool nodo_tiene_cluster(int i) {
        if(cluster_de_nodo[i]==-1){
            return false;
        }else return true;
    }
    float get_distancia_de_nodo_a_cluster(Nodo &n){
        if(n.indice==-1) return MAX_X*MAX_Y;
        //cout <<n.indice<<endl;
        int a = n.indice;
        if(n.indice> cantidad_de_nodos) return MAX_Y*MAX_X;
        if(cluster_de_nodo[n.indice] == -1 ) return MAX_Y*MAX_X;
        return distancia_euclidea(n.x,n.y,clusters[cluster_de_nodo[n.indice]].getX(),clusters[cluster_de_nodo[n.indice]].getY());
    }
    int cant_clusters(){
        return cantidad_de_clusters;
    }

    void mostrar_clusters(){
        for (int i = 0; i < cantidad_de_clusters; ++i) {
            cout <<"Cluster " << i<<" cantidad de nodos: "<< clusters[i].getCantNodos()<<" porcentaje completo de carga: " <<clusters[i].getCargaActual()<<"/"<< clusters[i].getDemandaTotal()<<endl;
        }
    }
    bool existe_cluster_con_exceso(){
        int i = 0;
        while(i < clusters.size()&&clusters[i].getCargaActual()<=clusters[i].getDemandaTotal()) {
            i++;
        }
        return ((i==clusters.size())? false:true);
    }

    void clear_clusters(){
        for (int j = 0; j < cluster_de_nodo.size(); ++j) {
            cluster_de_nodo[j] = -1;
        }
        for (int i = 0; i < clusters.size(); ++i) {
            clusters[i].clear_cluster();
        }
    }
    void agregar_cluster(){
        cantidad_de_clusters++;
        clusters.resize(cantidad_de_clusters);
    }

    void mostrar_clusters_completo(){
        int cant = 0;
        vector<bool> v(cantidad_de_nodos+1,false);
        for (int i = 0; i < cantidad_de_clusters; ++i) {
            cout <<"Cluster " << i<<" cantidad de nodos: "<< clusters[i].getCantNodos()<<" porcentaje completo de carga: " <<clusters[i].getCargaActual()<<"/"<< clusters[i].getDemandaTotal()<<" Nodos: "<<endl;
            for (int j = 0; j < getClusterIesimo(i).getNodos().size(); ++j) {
                if(getClusterIesimo(i).getNodos()[j].indice!=-1) {
                    cout << getClusterIesimo(i).getNodos()[j].indice <<",";
                    cant++;
                }
            }
            cout <<endl<<endl;
        }
        cout <<"Cantidad total de nodos asignados a cluster: "<<cant<<endl;
    }
    int cluster_mas_cercano_a_nodo(Nodo &n) {
        float distMin = MAX_X*MAX_Y;
        int indiceMin;
        for (int i = 0; i < cant_clusters(); ++i) {
            if(cluster_de_nodo[n.indice]!=i && distMin > distancia_euclidea(n.x,n.y, clusters[i].getX(),clusters[i].getY())){
                distMin = distancia_euclidea(n.x,n.y, clusters[i].getX(),clusters[i].getY());
                indiceMin = i;
            }
        }
        return indiceMin;
    }
    float getCapacidad(){
        return this->capacidad;
    }
    void agregar_cluster(cluster &c){
        clusters.push_back(c);
    }
private:
    vector<int> cluster_de_nodo;
    vector<cluster> clusters;
    Nodo deposito;
    int cantidad_de_nodos;
    int cantidad_de_clusters;
    float capacidad;
};

void exportar_grafo(vector<Nodo>& camino, float costo,int ruteo, string in);
float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y);
float convertir_para_tsp2(k_means& clusterizacion, string nombre_in, Nodo depo, float MAX_X, float MAX_Y);

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;
    string nombreCaso;
    int kOpt;
    float costoOpt;
    string clusteringUsado;
    vector<vector<Nodo>> caminosSol;

    //clusteringUsado = "SWEEP";

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

    //SWEEP
    cout << "GENERANDO CLUSTERS" << "\n";

    clusters = generarClusters(lcp, vn, capacidad);

    cout << "------------------------------" << "\n";

    cout << "RESOLVIENDO CAMINOS POR TSP" << "\n";

    //Voy solucionando todos los caminos
    for(int i = 0; i < clusters.size(); i++){
        float costoSol = 0;
        caminosSol.push_back(tsp_con_grasp(clusters[i], 0, costoSol, 1, max_x, max_y));
    }

    auto timeEndSweep = chrono::steady_clock::now();

    //Acá calculo lo que tardó en resolverlo el sweep
    double timeSweep = chrono::duration<double, milli>(timeEndSweep - timeStartSweep).count();

    cout << "------------------------------" << endl;

    //FIN SWEEP

//    //HEURISTICAS DE DYLAN - GABI
//
//    clusteringUsado = "SAVINGS";
//    //ACA SE USAN:
//    //  Savings
//    //  Golosa 1 - Vecino Más Cercano (SE USA ESTA)
//
//    System entrada(vn.size());
//    entrada.setCapacity((unsigned int) capacidad);
//
//    //El primero agregado es el depo
//    entrada.addCustomer((unsigned int) lcp.getNodoBase().indice, lcp.getNodoBase().x, lcp.getNodoBase().y);
//    entrada.setDemand((unsigned int) lcp.getNodoBase().indice, 0);
//
//    for(int i=0; i < vn.size(); i++){
//        if(vn[i] == lcp.getNodoBase())
//            continue;
//        entrada.addCustomer((unsigned int) vn[i].indice, vn[i].x, vn[i].y);
//        entrada.setDemand((unsigned int) vn[i].indice, (unsigned int)  vn[i].demanda);
//    }
//
////    Respuesta res = pesoCaminoTotalCW2(entrada);
//    Respuesta res = heuristicaGolosa(entrada);
//
//    for(Route ruta : res.obtenerRutas()){
//        vector<Nodo> camino;
//        camino.push_back(lcp.getNodoBase());
//        for(Customer cus : ruta.getRoute()){
//            camino.push_back(vn[cus.getID() - 1]);
//        }
//        camino.push_back(lcp.getNodoBase());
//        caminosSol.push_back(camino);
//    }
//
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
//
//    cout << "------------------------------" << endl;

    //system("pause");

    //HEURISTICA DE DIEGO

//    clusteringUsado = "ALTSWEEP";

    //

    //FIN HEURISTICA DIEGO

    solucionProb solActual(capacidad, caminosSol);

    //k_means clust = altSweep(vn, max_x, max_y, lcp.getNodoBase());

    cout << "SE SOBREVIVIO" << endl;

    system("pause");

    //Guardo el archivo
    ofstream arSalida;

//    arSalida.open("../salida/clusteringSWEEP.csv", std::ios_base::app);
//    arSalida << nombreCaso << "," << vn.size() << "," << solActual.getCapacidad() << "," << kOpt << "," << costoOpt << "," << "SWEEP" << "," << solActual.getCostoSol() << "," << solActual.getCaminos().size() << "," << timeSweep << "\n";
//    arSalida.close();

    cout << "EL COSTO DE LA SOL ANTES DE SIMAN: " << solActual.getCostoSol() << endl;

    //system("pause");

    //----------EMPIEZA SIMAN----------//

    cout << "APLICANDO SIMAN A LA SOLUCION PARA VER SI MEJORA" << endl;

    int cantPasos = 1000;
    float tempMax = 20;
    float coefEnfriamiento = 0.99;

    auto timeStartSimAn = chrono::steady_clock::now();
    solucionProb solSimAn = simulatedAnnealing(solActual, cantPasos, tempMax, coefEnfriamiento);
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

//    arSalida.open("../salida/SWEEP/SWEEP.txt", std::ios_base::app);
//    arSalida << vn.size() << "," << solActual.getCostoSol() << "," << timeSweep << "\n";
//    arSalida.close();

    arSalida.open("../salida/SIMAN.csv", std::ios_base::app);
    arSalida << nombreCaso << "," << clusteringUsado << ","  << vn.size() << "," << cantPasos << "," << tempMax << "," << coefEnfriamiento << "," << solActual.getCostoSol() << "," << solSimAn.getCostoSol() << "," << costoOpt << "," << timeSimAn << "\n";
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

float convertir_para_tsp2(k_means& clusterizacion, string nombre_in, Nodo depo, float MAX_X, float MAX_Y){
    float costo_viaje = 0;
    float ctsp2=0;
    int cant_rutas=0;
    for (int i = 0; i < clusterizacion.cant_clusters(); ++i) {
        //cout <<"cant nodos del cluster "<< i<<": "<<clusterizacion.getClusterIesimo(i).getCantNodos()<<endl;
        vector<Nodo> nodos = clusterizacion.getClusterIesimo(i).getNodos();
        vector<Nodo> nodos_con_dep(nodos.size()+1); //le sumo para agregar el deposito
        nodos_con_dep[0] = depo;
        for (int j = 1; j < nodos_con_dep.size(); ++j) {
            nodos_con_dep[j] = nodos[j-1];
        }
        costo_viaje = 0;
        vector<Nodo> camino;
        if(nodos_con_dep.size()!=1) {
            cant_rutas++;
            camino = tsp_con_grasp(nodos_con_dep,0,costo_viaje,1,MAX_X, MAX_Y);
        }
        ctsp2+=costo_viaje;
        if(nodos_con_dep.size()!=1) exportar_grafo(camino,costo_viaje,i,nombre_in);
    }
    //rutas_usadas=cant_rutas;
    //cout <<"Costo total: "<<ctsp2<<endl;
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