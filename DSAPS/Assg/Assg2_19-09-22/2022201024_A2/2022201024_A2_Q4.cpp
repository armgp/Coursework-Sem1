#include <iostream>
using namespace std;

template<typename T> class arrSpMatrix{
public:
    T** arr;
    int n=0;
    int m=0;
    //no of columns
    int capacity = 10;
    int ind=0;

    arrSpMatrix(int N, int M){
        n = N;
        m = M;
        arr = new T*[3];
        for(int i=0; i<3; i++){
            arr[i] = new T[capacity];
        }
        ind=0;
    }

    void resize(){
        int newCapacity = (3*capacity)/2;
        T** newArr = new T*[3];
        for(int i=0; i<3; i++){
            newArr[i] = new T[newCapacity];
        }

        for(int i=0; i<3; i++){
            for(int j=0; j<capacity; j++){
                newArr[i][j]=arr[i][j];
            }
        }
        
        for(int i = 0; i < 3; i++)
            delete[] arr[i];
        delete[] arr;
        capacity = newCapacity;
        arr = newArr;
    }

    void push_back(T row, T col, T val){
        if(ind==capacity) resize();
        arr[0][ind]=row;
        arr[1][ind]=col;
        arr[2][ind]=val;
        ind++;
    }

    T* operator[](int index){
        if(index>=capacity){
            throw "index out of bounds";
        }
        T* data = new T[3];
        data[0]=arr[0][index];
        data[1]=arr[1][index];
        data[2]=arr[2][index];
        return data;
    }

    arrSpMatrix<T> operator+(arrSpMatrix<T> mt){
        arrSpMatrix res(n, m);
        int i=0, j=0;
        int r=0, c=0, v=0;
        while(i<ind && j<mt.ind){
            if(arr[0][i]<mt.arr[0][j]){
                r=arr[0][i];
                c=arr[1][i];
                v=arr[2][i];
                i++;
            }else if(arr[0][i]>mt.arr[0][j]){
                r=mt.arr[0][j];
                c=mt.arr[1][j];
                v=mt.arr[2][j];
                j++;
            }else{
                if(arr[1][i]<mt.arr[1][j]){
                    r=arr[0][i];
                    c=arr[1][i];
                    v=arr[2][i];
                    i++;
                }else if(arr[1][i]>mt.arr[1][j]){
                    r=mt.arr[0][j];
                    c=mt.arr[1][j];
                    v=mt.arr[2][j];
                    j++;
                }else{
                    r=arr[0][i];
                    c=arr[1][i];
                    v=arr[2][i]+mt.arr[2][j];
                    i++;
                    j++;
                }
            }
            res.push_back(r, c, v);
        }

        while(i<ind){
            res.push_back(arr[0][i], arr[1][i], arr[2][i]);
            i++;
        }

        while(j<mt.ind){
            res.push_back(mt.arr[0][j], mt.arr[1][j], mt.arr[2][j]);
            j++;
        }

        return res;
    }

    arrSpMatrix<T> transpose(){
        arrSpMatrix<T> tm(m, n);
        tm.arr = arr;
        int *countArr = new T[m];
        for(int i=0; i<m; i++) countArr[i]=0;
        for(int i=0; i<ind; i++){
            countArr[arr[1][i]]++;
        }
        int* indexArr = new T[m+1];
        for(int i=1; i<m; i++){
            
        }
        return tm;
    }

    /* debug */
    void printArray(){
        for(int i=0; i<3; i++){
            for(int j=0; j<capacity; j++){
                cout<<arr[i][j]<<" ";
            }
            cout<<"\n";
        }
    }
};

template<typename T> ostream &operator<<(ostream &out, arrSpMatrix<T> &c){
    int k=0;
    for(int i=0; i<c.n; i++){
        for(int j=0; j<c.m; j++){
            if(c[k][0]==i && c[k][1]==j) out<<c[k++][2]<<" ";
            else out<<T()<<" ";
        }
        out<<"\n";
    }

    return out;
}

template<typename T> void arrSpAddition(){
    int n1, m1;
    cin>>n1>>m1;
    arrSpMatrix<T> mt1(n1, m1);
    T val=0;
    for(int i=0; i<n1; i++){
        for(int j=0; j<m1; j++){
            cin>>val;
            if(val!=0){
                mt1.push_back(i, j, val);
            }
        }
    }

    int n2, m2;
    cin>>n2>>m2;
    arrSpMatrix<T> mt2(n2, m2);
    val=0;
    for(int i=0; i<n2; i++){
        for(int j=0; j<m2; j++){
            cin>>val;
            if(val!=0){
                mt2.push_back(i, j, val);
            }
        }
    }
    arrSpMatrix<T> mt = mt1+mt2;
    cout<<mt1<<"+ \n"<<mt2<<"= \n"<<mt;
}

template<typename T> void arrSpTranspose(){
    int n, m;
    cin>>n>>m;
    arrSpMatrix<T> mt(n, m);
    T val=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cin>>val;
            if(val!=0){
                mt.push_back(i, j, val);
            }
        }
    }

    mt.printArray();
    
    cout<<"\n";
    arrSpMatrix<T> tm = mt.transpose();
    tm.printArray();
    cout<<tm;
}

template<typename T> void arrSpMultiplication(){

}

template<typename T> void switchArrSpOps(int tyOp){
    switch(tyOp){
        case 1:
            arrSpAddition<T>();
            break;
        case 2:
            arrSpTranspose<T>();
            break;
        case 3:
            arrSpMultiplication<T>();
            break;
        default:
            cout<<"Invalid Operation \n";
            break;
    }
}

int main(){
    int tyDs, tyOp;
    cin>>tyDs>>tyOp;

    //array
    if(tyDs==1){
        switchArrSpOps<int>(tyOp);
    }
    //linkedlist
    else{
        switch(tyOp){
            case 1:
                //additionn
                break;
            case 2:
                //transpose
                break;
            case 3:
                //multiplication
                break;
            default:
                cout<<"Invalid Operation \n";
                break;
        }
    }

    return 0;
}