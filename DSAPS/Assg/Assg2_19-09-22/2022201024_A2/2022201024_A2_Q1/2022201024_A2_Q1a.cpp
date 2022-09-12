#include <iostream>
#include<unordered_map>
using namespace std;

struct Node {
   int key=0;
   int value=0;
   struct Node* next=NULL;
   struct Node* prev=NULL;
};

class LRUcache {
    int capacity;
    unordered_map<int, Node*> cache;
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
        Node* curr = cache[key];
        curr->value = value;

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
        Node* newNode = new Node;
        newNode->key = key;
        newNode->value = value;
        cache[key]=newNode;
        if(head==NULL){
            head = newNode;
            tail = newNode;
        }else{
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }else if(cache.size() == capacity){
        int rmKey = tail->key;
        cache.erase(rmKey);
        tail->key = key;
        tail->value = value;
        cache[key] = tail;
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

    Node* curr = cache[key];
    
    if(curr != head){
        if(curr==tail) tail = curr->prev;
        curr->prev->next = curr->next;
        if(curr->next) curr->next->prev = curr->prev;
        curr->next = head;
        curr->prev = NULL;
        head->prev = curr;
        head = curr;
        
    }

    return curr->value;

}

int main(){
    int cap, q;
    cin>>cap>>q;
    LRUcache lc(cap);
    for(int i=0; i<q; i++){
        int a,b,c;
        cin>>a;
        if(a==1){
            cin>>b;
            cout<<lc.get(b)<<"\n";
        }else if(a==2){
            cin>>b>>c;
            lc.set(b, c);
        }
    }

    return 0;
}