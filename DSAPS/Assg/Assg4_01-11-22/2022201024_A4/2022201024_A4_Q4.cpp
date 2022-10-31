#include <iostream>
#include <vector>

using namespace std;

class UnionFind{
public:
    vector<int> root;
    vector<int> rank;
    vector<int> rootMembers;
    int maxUnionSize;
    int numberOfSets;

    UnionFind(int n){
        root.resize(n+1);
        rank.resize(n+1);
        rootMembers.resize(n+1);
        for(int i=0; i<=n; i++){
            root[i] = i;
            rank[i] = 1;
            rootMembers[i] = 1;
        }
        maxUnionSize = 1;
        numberOfSets = n;
    }

    int getRoot(int u){
        if(root[u] == u) return u;
        return root[u] = getRoot(root[u]);
    }

    void unionSet(int a, int b){
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
            numberOfSets--;
            int _root = getRoot(a);
            rootMembers[_root]++;
            if(rootMembers[_root]>maxUnionSize) maxUnionSize = rootMembers[_root];
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
        cout<<uf.numberOfSets<<" "<<uf.maxUnionSize<<"\n";
    }
    return 0;
}