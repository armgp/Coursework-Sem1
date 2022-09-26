#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(){
    vector<long> res(10000);
    for(int i=0; i<10000; i++){
        cin>>res[i];
    }
    sort(res.begin(), res.end());
    for(long i : res){
        cout<<i<<" ";
    }
}