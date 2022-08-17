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
    void push_back(T x);
    void push_front(T x);
    void pop_back();
    void pop_front();
    T front();
    T back();
    bool empty();
    int size();
    T operator[](int index);
};

template <typename T> T Deque<T>::operator[](int index){
        cout<<frontPtr<<", "<<backPtr<<" -->";
        int i;
        if(index>=qSize) {
            cout<<"Index out of Bounds, returnng the first element";
            i=frontPtr;
        }
        else{
            i = (frontPtr-index)%capacity;
            if(i<0){
                i=capacity+1;
            }
        }
        return arr[i];
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
    cout<<frontPtr<<" "<<backPtr<<"\n";
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
    return arr[frontPtr];
}
//8. OK.
template <typename T> T Deque<T>::back(){
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
//11. []


signed main(){
    fast;
     Deque<int> q;
     deque<int> Q;
    // Iterate range [1, 100],
    // push even numbers at the back
    // and push odd numbers at the front
    for (int i = 1; i < 17; i++)
        if (i % 2 == 0){
            q.push_back(i);
            Q.push_back(i);
        }
        else{
            q.push_front(i);
            Q.push_front(i);
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
    cout<<q[2]<<"\n";
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
    cout<<Q[2];
    return 0;
}



