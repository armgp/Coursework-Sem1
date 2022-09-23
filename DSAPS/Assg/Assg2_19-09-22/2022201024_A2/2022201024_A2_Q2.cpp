#include <iostream>
using namespace std;

class maxHeap{
    long long** arr;
    int capacity;
    int ind=0;

public:
    maxHeap(int c){
        capacity = c;
        arr = new long long*[2];
        for(int i=0; i<2; i++){
            arr[i] = new long long[capacity];
        }
    }

    bool isEmpty(){
        return ind==0;
    }

    void resize(){
        int newCapacity = (3*capacity)/2;
        long long** newArr = new long long*[2];
        for(int i=0; i<2; i++){
            newArr[i] = new long long[newCapacity];
        }

        for(int i=0; i<2; i++){
            for(int j=0; j<capacity; j++){
                newArr[i][j]=arr[i][j];
            }
        }
        capacity = newCapacity;
        arr = newArr;
    }
    
    void insert(long long val, int index){
        if(ind==capacity){
            cout<<"Heap size is full";
            return;
        }
        int i = ind++;
        arr[0][i]=val;
        arr[1][i]=index;

        while(i!=0){
            int parent = (i-1)/2;
            if(arr[0][parent] < arr[0][i]){
                long long tempPV = arr[0][parent];
                long long tempPI = arr[1][parent];
                arr[0][parent]=arr[0][i];
                arr[1][parent]=arr[1][i];
                arr[0][i]=tempPV;
                arr[1][i]=tempPI;
            }
            else break;
            i = parent;
        }
    }

    void heapify(int i){
        int leftChildInd = 2*i+1;
        int rightChildInd = 2*i+2;
        int maxEleInd=i;
        if(leftChildInd<ind && arr[0][leftChildInd]>arr[0][maxEleInd]){
            maxEleInd=leftChildInd;
        }
        if(rightChildInd<ind && arr[0][rightChildInd]>arr[0][maxEleInd]){
            maxEleInd=rightChildInd;
        }
        if(maxEleInd!=i){
            long long tempV = arr[0][maxEleInd];
            long long tempI = arr[1][maxEleInd];
            arr[0][maxEleInd]=arr[0][i];
            arr[1][maxEleInd]=arr[1][i];
            arr[0][i]=tempV;
            arr[1][i]=tempI;
            heapify(maxEleInd);
        }
    }

    long long* poll(){
        if(ind==0){
            cout<<"Heap empty";
            return NULL;
        }

        long long* res = new long long[2];
        long long a, b;
        a=arr[0][0];
        b=arr[1][0];
        res[0]=a;
        res[1]=b;
        if(ind==1){
            ind--;
        }else{
            long long tempV = arr[0][0];
            long long tempI = arr[1][0];
            arr[0][0]=arr[0][ind-1];
            arr[1][0]=arr[1][ind-1];
            arr[0][ind-1]=tempV;
            arr[1][ind-1]=tempI;
            ind--;
            heapify(0);
        }

        return res;
    }

    /* debug */
    void printArray(){
        for(int i=0; i<ind; i++){
            cout<<arr[0][i]<<" "<<arr[1][i]<<"\n";
        }
    }

};

int main(){
    int n, k;
    cin>>n>>k;
    long long val=0;
    maxHeap hp(n);
    long long maxSum = 0;
    for(int i=0; i<n; i++){
        cin>>val;
        if(val>0) maxSum+=val;
        else val = abs(val);
        hp.insert(val, 0);
    }

    int i=n-1;
    long long sortedVals[n];
    while(!hp.isEmpty()){
        long long* a = hp.poll();
        sortedVals[i--]=a[0];
    }

    maxHeap h(k+1);
    long long res[k+1];
    i=0;
    res[i++] = maxSum;

    h.insert(maxSum-sortedVals[0], 0);
    while(i<k){
        long long* a = h.poll();
        long long sum;
        if(a!=NULL) sum = a[0];
        else {
            cout<<"k has exceeded the maximum limit \n";
            break;
        }
        
        int ind = a[1];
        res[i++]=sum;
        if(ind+1<n) {
            h.insert(sum+sortedVals[ind]-sortedVals[ind+1], ind+1);
            h.insert(sum-sortedVals[ind+1], ind+1);
        }
    }

    for(int i=0; i<k; i++){
        cout<<res[i]<<" ";
    }
    

    return 0;
}