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

//13. Check Again
template <typename T> void Deque<T>::resize(int x, T d){
    T* newArr = new T[x];
    if(x<qSize){
        backPtr=backPtr+(qSize-x);
    }
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
    arr=newArr;
    frontPtr=qSize-1;
    qSize=capacity;
    backPtr=frontPtr+1<x?frontPtr+1:0;
    cout<<"\n";
}
//12. OK
template <typename T> void Deque<T>::clear(){
    T* newArr = new T[10];
    arr=newArr;
    backPtr=-1;
    frontPtr=-1;
    capacity=10;
    qSize=0;
}
//3. OK(check).
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
//4. OK(check).
template <typename T> void Deque<T>::push_front(T x){
    if(qSize==capacity) resize();
    if(frontPtr==-1 && backPtr==-1){
        frontPtr=backPtr=0;
    }
    else frontPtr = (frontPtr+1)%capacity;
    arr[frontPtr]=x;
    qSize++;
}
//5. OK(check).
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
//6. OK(check).
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
//7. OK.
template <typename T> T Deque<T>::front(){
    if(qSize==0) return T();
    return arr[frontPtr];
}
//8. OK.
template <typename T> T Deque<T>::back(){
    if(qSize==0) return T();
    return arr[backPtr];
}
//9. OK.
template <typename T> bool Deque<T>::empty(){
    return qSize==0;
}
//10. OK.
template <typename T> int Deque<T>::size(){
    return qSize;
}
//11. [] OK.
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
 
 
    // Print the current size
    cout << "Current size " << q.size() << endl;
 
    // Print front elements of deque
    cout << "Front element " << q.front() << endl;
 
    // Print last element of the deque
    cout << "Rear element " << q.back() << endl;
 
    cout << endl;
 
    cout << "Pop an element from front" << endl;
 
    // Pop an element from the front of deque
    q.pop_front();
 
    cout << "Pop an element from back" << endl;
 
    // Pop an element from the back of deque
    q.pop_back();
 
    cout << endl;
 
    // Print current size
    cout << "Current size " << q.size() << endl;
 
    // Print front elements of deque
    cout << "Front element " << q.front() << endl;
 
    // Print last element of the deque
    cout << "Rear element " << q.back() << endl;
    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    q.resize(7, 99);
    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n";
    cout<<q.back()<<"\n";
    q.clear();
    q.push_back(1012);
    for(int i=0; i<q.size(); i++){
        cout<<q[i]<<" ";
    }
    cout<<"\n"<<"---"<<"\n";
    cout<<"-------------------------------------------------------------------------"<<"\n";

 
    // Print the current size
    cout << "Current size " << Q.size() << endl;
 
    // Print front elements of deque
    cout << "Front element " << Q.front() << endl;
 
    // Print last element of the deque
    cout << "Rear element " << Q.back() << endl;
 
    cout << endl;
 
    cout << "Pop an element from front" << endl;
 
    // Pop an element from the front of deque
    Q.pop_front();
 
    cout << "Pop an element from back" << endl;
 
    // Pop an element from the back of deque
    Q.pop_back();
 
    cout << endl;
 
    // Print current size
    cout << "Current size " << Q.size() << endl;
 
    // Print front elements of deque
    cout << "Front element " << Q.front() << endl;
 
    // Print last element of the deque
    cout << "Rear element " << Q.back() << endl;
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }

     cout<<"\n";
    Q.resize(7, 99);
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n";
    cout<<Q.back()<<"\n";
    Q.clear();
    Q.push_back(1012);
    for(int i=0; i<Q.size(); i++){
        cout<<Q[i]<<" ";
    }
    cout<<"\n"<<"---"<<"\n";

    return 0;
}



