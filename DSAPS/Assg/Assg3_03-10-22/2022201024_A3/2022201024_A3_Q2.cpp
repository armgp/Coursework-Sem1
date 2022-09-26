#include <iostream>
#include <stdio.h>
#include  <vector>
#include <algorithm>
using namespace std;

#define LOC "../ex-sort-test/"
#define TMPSIZE 10


class minHeap{
    vector<vector<long>> arr;

public:
    minHeap(){}

    bool isEmpty(){
        return arr.size()==0;
    }
    
    void insert(long val, long index){
        arr.push_back({val, index});
        int i = arr.size()-1;
        while(i!=0){
            int parent = (i-1)/2;
            if(arr[parent][0] > arr[i][0]){
                long tempPV = arr[parent][0];
                long tempPI = arr[parent][1];
                arr[parent][0]=arr[i][0];
                arr[parent][1]=arr[i][1];
                arr[i][0]=tempPV;
                arr[i][1]=tempPI;
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
            long long tempV = arr[minEleInd][0];
            long long tempI = arr[minEleInd][1];
            arr[minEleInd][0]=arr[i][0];
            arr[minEleInd][1]=arr[i][1];
            arr[i][0]=tempV;
            arr[i][1]=tempI;
            heapify(minEleInd);
        }
    }

    vector<long> poll(){
        int ind = arr.size();
        if(ind==0){
            return {};
        }

        vector<long> res = {arr[0][0], arr[0][1]};
        arr[0][0]=arr[ind-1][0];
        arr[0][1]=arr[ind-1][1];
        arr.pop_back();
        heapify(0);

        return res;
    }

};




FILE* openFile(string fileName, string mode){
    FILE* of = fopen(fileName.c_str(), mode.c_str());
    if (of == NULL) {
        perror("Cant open the file.\n");
        exit(EXIT_FAILURE);
    }
    return of;
}

void sortFiles(vector<string> modularFiles, string op){
    FILE* outfile = openFile(op, "w");

    int n = modularFiles.size();
    vector<FILE*> openIpFiles(n);
    for(int i=0; i<n; i++){
        openIpFiles[i] = openFile(modularFiles[i], "r");
    }
 
    minHeap h;
    for(int i=0; i<n; i++){
        long val;
        FILE* file = openIpFiles[i];
        if(!feof(file)){
            fscanf (file, "%ld", &val);  
            h.insert(val, i);
        }
    }

    while(!h.isEmpty()){
        vector<long> smallestEleDet = h.poll();
        long smallestEle = smallestEleDet[0];
        long smallestEleInd = smallestEleDet[1];
        fprintf(outfile, "%ld ", smallestEle);
        long val;
        if(fscanf(openIpFiles[smallestEleInd], "%ld", &val)==1){
            h.insert(val, smallestEleInd);
        }
    }

    for(int i=0; i<n; i++){
        fclose(openIpFiles[i]);
        remove(modularFiles[i].c_str());
    }
}

void externalSort(string ip, string op){
    FILE* inputFile = openFile(ip, "r");
    vector<long> values(TMPSIZE);
    vector<string> modularFiles;
    int moduleNumber=1;
    bool stat = true;
    while(stat){
        int i=0;
        for(i=0; i<TMPSIZE; i++){
            if(fscanf(inputFile, "%ld", &values[i]) != 1){
                stat = false;
                break;
            }
        }

        if(!stat) break;

        sort(values.begin(), values.end());
        FILE* module = openFile(LOC+to_string(moduleNumber), "w");
        for(int j=0; j<i; j++){
            fprintf(module, "%ld ", values[j]);
        }
        modularFiles.push_back(LOC+to_string(moduleNumber++));
        fclose(module);
    }

    sortFiles(modularFiles, op);
}

int main(){
    externalSort("../ex-sort-test/input.txt", "../ex-sort-test/output_file.txt");
    return 0;
}