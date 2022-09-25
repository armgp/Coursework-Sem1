#include <iostream>
#include  <vector>
#include <algorithm>
using namespace std;

FILE* openFile(string fileName, string mode){
    FILE* of = fopen(fileName.c_str(), mode.c_str());
    if (of == NULL) {
        perror("Cant open the file.\n");
        exit(EXIT_FAILURE);
    }
    return of;
}

void externalSort(string ip, string op){
    FILE* inputFile = openFile(ip, "r");
    vector<long> values(1000);
    vector<FILE*> modularFiles;
    int moduleNumber=1;
    while(true){
        int i=0;
        for(i=0; i<1000; i++){
            if(fscanf(inputFile, "%ld", &values[i]) != 1){
                break;
            }
        }
        sort(values.begin(), values.end());
        FILE* module = openFile(to_string(moduleNumber++), "w");
        for(int j=0; j<i; j++){
            fprintf(module, "%ld", values[j]);
        }
        modularFiles.push_back(module);
        fclose(module);
    }
}

int main(){
    externalSort("input_file.txt", "output_file.txt");
    return 0;
}