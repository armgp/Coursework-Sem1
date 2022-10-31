#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

class MinHeap{
    vector<vector<int>> arr;

public:
    MinHeap(){}

    bool isEmpty(){
        return arr.size()==0;
    }
    
    void insert(int w, int u){
        arr.push_back({w, u});
        int i = arr.size()-1;
        while(i!=0){
            int parent = (i-1)/2;
            if(arr[parent][0] > arr[i][0]){
                vector<int> temp = arr[parent];
                arr[parent] = arr[i];
                arr[i] = temp;
            }
            else break;
            i = parent;
        }
    }

    void heapify(int i){
        int ind = arr.size();
        int leftChildInd = 2*i+1;
        int rightChildInd = 2*i+2;
        int minEleInd=i;
        if(leftChildInd<ind && arr[leftChildInd][0]<arr[minEleInd][0]){
            minEleInd=leftChildInd;
        }
        if(rightChildInd<ind && arr[rightChildInd][0]<arr[minEleInd][0]){
            minEleInd=rightChildInd;
        }
        if(minEleInd!=i){
            vector<int> temp = arr[minEleInd];
            arr[minEleInd] = arr[i];
            arr[i] = temp;
            heapify(minEleInd);
        }
    }

    vector<int> poll(){
        if(isEmpty()){
            cout<<"Heap empty";
            return {};
        }

        vector<int> res = arr[0];
        int n = arr.size();

        if(n==1){
            arr.pop_back();
        }else{
            arr[0] = arr[n-1];
            arr.pop_back();
            heapify(0);
        }

        return res;
    }

    /* debug */
    void printArray(){
        for(int i=0; i<arr.size(); i++){
            cout<<arr[i][0]<<" "<<arr[i][1]<<"\n";
        }
    }

};

int main(){
    int n, m, k;
    cin>>n>>m>>k;
    vector<vector<vector<int>>> adjList(n+1);
    for(int i=0; i<m; i++){
        int u,v,w;
        cin>>u>>v>>w;
        adjList[u].push_back({w, v});
        adjList[v].push_back({w, u});
    }
    vector<int> policeStations(k, 0);
    vector<int> minDist(n+1, INT_MAX);
    for(int i=0; i<k; i++) {
        cin>>policeStations[i];
        minDist[policeStations[i]] = 0;
    }

    MinHeap h;
    for(int p : policeStations) h.insert(0, p);

    while(!h.isEmpty()){
        vector<int> e = h.poll();
        int w = e[0];
        int u = e[1];
        int currDist = minDist[u];
        for(vector<int> edge : adjList[u]){
            int _w = edge[0];
            int _u = edge[1];
            int dist = currDist+_w;
            if(minDist[_u]>dist) {
                minDist[_u] = dist;
                h.insert(dist, _u);
            }
        }
    }

    for(int i=1; i<=n; i++){
        minDist[i]==INT_MAX? cout<<-1<<" " : cout<<minDist[i]<<" ";
    }

    return 0;
}