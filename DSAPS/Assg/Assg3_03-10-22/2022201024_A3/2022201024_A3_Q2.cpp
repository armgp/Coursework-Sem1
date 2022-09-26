#include <iostream>
#include  <vector>
#include <algorithm>
using namespace std;

#define LOC "../ex-sort-test/"

FILE* openFile(string fileName, string mode){
    FILE* of = fopen(fileName.c_str(), mode.c_str());
    if (of == NULL) {
        perror("Cant open the file.\n");
        exit(EXIT_FAILURE);
    }
    return of;
}

string mergeFiles(string o1, string o2){
    FILE* file1 = openFile(o1, "r");
    FILE* file2 = openFile(o2, "r");
    
}

string mergeSortFiles(vector<string> modularFiles, int st, int ed){
    if(st==ed) return modularFiles[st];
    int mid = (st+ed)/2;
    string out1 = mergeSortFiles(modularFiles, st, mid);
    string out2 = mergeSortFiles(modularFiles, mid+1, ed);
    return mergeFiles(out1, out2);
}

void externalSort(string ip, string op){
    FILE* inputFile = openFile(ip, "r");
    vector<long> values(10000);
    vector<string> modularFiles;
    int moduleNumber=1;
    bool stat = true;
    while(stat){
        int i=0;
        for(i=0; i<10000; i++){
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
    }

    for(string s : modularFiles){
        cout<<s<<"\n";
    }

    int n = modularFiles.size();
    mergeSortFiles(modularFiles, 0, n);
}

int main(){
    externalSort("../ex-sort-test/input.txt", "../ex-sort-test/output_file.txt");
    return 0;
}