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
        arr = newArr;
        for(int i = 0; i < 3; i++)
            delete[] newArr[i];
        delete[] newArr;
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

    T* operator+(arrSpMatrix<T> mt){
        arrSpMatrix res(n, m);
        
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
    int ind=0;
    for(int i=0; i<c.n; i++){
        for(int j=0; j<c.m; j++){
            if(c[ind][0]==i && c[ind][1]==j) out<<c[ind++][2]<<" ";
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

    // 1 0 0 3
    // 3 2 4 0
    // 0 5 9 5
    // 1 0 0 1
    // 0 0 6 1
    cout<<mt1<<"\n"<<mt2;
    // mt1.printArray();
}

template<typename T> void arrSpTranspose(){

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