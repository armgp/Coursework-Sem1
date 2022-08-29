#include <iostream>
using namespace std;

template <typename T> class Deque{
private:
    T* arr;
    int frontPtr;
    int backPtr;
    int capacity=10;
    int qSize;

public:

    int capacit(){
        return capacity;
    }

    Deque(){
        arr = new T[capacity];
        frontPtr=-1;
        backPtr=-1;
        qSize=0;
    }

    //member functions
    void deque();
    void deque(int n, T x);
    void resize();
    void resize(int x, T d);
    void push_back(T x);
    void push_front(T x);
    void pop_back();
    void pop_front();
    T front();
    T back();
    bool empty();
    int size();
    T& operator[](int index);
    void clear();
};

template <typename T> void Deque<T>::deque(){
        arr = new T[capacity];
        frontPtr=-1;
        backPtr=-1;
        qSize=0;
}

template <typename T> void Deque<T>::deque(int n, T x){
        capacity=n;
        arr = new T[capacity];
        for(int i=0; i<n; i++){
            arr[i]=x;
        }
        frontPtr=n-1;
        backPtr=0;
        qSize=n;
}

template <typename T> void Deque<T>::resize(){
    int newCapacity = (3*capacity)/2;
    T* newArr =  new T[newCapacity];
    for(int i=backPtr, j=0; ;i++, j++){
        int index = i%capacity;
        newArr[j] = arr[index];
        if(index==frontPtr){
            backPtr=0;
            frontPtr=j;
            capacity=newCapacity;
            arr = newArr;
            break;
        }
    }
}

template <typename T> void Deque<T>::resize(int x, T d){
    if(qSize==0){
        deque(x, d);
        return;
    }

    T* newArr = new T[x];
    if(x<qSize){
        backPtr=backPtr+(qSize-x);
    }
    if(backPtr>capacity) backPtr=qSize-x-1;

    int i=backPtr;
    int count=0;
    for(int k=0; k<x; k++){
        if(i==capacity) i=0;
        if(count<qSize){
            newArr[k]=arr[i++];
            count++;
        }else{
            newArr[k]=d;
        }
    }

    capacity=x;
    if(x<=qSize)qSize=x;
    arr=newArr;
    frontPtr=qSize-1;
    if(x>=qSize)qSize=x;
    backPtr=frontPtr+1<x?frontPtr+1:0;
}

template <typename T> void Deque<T>::clear(){
    T* newArr = new T[10];
    arr=newArr;
    backPtr=-1;
    frontPtr=-1;
    capacity=10;
    qSize=0;
}

template <typename T> void Deque<T>::push_back(T x){
    if(qSize==capacity) resize();
    if(backPtr==0) backPtr=capacity-1;
    else if(frontPtr==-1 && backPtr==-1){
        frontPtr=backPtr=0;
    }
    else backPtr--;
    arr[backPtr]=x;
    qSize++;
}

template <typename T> void Deque<T>::push_front(T x){
    if(qSize==capacity) resize();
    if(frontPtr==-1 && backPtr==-1){
        frontPtr=backPtr=0;
    }
    else frontPtr = (frontPtr+1)%capacity;
    arr[frontPtr]=x;
    qSize++;
}

template <typename T> void Deque<T>::pop_back(){
    if(qSize==0) throw "deque empty";
    if(frontPtr==backPtr){
        frontPtr=-1;
        backPtr=-1;
        qSize=0;
    }else{
        backPtr=(backPtr+1)%capacity;
        qSize--;
    }
}

template <typename T> void Deque<T>::pop_front(){
    if(qSize==0) throw "deque empty";
    if(frontPtr==backPtr){
        frontPtr=-1;
        backPtr=-1;
        qSize=0;
    }else{
        frontPtr--;
        qSize--;
    }
}

template <typename T> T Deque<T>::front(){
    if(qSize==0) return T();
    return arr[frontPtr];
}

template <typename T> T Deque<T>::back(){
    if(qSize==0) return T();
    return arr[backPtr];
}

template <typename T> bool Deque<T>::empty(){
    return qSize==0;
}

template <typename T> int Deque<T>::size(){
    return qSize;
}

template <typename T> T& Deque<T>::operator[](int index){
        int i;
        if(index>=qSize) {
            cout<<"Index out of bounds \n";
            return arr[0];
        }
        else{
            i = (frontPtr-index)%capacity;
            if(i<0){
                i=capacity+i;
            }
        }
        return arr[i];
}

int main(){
    cout<<"Enter the operation number: \n";
    int n;
    cin>>n;
    
    //template -> int
    Deque<int> q;
    int val;

    while(1){
        if(n==1){
            q.deque();
        }else if(n==2){
            cout<<"Enter the size and default value: \n";
            int N;
            cin>>N>>val;
            q.deque(N, val);
        }else if(n==3){
            cout<<"Enter value to push back: \n";
            cin>>val;
            q.push_back(val);
        }else if(n==4){
            q.pop_back();
        }else if(n==5){
            cout<<"Enter value to push front: \n";
            cin>>val;
            q.push_front(val);
        }else if(n==6){
            q.pop_front();
        }else if(n==7){
            cout<<q.front()<<"\n";
        }else if(n==8){
            cout<<q.back()<<"\n";
        }else if(n==9){
            cout<<"Enter a valid index(lesser than queue size): \n";
            int k;
            cin>>k;
            cout<<q[k]<<"\n";
        }else if(n==10){
            cout<<q.empty()<<"\n";
        }else if(n==11){
            cout<<q.size()<<"\n";
        }else if(n==12){
            cout<<"Enter the size and default value: \n";
            int k;
            cin>>k>>val;
            q.resize(k, val);
        }else if(n==13){
            q.clear();
        }else break;
        cin>>n;
    }

    return 0;
}




