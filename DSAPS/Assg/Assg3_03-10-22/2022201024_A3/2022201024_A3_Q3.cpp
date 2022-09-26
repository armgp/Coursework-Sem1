#include <iostream>
using namespace std;

struct Node{
    struct Node *charachters[26];
    bool isEnd;
    bool found;
    string currWord;
    Node(){
        isEnd = false;
        found = false;
        currWord ="";
        for(int i=0; i<26; i++) charachters[i]=NULL;
    }
};

class Trie {
private:
    Node* root;
public:
    Trie(){
        root = new Node();
    }

    void insertWord(string word){
        Node* curr = root;
        string w = "";
        for(char c : word){
            w+=c;
            if(curr->charachters[c-'a'] == NULL){
                Node* newNode = new Node();
                newNode->currWord+=w;
                curr->charachters[c-'a'] = newNode;
            }
            curr = curr->charachters[c-'a'];
        }
        curr->isEnd = true;
    }

    bool doesWordStartWith(char ch){
        return !(root->charachters[ch-'a']==NULL);
    }

    Node* getRoot(){
        return this->root;
    }
};

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

void dfsSearch(char** arr, int& r, int& c, int i, int j, Node* node, string* stArr, int& ind){

    char ch = arr[i][j];
    if(!node) return;
    
    if(node->isEnd && !node->found) {
        stArr[ind++]=node->currWord;
        node->found = true;
    }

    arr[i][j] = '0';
    
    if(i+1<r && arr[i+1][j]!='0') dfsSearch(arr, r, c, i+1, j, node->charachters[arr[i+1][j]-'a'], stArr, ind);
    if(j+1<c && arr[i][j+1]!='0') dfsSearch(arr, r, c, i, j+1, node->charachters[arr[i][j+1]-'a'], stArr, ind);
    if(i-1>=0 && arr[i-1][j]!='0') dfsSearch(arr, r, c, i-1, j, node->charachters[arr[i-1][j]-'a'], stArr, ind);
    if(j-1>=0 && arr[i][j-1]!='0') dfsSearch(arr, r, c, i, j-1, node->charachters[arr[i][j-1]-'a'], stArr, ind);
    
    arr[i][j] = ch;
}

void solvePuzzle(char** arr, int r, int c, Trie trie, int n){
    int ind = 0;
    string* stArr = new string[n];
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            char ch = arr[i][j];
            if(trie.doesWordStartWith(ch)){
                Node* root = trie.getRoot();
                dfsSearch(arr, r, c, i, j, root->charachters[ch-'a'], stArr, ind);
            }
        }
    }

    if(ind>1) mergeSort(stArr, 0, ind-1);

    cout<<ind<<"\n";
    for(int i=0; i<ind; i++){
        cout<<stArr[i]<<"\n";
    }
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

    Trie trie;

    string* stArr = new string[x];
    for(int i=0; i<x; i++){
        cin>>stArr[i];
        trie.insertWord(stArr[i]);
    }

    solvePuzzle(arr, r, c, trie, x);

    return 0;
}