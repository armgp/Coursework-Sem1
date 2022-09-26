#include <iostream>
using namespace std;

bool dfsSearch(char** arr, int& r, int& c, int i,int j, string& word, int ind, int& n){

    if(ind == n){
        return true;
    }

    char ch;
    if(i>=r || j>=c || arr[i][j]=='0' || arr[i][j]!=word[ind]) return false;
    else {
        ch = arr[i][j];
        arr[i][j]='0';
    }

    bool stat = false;
    if(dfsSearch(arr, r, c, i+1, j, word, ind+1, n)){
        stat = true;
    } 
    else if(dfsSearch(arr, r, c, i, j+1, word, ind+1, n)){
        stat = true;
    } 
    else if(dfsSearch(arr, r, c, i-1, j, word, ind+1, n)){
        stat = true;
    }
    else if(dfsSearch(arr, r, c, i, j-1, word, ind+1, n)){
        stat = true;
    } 

    arr[i][j] = ch;
    return stat;
}

bool isPresentInGrid(string word, char** arr, int r, int c){
    int n = word.size();
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            if(arr[i][j] == word[0]){
                if(dfsSearch(arr, r, c, i, j, word, 0, n)) return true;
            }
        }
    }
    return false;
}

void solvePuzzle(char** arr, int r, int c, string* stArr, int n){
    int count = 0;
    string res = "";
    for(int i=0; i<n; i++){
        if(isPresentInGrid(stArr[i], arr, r, c)){
            count++;
            res+=stArr[i];
            res+="\n";
        }
    }
    cout<<count<<"\n";
    cout<<res;
}

void merge(string* stArr, int st, int mid, int ed){
    int st1 = st;
    int ed1 = mid;
    int st2 = mid+1;
    int ed2 = ed;

    int ind = 0;
    int sz = ed-st+1;
    string* newArr = new string[sz];

    int i=st1, j=st2;
    while(i<=ed1 && j<=ed2){
        if(stArr[i]<=stArr[j]){
            newArr[ind++] = stArr[i++];
        }else{
            newArr[ind++] = stArr[j++];
        }
    }

    while(i<=ed1){
        newArr[ind++] = stArr[i++];
    }

    while(j<=ed2){
        newArr[ind++] = stArr[j++];
    }

    for(int i=0, j=st; i<sz; i++, j++){
        stArr[j] = newArr[i];
    }
}

void mergeSort(string* stArr, int st, int ed){
    if(st==ed) return;
    int mid = (st+ed)/2;
    mergeSort(stArr, st, mid);
    mergeSort(stArr, mid+1, ed);
    merge(stArr, st, mid, ed);
}

int main(){
    int r, c;
    cin>>r>>c;
    char **arr;
    arr = new char*[r];
    for(int i=0; i<r; i++){
        arr[i] = new char[c];
    }

    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            cin>>arr[i][j];
        }
    }

    int x;
    cin>>x;

    string* stArr = new string[x];
    for(int i=0; i<x; i++){
        cin>>stArr[i];
    }

    mergeSort(stArr, 0, x-1);

    solvePuzzle(arr, r, c, stArr, x);

    return 0;
}