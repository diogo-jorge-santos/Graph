#include "grafos.h"

using namespace std;

vector<Arco> grafonori::kruskal()
{
    vector<vector<TipoId>> conexas=compconexas();
    if(conexas.size()!=1){
        throw errGrafo{"Nao e possivel estabelecer uma arvore que ligue todos os nodos se existem nodos em componentes diferentes\n"};
    }
    vector<pair<double,Arco>> edgesAndWeights;

    for(unsigned int i=0; i<nodos.size(); i++){
        for(const auto& arco:nodos[i].adjs()){
            if(id2idx.at(arco.v()) > i){
                edgesAndWeights.push_back(make_pair(arco.w(),arco));
            }
        }
    }
    sort(edgesAndWeights.begin(),edgesAndWeights.end());

    vector<Arco>edgesInTheTree;
    vector<unsigned int> nodesInTheTree (nodos.size(), 0);
    unsigned int currentComponent = 0;

        for (size_t i=0; i<edgesAndWeights.size(); i++)
        {
            if (edgesInTheTree.size() == (nodos.size() - 1)) break;

            // If neither node is in the forest, then a new connected component must be initiated.
            if (nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] == 0 &&
                    nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] == 0)
            {
                edgesInTheTree.push_back(edgesAndWeights[i].second);

                currentComponent++;
                nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] = currentComponent;
                nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] = currentComponent;
            }

            // If one node is in the forest but not the other one,
            // then that edge is added to the tree where that node is.
            else if (nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] != 0 &&
                   nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] == 0)
            {
                edgesInTheTree.push_back(edgesAndWeights[i].second);
                nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] = nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())];
            }

            else if (nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] == 0 &&
                   nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] != 0)
            {
                edgesInTheTree.push_back(edgesAndWeights[i].second);
                nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] = nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())];
            }

            // If both nodes are in different connected components, then it gets tricky:
            // the edge must be added, but with the two components merging into one.
            else if (nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] != 0 &&
                   nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())] != 0 &&
                   nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())] != nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())])
            {
                edgesInTheTree.push_back(edgesAndWeights[i].second);

                for (size_t j=0; j<nodesInTheTree.size(); j++)
                {
                    if (nodesInTheTree[j] == nodesInTheTree[id2idx.at(edgesAndWeights[i].second.v())])
                        nodesInTheTree[j] = nodesInTheTree[id2idx.at(edgesAndWeights[i].second.u())];
                }
            }
        }
    return edgesInTheTree;
}
vector<Arco> grafonori::dijkstra(const TipoId& a,const TipoId& b){
    vector<vector<TipoId>> conexas=compconexas();
    size_t n1 = 0,n2 = 0;
    for(size_t i=0;i<conexas.size();i++){
        for(size_t j=0;j<conexas[i].size();j++){
        if(conexas[i][j]==a){
            n1=i;
        }
        if(conexas[i][j]==b){
            n2=i;
        }
      }
    }
    if(n1!=n2){throw errGrafo{"Nao possivel ligar grafos em componentes diferentes\n"};}
    priority_queue<par,vector<par>,greater<par>>pq;
    vector<double>dist(nodos.size(),numeric_limits<double>::infinity());
    vector<TipoId>pred(nodos.size(),-1);
    dist[id2idx.at(a)]=0.0;
    pq.push(make_pair(0.0,a));
    while(!pq.empty()){
        par u=pq.top();
        if(id2idx.at(u.second)==id2idx.at(b))break; //para melhorar a eficiencia
        pq.pop();
        for(const auto& arco:nodos[id2idx.at(u.second)].adjs()){
            if(dist[id2idx.at(arco.v())]>dist[id2idx.at(arco.u())]+arco.w()){
                dist[id2idx.at(arco.v())]=dist[id2idx.at(arco.u())]+arco.w();
                pred[id2idx.at(arco.v())]=arco.u();
                pq.push(make_pair(dist[id2idx.at(arco.v())],arco.v()));
            }
        }
    }
    vector<Arco> dijk;
    TipoId currentIndex = b;

        while (id2idx.at(currentIndex) != id2idx.at(a))
        {
            dijk.push_back(Arco(pred[id2idx.at(currentIndex)],currentIndex,
                           dist[id2idx.at(currentIndex)]-dist[id2idx.at(pred[id2idx.at(currentIndex)])]));
            currentIndex = pred[id2idx.at(currentIndex)];
        }

        return dijk;
}

