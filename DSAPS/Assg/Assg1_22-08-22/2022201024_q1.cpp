#include <iostream>
#define int int64_t
#define fast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
using namespace std;

//OK
bool bigIntGreaterThan(string a, string b){
    if(a.size()>b.size()) return true;
    else if(a.size()<b.size()) return false;
    else{
        int n=a.size();
        for(int i=0; i<n; i++){
            if(a[i]>b[i]) return true;
            else if(a[i]<b[i]) return false;
        }
    }
    return false;
}
//OK
bool bigIntLesserThan(string a, string b){
    if(a.size()<b.size()) return true;
    else if(a.size()>b.size()) return false;
    else{
        int n=a.size();
        for(int i=0; i<n; i++){
            if(a[i]<b[i]) return true;
            else if(a[i]>b[i]) return false;
        }
    }
    return false;
}
//OK
bool bigIntEqualTo(string a, string b){
    if(a.size()!=b.size()) return false;
    int n=a.size();
    for(int i=0; i<n; i++){
        if(a[i]!=b[i]) return false;
    }
    return true;
}

//OK
string bigIntAdd(string a, string b){
    string res="";
    int i=a.size()-1, j=b.size()-1;
    int carry=0;
    while(i>=0 && j>=0){
        int dig1 = a[i]-'0';
        int dig2 = b[j]-'0';
        int val = dig1+dig2+carry;
        if(val==0) res='0'+res;
        else{
            for(int ind=0; (ind<2 && val>0); ind++){
                int curr = val%10;
                val/=10;
                if(ind==0){
                    char dig = '0'+curr;
                    res=dig+res;
                    if(val==0){
                        carry=0;
                        break;
                    }
                } 
                else carry=curr;
            }
        }
        i--;
        j--;
    }

    while(i>=0){
        int digit = a[i]-'0';
        int val = digit+carry;
        if(val==0) res='0'+res;
        else{
            for(int ind=0; (ind<2 && val>0); ind++){
                int curr = val%10;
                val/=10;
                if(ind==0){
                    char dig = '0'+curr;
                    res=dig+res;
                    if(val==0){
                        carry=0;
                        break;
                    }
                } 
                else carry=curr;
            }
        }
        i--;
    }

    while(j>=0){
        int digit = b[j]-'0';
        int val = digit+carry;
        if(val==0) res='0'+res;
        for(int ind=0; (ind<2 && val>0); ind++){
            int curr = val%10;
            val/=10;
            if(ind==0){
                char dig = '0'+curr;
                res=dig+res;
                if(val==0){
                    carry=0;
                    break;
                }
            } 
            else carry=curr;
        }
        j--;
    }

    if(carry!=0){
        char digit = carry+'0';
        res=digit+res;
    }

    return res;
}
//OK
string bigIntSubtract(string a, string b){
    string res="";
    int i=a.size()-1, j=b.size()-1;
    int borrow=0;
    while(i>=0 && j>=0){
        int dig1 = a[i]-'0'-borrow;
        int dig2 = b[j]-'0';
        int val = dig1-dig2;
        if(val<0){
            val+=10;
            borrow=1;
        }else borrow=0;
        char digit = '0'+val;
        res=digit+res;
        i--;
        j--;
    }

    while(i>=0){
        int dig = a[i]-'0'-borrow;
        int val = dig;
        
        if(val<0){
            val+=10;
            borrow=1;
        }else borrow=0;
        char digit = '0'+val;
        res=digit+res;
        i--;
    }

    int resLength=res.size();
    int ansStart=0;
    for(int ind=0; ind<resLength; ind++){
        if(res[ind]!='0'){
            ansStart=ind;
            break;
        }
    }

    return res.substr(ansStart, resLength-ansStart);
}
//OK
string bigIntMultiply(string a, string b){
    string product="0";
    int n=a.size(), m=b.size();
    
    for(int i=n-1; i>=0; i--){
        int arr[m];
        arr[m-1]=0;
        int dig1=a[i]-'0';
        if(dig1==0) continue;
        string res="";
        int carry=0;

        for(int j=m-1, k=m-1; j>=0; j--, k--){
            char digit;
            int dig2=b[j]-'0';
            arr[k]=arr[k]+(dig1*dig2);
            if(arr[k]>=10){
                int val=arr[k]%10;
                int carr=arr[k]/10;
                digit='0'+val;
                if(k!=0) arr[k-1]=carr;
                else carry=carr;
                arr[k]=val;
            }else{
                digit = '0'+arr[k];
                if(k!=0) arr[k-1]=0;
            }
            res=digit+res;
        }
        if(carry!=0){
            char digit='0'+carry;
            res=digit+res;
        }

        for(int idx=0; idx<n-1-i; idx++){
            res=res+'0';
        }

        product = bigIntAdd(product, res);
    }
    return product;
}
//OK
string bigIntDivide(string a, string b){
    if(bigIntLesserThan(a, b) || a=="0") return "0";
    else if(bigIntEqualTo(a, b)) return "1";
    else if(b=="1") return a;
    else if(b=="0"){
        throw "cannot be divided by 0";
    }

    string res="0";
    string curr="0";
    int n=a.size();
    for(int i=0; i<n; i++){
        curr=bigIntMultiply(curr, "10");
        string currDig="";
        currDig+=a[i];
        curr=bigIntAdd(curr, currDig);
        if(bigIntGreaterThan(curr, b) || bigIntEqualTo(curr, b)){
            for(int j=9; j>=1; j--){
                char v = '0'+j;
                string J = "";
                J=J+v;
                string val = bigIntMultiply(b, J);
                if(bigIntLesserThan(val, curr) || bigIntEqualTo(val, curr)){
                    res=bigIntAdd(bigIntMultiply(res,"10"), J);
                    curr = bigIntSubtract(curr, val);
                    break;
                }
            }
        }else res=bigIntMultiply(res,"10");
    }
    return res;

}
//OK
string bigIntModulo(string a, string b){
    if(bigIntLesserThan(a, b)) return a;
    else if(bigIntEqualTo(a, b) || b=="1") return "0";
    else if(b=="0"){
        throw "cannot be divided by 0";
    }

    string curr="0";
    int n=a.size();
    for(int i=0; i<n; i++){
        curr=bigIntMultiply(curr, "10");
        string currDig="";
        currDig+=a[i];
        curr=bigIntAdd(curr, currDig);
        if(bigIntGreaterThan(curr, b) || bigIntEqualTo(curr, b)){
            for(int j=9; j>=1; j--){
                char v = '0'+j;
                string J = "";
                J=J+v;
                string val = bigIntMultiply(b, J);
                if(bigIntLesserThan(val, curr) || bigIntEqualTo(val, curr)){
                    curr = bigIntSubtract(curr, val);
                    break;
                }
            }
        };
    }
    return curr;
}

