#ifndef GRAFOS2_H
#define GRAFOS2_H

#endif // GRAFOS2_H


#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>
#include <sstream>
#include <list>
#include <iterator>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <limits>
#include <functional>
#include <fstream>
using namespace std;

typedef int TipoId;
typedef pair<double,TipoId>par;

class errGrafo{
    string msg;
public:
    errGrafo(const string& s):msg{s}{}
    const string & what() const { return msg;}
};
class Arco{
private:
    TipoId ori, dest;
    double peso;
public:
    bool operator<(const Arco &e) const
            {
                return peso < e.w();
            }
    bool operator>(const Arco &e) const
            {
                return peso > e.w();
            }
    Arco(const TipoId& o,const TipoId& d,double c):ori(o),dest(d),peso(c){}
    const TipoId& u() const{return ori;}
    const TipoId& v() const{return dest;}
    double w() const {return peso;}
    void setW(double p){peso=p;}
};
class Nodo{
private:
    vector<Arco>adj;
    TipoId id;
public:
    explicit Nodo (const TipoId& ident):id(ident){}
    void setId(const TipoId&ident)  {id=ident;}
    const TipoId& ident() const {return id;}
    void insereArco(const Arco& a){adj.push_back(a);}
    const vector<Arco>& adjs() const {return adj;}
    void apagaArco(const TipoId& dest){
        for(size_t i=0; i<adj.size();i++){
            if(adj[i].v()==dest){
                adj.erase(adj.begin()+i);
            }
        }
    }

};

class grafonori{
  private:
    vector<Nodo>nodos;
    map<TipoId,unsigned int>id2idx;
public:
    explicit grafonori(size_t n){
        nodos.resize(n,Nodo(0));
        for(size_t i=0;i<n;i++){
            id2idx[i+1]=i;
            nodos[i].setId(i+1);
        }
    }
    grafonori(){}
    grafonori(const string& s){
            lerGrafo(s);
    }
    void novoArco(const TipoId& ori,const TipoId& dest, double w=0){
        if(id2idx.find(ori)==id2idx.end()||id2idx.find(dest)==id2idx.end()){
            throw errGrafo{"nova aresta: nod invalido"};
        }
        nodos[id2idx[ori]].insereArco(Arco(ori,dest,w));
    }
    void novoNodo(const TipoId& ident);
    void novaAresta(const TipoId& ori, const TipoId& dest,double w=0);
    void print() const;
    void apagarNodo(const TipoId &ident);
    void apagarAresta(const TipoId& ori, const TipoId& dest);
    void lerGrafo(const string& s);
    grafonori(vector<Nodo>nodosaux,map<TipoId,unsigned int>id2idxaux){
        nodos=nodosaux;
        id2idx=id2idxaux;
    }
    vector<bool>DFS_nrv(const TipoId& inicio);
    vector<vector<TipoId>>compconexas();
    vector<Arco> kruskal();
    vector<Arco> dijkstra(const TipoId& a,const TipoId& b);
};
void printvector(const vector<Arco>& aux);
void printvectorvector(const vector<vector<TipoId>>& compnexas);
void print_in_reverse(const vector<Arco>&e);