void grafonori::lerGrafo(const string& s){//s=localização do ficheiro
      ifstream ler(s);
      vector<string>graph;
      string aux;
      int n_nodos=0;
      size_t n_arestas = 0;
      if(!ler) cerr<<"nao le\n";
      while(1){
          getline(ler,aux);
          graph.push_back(aux);
          if(ler.eof()) break;
      }
      ler.close();

      for(size_t i=0;i<graph.size()-1;i++){
          //linha 1 e 2 ler o numero de nodos e o de arestas
          char a1,a2,a3,a4;
          TipoId a,b;
          double c;
          istringstream ist(graph[i]);
          if(i==0){
              ist>>n_nodos;
              for(auto j=0;j<n_nodos;j++){
                  novoNodo(j);
              }
          }
          if(i==1) ist>>n_arestas;
          if(i>=2 && i<=n_arestas+2){
              ist>>a1>>a>>a2>>b>>a3>>c>>a4;
              novaAresta(a,b,c);
          }
      }
}

vector<bool> grafonori::DFS_nrv(const TipoId& inicio){
    stack<unsigned int> a;
    vector<bool> visitados(nodos.size(),false);
    if(id2idx.find(inicio)==id2idx.end()){
        throw errGrafo{"DFS: inicio invalido."};}
    a.push(id2idx.at(inicio));
    while(!a.empty()){
        unsigned int u=a.top();
        a.pop();
        if(!visitados[u]){
        for(const auto& arco:nodos[u].adjs()){
            a.push(id2idx.at(arco.v()));
        }
        visitados[u]=true;
    }



}
    return visitados;
}

vector<vector<TipoId>> grafonori::compconexas(){
    vector<vector<TipoId>> compnexas;
    vector<bool> javisitado(nodos.size(),false);
    size_t auxx=0;
    while(auxx!=nodos.size()){
        size_t aux = 0;
        vector<TipoId> comp;
        //vector<TipoId> nodosapagar;
        for(size_t i=0;i<nodos.size();i++){
            if(javisitado[i]==false)
                aux=i;
        }
        vector<bool> isin = DFS_nrv(nodos[aux].ident());
        for(size_t i=0; i< isin.size();i++){
            if(isin[i]==true){
              auxx++;
              comp.push_back(nodos[i].ident());
            }
        }
        for(size_t i=0; i< isin.size();i++){
            if(isin[i]==true){
                javisitado[i]=true;
            }
        }

        compnexas.push_back(comp);

    }
    return(compnexas);


}

void printvector(const vector<Arco>& aux){
    for(size_t i=0;i<aux.size();i++){
        cout<<"("<< aux[i].u()<<","<<aux[i].v() <<",peso="<< aux[i].w()<<")"<< endl;
    }
    double peso=0;
    for(size_t i=0;i<aux.size();i++){
        peso=peso+aux[i].w();
    }
    cout<<"peso arvore: "<< peso <<endl;
   }
void printvectorvector(const vector<vector<TipoId>>& compnexas){
    for(size_t i=0;i<compnexas.size();i++){
        for(size_t j=0;j<compnexas[i].size();j++){
            cout<<compnexas[i][j]<< " ";
        }
        cout<< endl;
    }
}
void print_in_reverse(const vector<Arco>&e){
    for(size_t i=0;i<e.size();i++){
        cout<<"("<<e[e.size()-i-1].u()<<","<<e[e.size()-i-1].v()<<","<<e[e.size()-i-1].w()<<")\n";
    }
    double peso=0;
    for(size_t i=0;i<e.size();i++){
        peso=peso+e[i].w();
    }
    cout<<"peso arvore: "<< peso <<endl;
}

void grafonori::novoNodo(const TipoId& ident){
    for(size_t i=0;i<nodos.size();i++){
        if(nodos[i].ident()==ident){
            throw errGrafo{"nodo ja existe"};
        }
    }
    nodos.push_back(Nodo(ident));
    id2idx[ident]=nodos.size()-1;
}
void grafonori::novaAresta(const TipoId& ori, const TipoId& dest,double w){
    for(const auto& arco:nodos[id2idx.at(ori)].adjs()){
        if((arco.v()==dest && arco.u()==ori)||(arco.v()==ori && arco.u()==dest)){ throw errGrafo{"novaAresta: a aresta ja existe."};}
    }
    novoArco(ori,dest,w);
    novoArco(dest,ori,w);
}
void grafonori::print() const{
    for(size_t i=0;i<nodos.size();i++){
        for(const auto& it:nodos[i].adjs()){
            if(it.u()<it.v()){
                cout<<"("<<it.u()<<","<<it.v()<<","<<"peso="<<it.w()<<")"<< endl;
            }
        }
    }
}
void grafonori::apagarNodo(const TipoId& ident){
    for(size_t i=0;i<nodos.size();i++){
        if(nodos[i].ident()==ident){
            nodos.erase(nodos.begin()+i);
        }
        else
            for(size_t j=0;j<nodos[i].adjs().size();j++){
                if(nodos[i].adjs()[j].v()==ident){
                    nodos[i].apagaArco(ident);
                }
        }
    }
   id2idx.erase(ident);
}
void grafonori::apagarAresta(const TipoId& ori, const TipoId& dest){
    for(size_t i=0;i<nodos.size();i++){
        if(ori==nodos[i].ident()){
            nodos[i].apagaArco(dest);
        }
    }
}




