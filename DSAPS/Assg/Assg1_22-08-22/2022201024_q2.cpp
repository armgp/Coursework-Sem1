#include <iostream>
#include <bits/stdc++.h>
#define int int64_t
#define fast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
using namespace std;

template <typename T> class Deque{
private:
    T* arr;
    int frontPtr;
    int backPtr;
    int capacity=10;
    int qSize;

public:
    //constructor OK.
    Deque(){
        arr = new T[capacity];
        frontPtr=-1;
        backPtr=-1;
        qSize=0;
    }
    //constructor OK.
    Deque(int n, T x){
        capacity=n;
        arr = new T[capacity];
        for(int i=0; i<n; i++){
            arr[i]=x;
        }
        frontPtr=n-1;
        backPtr=0;
        qSize=n;
    }

    int capacit(){
        return capacity;
    }

    //member functions
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
            throw "Index out of bounds";
        }
        else{
            i = (frontPtr-index)%capacity;
            if(i<0){
                i=capacity+i;
            }
        }
        return arr[i];
}

signed main(){
    fast;
     Deque<int> q;
     deque<int> Q;
     cout<<q.front()<<"\n";
    
    for (int i = 1; i < 17; i++){
        if (i % 2 == 0){
            q.push_back(i);
            Q.push_back(i);
        }
        else{
            q.push_front(i);
            Q.push_front(i);
        }
    }
 
    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n \n";

    
    q.resize(7, 99);
    Q.resize(7, 99);

    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }


    cout<<"\n";
    cout<<q.back()<<"\n";
    cout<<Q.back()<<"\n \n";

    q.clear();
    Q.clear();

    q.push_back(1012);
    Q.push_back(1012);

    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n \n";

    q.resize(12, 33);
    Q.resize(12, 33);

    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n \n";

    cout<<q.front()<<" "<<Q.front()<<"\n";
    cout<<q.back()<<" "<<Q.back()<<"\n \n";

    q.push_back(19);
    Q.push_back(19);

    q.push_front(0);
    Q.push_front(0);

    cout<<q.front()<<" "<<q.back()<<"\n";
    cout<<Q.front()<<" "<<Q.back()<<"\n \n";

    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }

    q.resize(3, 0);
    Q.resize(3, 0);

    cout<<"\n \n";
    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n \n";

    cout<<"-------------------------------------------------------------------------"<<"\n";
    return 0;
}




