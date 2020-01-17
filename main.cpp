#include "grafos.h"

int main()
{
    try{
    grafonori G;
    G.lerGrafo("grafos.txt");
    vector<Arco>a=G.kruskal();
    printvector(a);
    cout<<endl;
    cout<<endl;
    vector<Arco>b=G.dijkstra(0,6);
    print_in_reverse(b);
    cout<<endl;
    cout<<endl;
    vector<vector<TipoId>>c=G.compconexas();
    printvectorvector(c);
    return 0;
    }
    catch(const errGrafo& e){ cout<<e.what();}
}
