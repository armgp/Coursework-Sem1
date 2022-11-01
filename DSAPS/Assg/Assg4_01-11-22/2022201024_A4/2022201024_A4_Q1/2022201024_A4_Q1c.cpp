#include <iostream>
#include <string.h>
#include <limits.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Node{
    int val;
    Node* prev;
    Node* next;
    Node(){
        val = 0;
        prev = NULL;
        next = NULL;
    }

    Node(int _val){
        val = _val;
        prev = NULL;
        next = NULL;
    }
};

class Dequeue{
public:
    Node* Front;
    Node* Back;
    int size;
    
    Dequeue(){
        Front = NULL;
        Back = NULL;
        size = 0;
    }

    bool empty(){
        return size==0;
    }

    void pop_front(){
        Front = Front->next;
        if(Front) Front->prev = NULL;
        size--;
    }

    void pop_back(){
        Back = Back->prev;
        if(Back) Back->next = NULL;
        size--;
    }

    void push_back(int v){
        Node* newNode = new Node(v);
        if(size == 0){
            Front = newNode;
            Back = newNode;
        }else{
            Back->next = newNode;
            newNode->prev = Back;
            Back = newNode;
        }
        size++;
    }

    int front(){
        return Front->val;
    }

    int back(){
        return Back->val;
    }
};

struct Suffix{
    int index; 
    vector<int> rank; 
    Suffix(int i, int a, int b){
        index = i;
        rank.resize(2);
        rank[0]=a;
        rank[1]=b;
    }
};

bool compare(struct Suffix s1, struct Suffix s2){
    if(s1.rank[0]<s2.rank[0]) return true;
    else if(s1.rank[0]>s2.rank[0]) return false;
    else if(s1.rank[1]<s2.rank[1]) return true;
    else if(s1.rank[1]>s2.rank[1]) return false;
    return true;
}

vector<int> getSuffixArray(string s){

    int n = s.size();
    vector<struct Suffix> suffixes;

    for(int i=0; i<n; i++){
        int a = s[i]-'a';
        int b = i+1<n ? s[i+1]-'a' : -1;
        struct Suffix suf(i, a, b);
        suffixes.push_back(suf);
    }

    sort(suffixes.begin(), suffixes.end(), compare);
    vector<int> indexes(n);

    for(int j=4; j<n; j*=2){
        int rank = 0;
        vector<int> prevRank = {suffixes[0].rank[0], suffixes[0].rank[1]};
        suffixes[0].rank[0] = rank;
        indexes[suffixes[0].index] = 0;

        for(int i=1; i<n; i++){
            vector<int> currRank = {suffixes[i].rank[0], suffixes[i].rank[1]};
            if(currRank == prevRank){
                suffixes[i].rank[0] = rank;
            }else suffixes[i].rank[0] = ++rank;
            prevRank = currRank;
            indexes[suffixes[i].index] = i;
        }

        for(int i=0; i<n; i++){
            int nextIndex = suffixes[i].index+j/2;
            suffixes[i].rank[1] = nextIndex<n ? suffixes[indexes[nextIndex]].rank[0] : -1;
        }

        sort(suffixes.begin(), suffixes.end(), compare);
    }

    vector<int> result;
    for(struct Suffix resSuf : suffixes){
        result.push_back(resSuf.index);
    }

    return result;

}

vector<int> getLCPFast(vector<int> SA, string str){
    int n = SA.size();
    vector<int> LCP(n);
    vector<int> rank(n);
    int h=0;
    for(int i=0; i<n; i++){
        rank[SA[i]] = i;
    }

    for(int i=0; i<n; i++){
        if(rank[i]>0){
            int k = SA[rank[i]-1];
            while(i+h<n && k+h<n && str[i+h] == str[k+h]) h++;
            LCP[rank[i]] = h;
            if(h>0) h--;
        }
    }

    return LCP;
}

vector<int> getLCPSlow(vector<int> SA, string str){
    int n = SA.size();
    vector<int> LCP(n, 0);
    for(int i=1; i<n; i++){
        int st=0;
        string s1 = str.substr(SA[i]);
        string s2 = str.substr(SA[i-1]);
        while(s1[st]==s2[st]) st++;
        LCP[i] = st;
    }
    return LCP;
}

int main(){
    string s;
    cin>>s;
    int n = s.size();
    string str = s+"#";
    for(int i=n-1; i>=0; i--){
        str+=s[i];
    }

    vector<int> suffixArray = getSuffixArray(str);
    vector<int> lcpArray = getLCPFast(suffixArray, str);
    int maxLen = 0;
    for(int i=0; i<2*n; i++){
        if((suffixArray[i]<n && suffixArray[i+1]>n) || (suffixArray[i]>n && suffixArray[i+1]<n)){
            if(str[suffixArray[i]] == str[suffixArray[i]+lcpArray[i+1]-1])
                maxLen = max(maxLen,lcpArray[i+1]);
        }
    }

    cout<<str<<"\n";

    for(int i : suffixArray) cout<<str.substr(i)<<"  "<<i<<"\n";
    cout<<"\n";

    for(int i : lcpArray) cout<<i<<" ";
    cout<<"\n";

    cout<<maxLen;

    return 0;
}
