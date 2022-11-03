#include <iostream>
#include <string.h>
#include <limits.h>
#include <vector>
#include <algorithm>

using namespace std;

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

bool compare(struct Suffix &s1, struct Suffix &s2)
{
    return (s1.rank[0] == s2.rank[0])? (s1.rank[1] < s2.rank[1] ?1: 0):
               (s1.rank[0] < s2.rank[0] ?1: 0);
}

vector<int> getSuffixArray(string s){

    int n = s.size();
    vector<struct Suffix> suffixes;

    for(int i=0; i<n; i++){
        int a = s[i];
        int b = i+1<n ? s[i+1] : s[n-(i+1)];
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
            suffixes[i].rank[1] = nextIndex<n ? suffixes[indexes[nextIndex]].rank[0] : suffixes[indexes[nextIndex-n]].rank[0];
        }

        sort(suffixes.begin(), suffixes.end(), compare);
    }

    vector<int> result;
    for(struct Suffix resSuf : suffixes){
        result.push_back(resSuf.index);
    }

    return result;

}

int main(){
    string s;
    cin>>s;

    vector<int> suffixArray = getSuffixArray(s);
    int index = suffixArray[0];
    string str= s+s;
    int n = s.size();

    cout<<str.substr(index, n);

    return 0;
}