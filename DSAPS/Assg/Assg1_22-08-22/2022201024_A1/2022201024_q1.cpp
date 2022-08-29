#include <iostream>
#define int int64_t
using namespace std;

string cleanStringBigInt(string a){
    int aLength=a.size();
    int st=aLength-1;
    for(int ind=0; ind<aLength; ind++){
        if(a[ind]!='0'){
            st=ind;
            break;
        }
    }

    return a.substr(st, aLength-st);
}

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

bool bigIntEqualTo(string a, string b){
    if(a.size()!=b.size()) return false;
    int n=a.size();
    for(int i=0; i<n; i++){
        if(a[i]!=b[i]) return false;
    }
    return true;
}

string bigIntAdd(string a, string b){
    a=cleanStringBigInt(a);
    b=cleanStringBigInt(b);
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

string bigIntSubtract(string a, string b){
    if(b=="0") return a;
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

    return cleanStringBigInt(res);
}

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

string bigIntModulo(string a, string b){
    if(bigIntLesserThan(a, b)) return a;
    else if(bigIntEqualTo(a, b) || b=="1") return "0";
    else if(b=="0"){
        throw "cannot be divided by 0";
    }

    int bSize=b.size();
    while(bigIntGreaterThan(a, b)){
        string val = a.substr(0, bSize);
        int end=bSize-1;
        if(bigIntLesserThan(val, b)){
            val = a.substr(0, bSize+1);
            end=bSize;
        } 
        while(!bigIntGreaterThan(b, val)){
            val = bigIntSubtract(val, b);
        }
        a=val+a.substr(end+1);
        a=cleanStringBigInt(a);
    }
    a=cleanStringBigInt(a);
    return a;
}

string addSubMul(string eq){
    int n = eq.size();
    string numbers[n];

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

    int numbersLength = idx;
    for(int i=0; i<numbersLength; i++){
        if(numbers[i]=="x"){
            string mulRes = bigIntMultiply(numbers[i-1], numbers[i+1]);
            numbers[i-1] = "#";
            numbers[i]="#";
            numbers[i+1]=mulRes;
            i++;
        }
    }

    int k=0;
    string resNumbers[numbersLength];
    for(int i=0; i<numbersLength; i++){
        if(numbers[i]!="#") resNumbers[k++]=numbers[i];
    }

    int j=0;
    string finalResults[3];
    for(int i=0; i<k; i++){
        if(resNumbers[i]=="+" || resNumbers[i]=="-"){
            string opRes = (resNumbers[i]=="+" ? bigIntAdd(resNumbers[i-1], resNumbers[i+1]) : bigIntSubtract(resNumbers[i-1], resNumbers[i+1]));
            finalResults[j-1] = opRes;
            i++;
            resNumbers[i]=opRes;
        }else{
            finalResults[j++]=resNumbers[i];
        }
    }
    return finalResults[0];

}

string exp(string x, int n){
    if(n==0) return "1";
    else if(n==1) return x;

    string res="1";
    while(n>0){
        if(n%2==1){
            res = bigIntMultiply(res, x);
        }
        x=bigIntMultiply(x, x);
        n/=2;
    }
    return res;
}

string bigGcd(string a, string b){
    if(a=="0" && b=="0") cout<<"GCD(0, 0) is undefined";
    string r="0";
    while(!bigIntEqualTo(b, "0")){
        r=bigIntModulo(a, b);
        a=b;
        b=r;
    }
    return a;
}

string fact(string n){
    if(n=="0" || n=="1") return "1";
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
    int operation;
    cin>>operation;
    while(1){
        if(operation==1){
            string str;
            cin>>str;
            cout<<addSubMul(str)<<"\n";
        }else if(operation==2){
            string x;
            int n;
            cin>>x>>n;
            x=cleanStringBigInt(x);
            cout<<exp(x, n)<<"\n";
        }else if(operation==3){
            string a, b;
            cin>>a>>b;
            a=cleanStringBigInt(a);
            b=cleanStringBigInt(b);
            cout<<bigGcd(a, b)<<"\n";
        }else if(operation==4){
            string str;
            cin>>str;
            cout<<fact(str)<<"\n";
        }
        else{
            cout<<"This Operation doesnt exist.. Exiting!"<<"\n";
            break;
        }
        cin>>operation;
    }
    
    return 0;
}