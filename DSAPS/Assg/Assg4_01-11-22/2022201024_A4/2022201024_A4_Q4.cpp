#include <iostream>
#include <vector>

using namespace std;

class UnionFind{
    vector<int> root;
    vector<int> rank;
public:
    UnionFind(int n){
        root.resize(n+1);
        rank.resize(n+1);
        for(int i=0; i<=n; i++){
            root[i] = i;
            rank[i] = 1;
        }
    }

    int getRoot(int u){
        if(root[u] == u) return u;
        return root[u] = getRoot(root[u]);
    }

    int unionSet(int a, int b){
        int rootA = getRoot(a);
        int rootB = getRoot(b);
        if(rootA!=rootB){
            if(rank[rootA]<rank[rootB]){
                root[rootA] = rootB;
            }else if(rank[rootA]>rank[rootB]){
                root[rootB] = rootA;
            }else{
                root[rootA] = rootB;
                rank[rootB] += 1;
            }
        }
    }

    bool isConnected(int a, int b){
        return getRoot(a) == getRoot(b);
    }
};

int main(){
    int n,m;
    cin>>n>>m;
    UnionFind uf(n);
    for(int i=0; i<m; i++){
        int a, b;
        cin>>a>>b;
        uf.unionSet(a, b);
    }
    return 0;
}