#include <iostream>
#include <stdio.h>
#include  <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>
#include <fstream>
// #include <chrono>
using namespace std;

#define TMPSIZE 10000

string LOC = "../ex-sort-test/";
long nTempFiles=0;

class minHeap{
    vector<vector<long>> arr;

public:
    minHeap(){}

    bool isEmpty(){
        return arr.size()==0;
    }
    
    void insert(long val, long index){
        arr.push_back({val, index});
        long i = arr.size()-1;
        while(i!=0){
            long parent = (i-1)/2;
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

    void heapify(long i){
        long ind = arr.size();
        long leftChildInd = 2*i+1;
        long rightChildInd = 2*i+2;
        long minEleInd=i;
        if(leftChildInd<ind && arr[leftChildInd][0]<arr[minEleInd][0]){
            minEleInd=leftChildInd;
        }
        if(rightChildInd<ind && arr[rightChildInd][0]<arr[minEleInd][0]){
            minEleInd=rightChildInd;
        }
        if(minEleInd!=i){
            long tempV = arr[minEleInd][0];
            long tempI = arr[minEleInd][1];
            arr[minEleInd][0]=arr[i][0];
            arr[minEleInd][1]=arr[i][1];
            arr[i][0]=tempV;
            arr[i][1]=tempI;
            heapify(minEleInd);
        }
    }

    vector<long> poll(){
        long ind = arr.size();
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
    ofstream outfile;
    outfile.open(op);

    long n = modularFiles.size();
    vector<ifstream> openIpFiles(n);
    for(long i=0; i<n; i++){
        openIpFiles[i].open(modularFiles[i]);
    }
 
    minHeap h;
    for(long i=0; i<n; i++){
        long val;
        // ifstream file = openIpFiles[i];
        // if(!feof(file)){
        //     fscanf (file, "%ld", &val);  
        //     h.insert(val, i);
        // }
        if(openIpFiles[i]>>val){
            h.insert(val, i);
        }
    }

    while(!h.isEmpty()){
        vector<long> smallestEleDet = h.poll();
        long smallestEle = smallestEleDet[0];
        long smallestEleInd = smallestEleDet[1];
        // fprintf(outfile, "%ld ", smallestEle);
        outfile<<smallestEle<<" ";
        long val;
        if(openIpFiles[smallestEleInd]>>val){
            h.insert(val, smallestEleInd);
        }
    }

    for(long i=0; i<n; i++){
        openIpFiles[i].close();
        // remove(modularFiles[i].c_str());
        if(remove(modularFiles[i].c_str()) != 0 ) perror( "Error deleting file" );
    }
}

void externalSort(string ip, string op){
    ifstream inputFile;
    inputFile.open(ip);
    vector<string> modularFiles;
    int moduleNumber=1;
    bool stat = true;
    while(stat){
        int i=0;
        vector<long> values(TMPSIZE, 0);
        for(i=0; i<TMPSIZE; i++){
            if(!(inputFile>>values[i])){
                stat = false;
                break;
            }
        }

        if(!stat) break;

        sort(values.begin(), values.end());

        ofstream module;
        module.open(LOC+to_string(moduleNumber));

        for(int j=0; j<i; j++){
            module<<values[j]<<" ";
        }
        modularFiles.push_back(LOC+to_string(moduleNumber++));
        
        module.close();
    }
    inputFile.close();
    nTempFiles = modularFiles.size();
    sortFiles(modularFiles, op);
}

int main(int argc, char *argv[]){
    clock_t st, ed;
    st = clock();
    ios_base::sync_with_stdio(false);
    // externalSort("../ex-sort-test/input.txt", "../ex-sort-test/output_file.txt");
    string inf = argv[1];
    string otf = argv[2];
   
    LOC="";
    for(int i=otf.size()-1; i>=0; i--){
        if(otf[i]=='/'){
            LOC = otf.substr(0, i+1);
            break;
        }
    }
    externalSort(argv[1], argv[2]);

    ed = clock();
    double time = double(ed-st) / double(CLOCKS_PER_SEC);

    cout<<"Number of integers in a temporary file: "<<TMPSIZE<<"\n";
    cout<<"Number of temporary files created: "<<nTempFiles<<"\n";
    cout <<"Total time taken by the program: "<<fixed<<setprecision(2)<<time<<"\n";
    return 0;
}

