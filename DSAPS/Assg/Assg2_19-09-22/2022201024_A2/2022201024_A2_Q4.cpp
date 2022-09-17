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
        
        // for(int i = 0; i < 3; i++)
        //     delete[] arr[i];
        // delete[] arr;
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

    void push_back_ow(T row, T col, T val){
        for(int i=0; i<ind; i++){
            if(arr[0][i]==row && arr[1][i]==col){
                arr[2][i]+=val;
                return;
            }

            if(row<=arr[0][i] && arr[1][i]>col){
                T k=i;
                T tempRow, tempCol, tempVal;
                while(arr[2][k]!=0 && k<ind){
                    tempRow = arr[0][k];
                    tempCol = arr[1][k];
                    tempVal = arr[2][k];
                    arr[0][k]=row;
                    arr[1][k]=col;
                    arr[2][k]=val;
                    row = tempRow;
                    col = tempCol;
                    val = tempVal;
                    k++;
                }
                break;
            }
        }
        push_back(row, col, val);
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
        arrSpMatrix<T> res(n, m);
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

    arrSpMatrix<T> operator*(arrSpMatrix<T> mt){
        if(m!=mt.n){
            cout<<"Matrices not compatible for multiplication!\n";
            return arrSpMatrix(0, 0);
        }

        arrSpMatrix<T> b = mt.transpose();
        arrSpMatrix<T> res(n, b.n);

        for(int i=0; i<ind; i++){
            for(int j=0; j<b.ind; j++){
                if(arr[1][i]==b.arr[1][j]){
                    res.push_back_ow(arr[0][i], b.arr[0][j], arr[2][i]*b.arr[2][j]);
                }
            }
        }

        return res;
    }

    arrSpMatrix<T> transpose(){
        arrSpMatrix<T> tm(m, n);
        for(int i=0; i<ind; i++){
            tm.push_back(arr[0][i], arr[1][i], arr[2][i]);
        }

        int *countArr = new T[m];
        for(int i=0; i<m; i++) countArr[i]=0;
        for(int i=0; i<ind; i++){
            countArr[arr[1][i]]++;
        }
        int* indexArr = new T[m+1];
        for(int i=0; i<m; i++) indexArr[i]=0;
        for(int i=1; i<m; i++){
            indexArr[i] = countArr[i-1] + indexArr[i-1];
        }

        for(int i=0; i<ind; i++){
            int j = indexArr[arr[1][i]]++;
            tm.arr[0][j] = arr[1][i];
            tm.arr[1][j] = arr[0][i];
            tm.arr[2][j] = arr[2][i];
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
    cout<<mt1<<"+ \n"<<mt2<<"= \n"<<mt<<"\n";
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
    arrSpMatrix<T> tm = mt.transpose();
    cout<<tm<<"\n";
}

template<typename T> void arrSpMultiplication(){
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
    arrSpMatrix<T> mt = mt1*mt2;
    cout<<mt1<<"* \n"<<mt2<<"= \n"<<mt<<"\n";
    cout<<"\n";
    mt.printArray();
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


