#include <iostream>
using namespace std;

class maxHeap{
    long long** arr;
    int capacity=10;
    int ind=0;

    maxHeap(){}

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

    void insert(long long val, long long index){
        if(ind==capacity) resize();
        int i = ind++;
        arr[0][i]=val;
        arr[1][i]=index;

        while(i!=0){
            int parent = (i-1)/2;
            if(arr[parent] < arr[ind]){
                long long tempPV = arr[0][parent];
                long long tempPI = arr[1][parent];
                arr[0][parent]=arr[0][i];
                arr[1][parent]=arr[1][i];
                arr[0][i]=tempPV;
                arr[1][i]=tempPI;
            }else break;
            i = parent;
        }
    }

};

int main(){
    
    return 0;
}