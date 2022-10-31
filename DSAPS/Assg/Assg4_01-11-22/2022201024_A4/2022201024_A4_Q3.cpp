#include <iostream>
#include <vector>

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
    for(int i=0; i<k; i++) cin>>policeStations[i];

    // MinHeap h;
    // for(vector<vector<int>> vv : adjList){
    //     for(vector<int> e : vv) h.insert(e[0], e[1]);
    // }

    // while(!h.isEmpty()){
    //     vector<int> e = h.poll();
    //     cout<<e[0]<<"-"<<e[1]<<"\n";
    // }

    return 0;
}