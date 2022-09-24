#include <iostream>
#include <algorithm>
using namespace std;


/* Arr-Sparse-Matrix */
template<typename T> void arrMerge(T** arr, int st1, int ed1, int st2, int ed2){
    int size = ed2-st1+1;
    T** newarr = new T*[3];
    for(int i=0; i<3; i++){
        newarr[i] = new T[size];
    }

    int k=0;
    int i=st1;
    int j=st2;
    while(i<=ed1 && j<=ed2){
        if(arr[0][i]<arr[0][j]){
            newarr[0][k]=arr[0][i];
            newarr[1][k]=arr[1][i];
            newarr[2][k]=arr[2][i];
            k++;
            i++;
        }else if(arr[0][i]>arr[0][j]){
            newarr[0][k]=arr[0][j];
            newarr[1][k]=arr[1][j];
            newarr[2][k]=arr[2][j];
            k++;
            j++;
        }else if(arr[1][i]<arr[1][j]){
            newarr[0][k]=arr[0][i];
            newarr[1][k]=arr[1][i];
            newarr[2][k++]=arr[2][i++];
        }else{
            newarr[0][k]=arr[0][j];
            newarr[1][k]=arr[1][j];
            newarr[2][k++]=arr[2][j++];
        }
    }

    while(i<=ed1){
        newarr[0][k]=arr[0][i];
        newarr[1][k]=arr[1][i];
        newarr[2][k++]=arr[2][i++];
    }

    while(j<=ed2){
        newarr[0][k]=arr[0][j];
        newarr[1][k]=arr[1][j];
        newarr[2][k++]=arr[2][j++];
    }

    for(int it=st1, jt=0; it<=ed2 && jt<size; it++, jt++){
        arr[0][it]=newarr[0][jt];
        arr[1][it]=newarr[1][jt];
        arr[2][it]=newarr[2][jt];
    }
}

template<typename T> void arrMergeSort(T** arr, int st, int ed){
    if(st>=ed) return;
    int mid = (st+ed)/2;
    arrMergeSort(arr, st, mid);
    arrMergeSort(arr, mid+1, ed);
    arrMerge(arr, st, mid, mid+1, ed);
}

template<typename T> class arrSpMatrix{
public:
    T** arr;
    int n=0;
    int m=0;
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
                int k=i;
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
        T r=0, c=0, v=0;
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
                    res.push_back(arr[0][i], b.arr[0][j], arr[2][i]*b.arr[2][j]);
                }
            }
        }

        arrMergeSort(res.arr, 0, res.ind-1);

        //add values for same row col
        for(int i=0; i<res.ind-1; i++){
            if(res.arr[0][i]==res.arr[0][i+1] && res.arr[1][i]==res.arr[1][i+1]){
                res.arr[2][i]+=res.arr[2][i+1];
                for(int j=i+1; j<res.ind-1; j++){
                    res.arr[0][j]=res.arr[0][j+1];
                    res.arr[1][j]=res.arr[1][j+1];
                    res.arr[2][j]=res.arr[2][j+1];
                }
                res.ind--;
                i--;
            }
        }
        
        return res;
    }

    arrSpMatrix<T> transpose(){
        arrSpMatrix<T> tm(m, n);
        for(int i=0; i<ind; i++){
            tm.push_back(arr[0][i], arr[1][i], arr[2][i]);
        }

        T *countArr = new T[m];
        for(int i=0; i<m; i++) countArr[i]=0;
        for(int i=0; i<ind; i++){
            countArr[int(arr[1][i])]++;
        }
        T* indexArr = new T[m+1];
        for(int i=0; i<m; i++) indexArr[i]=0;
        for(int i=1; i<m; i++){
            indexArr[i] = countArr[i-1] + indexArr[i-1];
        }

        for(int i=0; i<ind; i++){
            int j = indexArr[int(arr[1][i])]++;
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
        cout<<"\n";
    }
};

