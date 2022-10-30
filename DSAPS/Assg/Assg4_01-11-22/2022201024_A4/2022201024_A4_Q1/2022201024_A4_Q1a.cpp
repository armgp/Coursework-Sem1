#include <iostream>
#include <string.h>

#include <vector>
#include <algorithm>

using namespace std;

int main(){
    string s;
    cin>>s;

    vector<pair<string, int>> suffixArr;
    int n = s.size();
    s+=s;
    for(int i=0; i<n; i++){
        suffixArr.push_back(make_pair(s.substr(i), i));
    }

    sort(suffixArr.begin(), suffixArr.end());

    // for(auto st : suffixArr) cout<<st.first<<", "<<st.second<<"\n"
    cout<<s.substr(suffixArr[0].second, n)<<"\n";
    return 0;
}