#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(int argc, char *argv[]){
    vector<long> res(40);
    for(int i=0; i<40; i++){
        cin>>res[i];
    }
    sort(res.begin(), res.end());
    for(long i : res){
        cout<<i<<" ";
    }
    // string a = argv[0];
    // string b = argv[1];
    // cout<<a<<" -- "<<b<<"\n";
    return 0;
}