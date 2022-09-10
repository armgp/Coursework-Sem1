#include <iostream>
#include<unordered_map>
using namespace std;

struct Node {
   int data;
   struct Node* next;
   struct Node* prev;
};

class LRUcache {
    int capacity;
    unordered_map<int, int> cache;
    Node* head=NULL;
    Node* tail=NULL;

public:
    LRUcache(int c);
    void set(int key, int value);
    int get(int key);
};

LRUcache::LRUcache(int c){
    capacity = c;
}

void LRUcache::set(int key, int value){
    if(cache.find(key)!=cache.end()){
        cache[key]=value;
        Node* curr = head;
        while(curr!=NULL){
            if(curr->data == key){
                break;
            }
            curr = curr->next;
        }

        if(curr != head){
            curr->prev->next = curr->next;
            if(curr->next) curr->next->prev = curr->prev;
            else tail = curr->prev;
            curr->next = head;
            curr->prev = NULL;
            head->prev = curr;
            head = curr;
        }

    }else if(cache.size()<capacity){
        cache[key]=value;
        struct Node* newNode = new Node;
        newNode->data = key;
        if(head==NULL){
            head = newNode;
            tail = newNode;
        }else{
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }else if(cache.size() == capacity){
        int rmKey = tail->data;
        cache.erase(rmKey);
        cache[key] = value;
        tail->data = key;
        if(capacity > 1){
            struct Node* temp = tail->prev;
            if(tail->prev) tail->prev->next = NULL;
            tail->prev = NULL;
            tail->next = head;
            head->prev = tail;
            head = tail;
            tail = temp;
        }
    }
}

int LRUcache::get(int key){
    if(cache.find(key)==cache.end()) return -1;

    Node* curr = head;
    while(curr!=NULL){
        if(curr->data == key){
            break;
        }
        curr = curr->next;
    }
    if(curr != head){
        if(curr==tail) tail = curr->prev;
        curr->prev->next = curr->next;
        if(curr->next) curr->next->prev = curr->prev;
        curr->next = head;
        curr->prev = NULL;
        head->prev = curr;
        head = curr;
        
    }

    return cache[key];

}

    //For LRU Cache
    //["LRUCache", "set", "set", "get", "set", "get", "set", "get", "get", "get"]
    //[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
    // ● constructor(capacity): Creates the cache with given capacity.
    // ● get(key): Return the value associated with the key, if not present return -1.
    // ● set(key, value): Insert a new key value pair in the cache or update if
    //   already present. If the capacity of the cache is exceeded, then replace
    //   with the Least recently used element from the cache.

int main(){
    // int cap, q;
    // cin>>cap>>q;
    // LRUcache lc(cap);
    // for(int i=0; i<q; i++){
    //     int a,b,c;
    //     cin>>a;
    //     if(a==1){
    //         cin>>b;
    //         cout<<lc.get(b)<<"\n";
    //     }else if(a==2){
    //         cin>>b>>c;
    //         lc.set(b, c);
    //     }
    // }


    int cap, q;
    cin>>cap>>q;
    LRUcache lc(cap);
    for(int i=0; i<q; i++){
        string a;
        int b,c;
        cin>>a;
        if(a=="GET"){
            cin>>b;
            cout<<lc.get(b)<<"\n";
        }else if(a=="SET"){
            cin>>b>>c;
            lc.set(b, c);
        }
    }

    return 0;
}