template<typename T> ostream &operator<<(ostream &out, arrSpMatrix<T> &c){
    int k=0;
    for(int i=0; i<c.n; i++){
        for(int j=0; j<c.m; j++){
            if(k<c.ind && c[k][0]==i && c[k][1]==j) out<<c[k++][2]<<"\t";
            else out<<T()<<"\t";
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
    mt.printArray();

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
    // mt.printArray();
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



/* LL-Sparse-Matrix */
template<typename T> struct Node{
    T row=-1;
    T col=-1;
    T val=-1;
    Node* next = NULL;

    Node(){};

    Node(T r, T c, T v){
        row=r;
        col=c;
        val=v;
    }
};

template<typename T> Node<T>* llMerge(Node<T>* a, Node<T>* b){
    Node<T>* d = new Node<T>();
    Node<T>* res = d;
    while(a!=NULL && b!=NULL){
        if(a->row < b->row){
            d->next = a;
            d = a;
            a = a->next;
            d->next = NULL;
        }else if(a->row > b->row){
            d->next = b;
            d = b;
            b = b->next;
            d->next = NULL;
        }else{
            if(a->col < b->col){
                d->next = a;
                d = a;
                a = a->next;
                d->next = NULL;
            }else if(a->col > b->col){
                d->next = b;
                d = b;
                b = b->next;
                d->next = NULL;
            }else {
                b->val = a->val+b->val;
                d->next = b;
                d = b;
                b = b->next;
                a = a->next;
                d->next = NULL;
            }
        }
    }

    while(a!=NULL){
        d->next = a;
        d = a;
        a = a->next;
        d->next = NULL;
    }

    while(b!=NULL){
        d->next = b;
        d = b;
        b = b->next;
        d->next = NULL;
    }

    return res->next;
}

template<typename T> Node<T>* llMergeSort(Node<T>* front, int n){
    
    if(front->next == NULL || n==1) return front;
    Node<T>* curr = front;
    int idx=1;
    while(idx<n/2){
        curr = curr->next;
        idx++;
    }
    Node<T>* c = curr->next;
    curr->next = NULL;

    Node<T>* a = llMergeSort(front, n/2);
    Node<T>* b = llMergeSort(c, n-(n/2));
    Node<T>* res = llMerge(a, b);
    return res;
}

template<typename T> class llSpMatrix{
public:
    int n;
    int m;
    int size=0;
    Node<T>* front=NULL;
    Node<T>* back=NULL;

    llSpMatrix(int N, int M){
        n=N;
        m=M;
        front = new Node<T>();
        back = front;
    }

    void push_back(T row, T col, T val){
        if(front->row == -1){
            front->row = row;
            front->col = col;
            front->val = val;
            size=1;
            return;
        }
        Node<T>* newNode = new Node<T>(row, col, val);
        size++;
        back->next = newNode;
        back = newNode;
    }

    void push_back_ow(T row, T col, T val){
        Node<T>* i = front;
        Node<T>* prev=NULL;
        while(i!=NULL){
            if(i->row==row && i->col==col){
                i->val=i->val+val;
                return;
            }

            if(row<=i->row && i->col>col){
                Node<T>* newNode = new Node<T>(row, col, val);
                if(prev==NULL){
                    newNode->next = front;
                    front = newNode;
                }else{
                    newNode->next = i;
                    prev->next = newNode;
                }
                break;
            }
            prev = i;
            i = i->next;
        }
        push_back(row, col, val);
    }

    Node<T>* operator[](int index){
        if(index>=size){
            throw "index out of bounds";
        }
        Node<T>* curr = front;
        int pos = 0;
        while(curr!=NULL){
            if(pos == index){
                return curr;
            }
            curr = curr->next;
            size++;
        }
    }

    llSpMatrix<T> operator+(llSpMatrix<T> mt){
        Node<T>* i=front;
        Node<T>* j = mt.front;
        llSpMatrix<T> res(n, m);
        T r=0, c=0, v=0;
        while(i!=NULL && j!=NULL){
            if(i->row<j->row){
                r=i->row;
                c=i->col;
                v=i->val;
                i=i->next;
            }else if(i->row>j->row){
                r=j->row;
                c=j->col;
                v=j->val;
                j=j->next;
            }else{
                if(i->col<j->col){
                    r=i->row;
                    c=i->col;
                    v=i->val;
                    i=i->next;
                }else if(i->col>j->col){
                    r=j->row;
                    c=j->col;
                    v=j->val;
                    j=j->next;
                }else{
                    r=i->row;
                    c=i->col;
                    v=i->val+j->val;
                    i=i->next;
                    j=j->next;
                }
            }
            res.push_back(r, c, v);
        }

        while(i!=NULL){
            res.push_back(i->row, i->col, i->val);
            i=i->next;
        }

        while(j!=NULL){
            res.push_back(j->row, j->col, j->val);
            j=j->next;
        }

        return res;
    }

    llSpMatrix<T> operator*(llSpMatrix<T> mt){
        if(m!=mt.n){
            cout<<"Matrices not compatible for multiplication!\n";
            return llSpMatrix(0, 0);
        }

        llSpMatrix<T> b = mt.transpose();
        llSpMatrix<T> res(n, b.n);

        Node<T>* i=front;
        while(i!=NULL){
            Node<T>* j=b.front;
            while(j!=NULL){
                if(i->col==j->col){
                    res.push_back(i->row, j->row, i->val*j->val);
                }
                j = j->next;
            }
            i = i->next;
        }

        res.front = llMergeSort(res.front, res.size);
        Node<T>* curr = res.front;
        while(curr!=NULL){
            res.back=curr;
            curr=curr->next;
        }

        return res;
    }

    llSpMatrix<T> transpose(){
        llSpMatrix<T> tm(m, n);
        Node<T>* i = front;
        while(i!=NULL){
            tm.push_back(i->col, i->row, i->val);
            i=i->next;
        }
        
        tm.front = llMergeSort(tm.front, tm.size);
        Node<T>* curr = tm.front;
        while(curr!=NULL){
            tm.back=curr;
            curr=curr->next;
        }

        return tm;
    }

    /* debug */
    void printArray(){
        Node<T>* curr = front;
        while(curr!=NULL){
            cout<<curr->row<<" "<<curr->col<<" "<<curr->val<<"\n";
            curr = curr->next;
        }
    }
};

template<typename T> ostream &operator<<(ostream &out, llSpMatrix<T> &c){
    Node<T>* k = c.front;;
    for(int i=0; i<c.n; i++){
        for(int j=0; j<c.m; j++){
            if(k!=NULL && k->row==i && k->col==j) {
                out<<k->val<<"\t";
                k=k->next;
            }
            else out<<T()<<"\t";
        }
        out<<"\n";
    }

    return out;
}

template<typename T> void llSpAddition(){
    int n1, m1;
    cin>>n1>>m1;
    llSpMatrix<T> mt1(n1, m1);
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
    if(n1!=n2 || m1!=m2){
        cout<<"Matrices cannot be added!! \n";
        return;
    }
    llSpMatrix<T> mt2(n2, m2);
    val=0;
    for(int i=0; i<n2; i++){
        for(int j=0; j<m2; j++){
            cin>>val;
            if(val!=0){
                mt2.push_back(i, j, val);
            }
        }
    }
    llSpMatrix<T> mt = mt1+mt2;
    cout<<mt1<<"+ \n"<<mt2<<"= \n"<<mt<<"\n";
}

template<typename T> T** arrMerge(T** a1, int n1, T** a2, int n2){
    
}

template<typename T> void llSpTranspose(){
    int n, m;
    cin>>n>>m;
    llSpMatrix<T> mt(m, n);
    T val=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cin>>val;
            if(val!=0){
                mt.push_back(i, j, val);
            }
        }
    }
    llSpMatrix<T> tm = mt.transpose();
    cout<<tm<<"\n";
}

template<typename T> void llSpMultiplication(){
    int n1, m1;
    cin>>n1>>m1;
    llSpMatrix<T> mt1(n1, m1);
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
    llSpMatrix<T> mt2(n2, m2);
    val=0;
    for(int i=0; i<n2; i++){
        for(int j=0; j<m2; j++){
            cin>>val;
            if(val!=0){
                mt2.push_back(i, j, val);
            }
        }
    }
    llSpMatrix<T> mt = mt1*mt2;
    
    cout<<mt1<<"* \n"<<mt2<<"= \n"<<mt<<"\n";
    cout<<"\n";
}

template<typename T> void switchLLSpOps(int tyOp){
    switch(tyOp){
        case 1:
            llSpAddition<T>();
            break;
        case 2:
            llSpTranspose<T>();
            break;
        case 3:
            llSpMultiplication<T>();
            break;
        default:
            cout<<"Invalid Operation \n";
            break;
    }
}


int main(){
    int tyDs, tyOp;
    cin>>tyDs>>tyOp;

    if(tyDs==1) switchArrSpOps<float>(tyOp);
    else switchLLSpOps<float>(tyOp);

    return 0;
}


