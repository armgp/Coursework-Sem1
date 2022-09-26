#include <iostream>
#include <stdio.h>
#include  <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>
// #include <chrono>
using namespace std;

#define TMPSIZE 1000

string LOC = "../ex-sort-test/";
int nTempFiles=0;

class minHeap{
    vector<vector<int>> arr;

public:
    minHeap(){}

    bool isEmpty(){
        return arr.size()==0;
    }
    
    void insert(int val, int index){
        arr.push_back({val, index});
        int i = arr.size()-1;
        while(i!=0){
            int parent = (i-1)/2;
            if(arr[parent][0] > arr[i][0]){
                int tempPV = arr[parent][0];
                int tempPI = arr[parent][1];
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
            int tempV = arr[minEleInd][0];
            int tempI = arr[minEleInd][1];
            arr[minEleInd][0]=arr[i][0];
            arr[minEleInd][1]=arr[i][1];
            arr[i][0]=tempV;
            arr[i][1]=tempI;
            heapify(minEleInd);
        }
    }

    vector<int> poll(){
        int ind = arr.size();
        if(ind==0){
            return {};
        }

        vector<int> res = {arr[0][0], arr[0][1]};
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
        int val;
        FILE* file = openIpFiles[i];
        if(!feof(file)){
            fscanf (file, "%d", &val);  
            h.insert(val, i);
        }
    }

    while(!h.isEmpty()){
        vector<int> smallestEleDet = h.poll();
        int smallestEle = smallestEleDet[0];
        int smallestEleInd = smallestEleDet[1];
        fprintf(outfile, "%d ", smallestEle);
        int val;
        if(fscanf(openIpFiles[smallestEleInd], "%d", &val)==1){
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
    vector<string> modularFiles;
    int moduleNumber=1;
    bool stat = true;
    while(stat){
        int i=0;
        vector<int> values(TMPSIZE, 0);
        for(i=0; i<TMPSIZE; i++){
            if(fscanf(inputFile, "%d", &values[i]) != 1){
                stat = false;
                break;
            }
        }

        if(!stat) break;

        sort(values.begin(), values.end());
        FILE* module = openFile(LOC+to_string(moduleNumber), "w");
        for(int j=0; j<i; j++){
            fprintf(module, "%d ", values[j]);
        }
        modularFiles.push_back(LOC+to_string(moduleNumber++));
        fclose(module);
    }
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