//1.OK
string addSubMul(string eq){
    int n = eq.size();
    string numbers[(n/2)+1];

    //creating an array of numbers and symbols
    int start=0;
    int idx=0;
    for(int i=0; i<=n; i++){
        if(i==n){
            string num = eq.substr(start, i-start);
            if(num!=""){
                numbers[idx++]=num;
            }
            break;
        }

        if(!isdigit(eq[i]) && eq[i]!='+' && eq[i]!='-' && eq[i]!='x') {
            return "WRONG INPUT";
        }
        else if(eq[i]=='+' || eq[i]=='-' || eq[i]=='x'){
            string num = eq.substr(start, i-start);
            if(num!=""){
                numbers[idx++]=num;
            }
            numbers[idx++]=eq[i];
            start=i+1;
        }
    }

    //using numbers array for calculation
    int numbersLength = idx;

    //first all multiplication ops are done
    int j=0;
    string multipliedResults[numbersLength];
    for(int i=0; i<numbersLength; i++){
        if(numbers[i]=="x"){
            string mulRes = bigIntMultiply(numbers[i-1], numbers[i+1]);
            multipliedResults[j-1] = mulRes;
            i++;
        }else{
            multipliedResults[j++]=numbers[i];
        }
    }
    int mulResLength = j;

    for(int i=0; i<mulResLength; i++){
        cout<<multipliedResults[i]<<"\n";
    }
    cout<<"---------------------"<<"\n"<<"\n";

    j=0;
    string finalResults[3];
    for(int i=0; i<mulResLength; i++){
        if(multipliedResults[i]=="+" || multipliedResults[i]=="-"){
            string opRes = (multipliedResults[i]=="+" ? bigIntAdd(multipliedResults[i-1], multipliedResults[i+1]) : bigIntSubtract(multipliedResults[i-1], multipliedResults[i+1]));
            finalResults[j-1] = opRes;
            i++;
            multipliedResults[i]=opRes;
        }else{
            finalResults[j++]=multipliedResults[i];
        }
    }
    return finalResults[0];
}
//2.
string exp(string x, string n){
    string res;



    return res;
}
//3.
string gcd(string a, string b){
    string res;
    return res;
}
//4.OK
string fact(string n){
    if(n=="0" || n=="1") return n;
    string k="1";
    string ans="1";
    while(k!=n){
        ans=bigIntMultiply(k, ans);
        k=bigIntAdd(k, "1");
    }
    ans=bigIntMultiply(ans, n);
    return ans;
}

signed main(){
    fast;
    
    int operation;
    cin>>operation;
    
    if(operation==1){
        string str;
        cin>>str;
        cout<<addSubMul(str);
    }else if(operation==2){
        string x, n;
        cin>>x>>n;
        cout<<exp(x, n);
    }else if(operation==3){
        string a, b;
        cin>>a>>b;
        cout<<gcd(a, b);
    }else if(operation==4){
        string str;
        cin>>str;
        cout<<fact(str);
    }else{
        cout<<"This Operation doesnt exist"<<"\n";
    }

    // cout<<bigIntModulo("", "")<<"\n";
    // cout<<bigIntMultiply("0", "10")<<"\n";

    return 0;
}