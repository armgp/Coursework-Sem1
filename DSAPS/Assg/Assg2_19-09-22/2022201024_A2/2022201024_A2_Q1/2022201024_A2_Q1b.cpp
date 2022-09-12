#include <iostream>
#include<unordered_map>
#include <unordered_set>
using namespace std;

struct Node {
   int key=0;
   int value=0;
   int calls=0;
   struct Node* next=NULL;
   struct Node* prev=NULL;
};

class LFUcache {
    int capacity;
    int minFrequency=1;
    unordered_map<int, Node*> cache;
    unordered_map<int, Node*> frequencyMapHead;
    unordered_map<int, Node*> frequencyMapTail;

public:
    LFUcache(int c);
    void set(int key, int value);
    int get(int key);
};

LFUcache::LFUcache(int c){
    capacity = c;
}

void LFUcache::set(int key, int value) {
    if(cache.find(key)!=cache.end()){
        Node* node = cache[key];
        node->value = value;
        if(node->next==NULL && node->prev==NULL){
            frequencyMapHead.erase(node->calls);
            frequencyMapTail.erase(node->calls);
        }else if(node->next==NULL){
            frequencyMapTail[node->calls] = frequencyMapTail[node->calls]->prev;
            node->prev->next = NULL;
            node->prev = NULL;
        }else if(node->prev==NULL){
            frequencyMapHead[node->calls] = frequencyMapHead[node->calls]->next;
            node->next->prev = NULL;
            node->next = NULL;
        }else{
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next=NULL;
            node->prev=NULL;
        }
        node->calls++;
        if(frequencyMapHead.find(node->calls)!=frequencyMapHead.end()){
            Node* head = frequencyMapHead[node->calls];
            node->next = head;
            head->prev = node;
            frequencyMapHead[node->calls]=node;
        }else{
            frequencyMapHead[node->calls]=node;
            frequencyMapTail[node->calls]=node;
        }
        if(frequencyMapHead.find(minFrequency)==frequencyMapHead.end()) minFrequency++;
        }else if(cache.size()<capacity){
        Node* newNode = new Node;
        newNode->key = key;
        newNode->value = value;
        newNode->calls = 1;

        if(frequencyMapHead.find(1)!=frequencyMapHead.end()){
            Node* head = frequencyMapHead[1];
            newNode->next = head;
            head->prev = newNode;
            frequencyMapHead[1]=newNode;
        }else{
            frequencyMapHead[1]=newNode;
            frequencyMapTail[1]=newNode;
        }
        minFrequency = 1;
        cache[key] = newNode;
    }else if(cache.size()==capacity){
        if(capacity==0) return;
        Node* rmNode = frequencyMapTail[minFrequency];
        cache.erase(rmNode->key);
        if(rmNode->next==NULL && rmNode->prev==NULL){
            frequencyMapHead.erase(minFrequency);
            frequencyMapTail.erase(minFrequency);
        }else if(rmNode->next==NULL){
            frequencyMapTail[rmNode->calls] = frequencyMapTail[rmNode->calls]->prev;
            rmNode->prev->next = NULL;
            rmNode->prev = NULL;
        }else if(rmNode->prev==NULL){
            frequencyMapHead[rmNode->calls] = frequencyMapHead[rmNode->calls]->next;
            rmNode->next->prev = NULL;
            rmNode->next = NULL;
        }else{
            rmNode->prev->next = rmNode->next;
            rmNode->next->prev = rmNode->prev;
            rmNode->next=NULL;
            rmNode->prev=NULL;
        }
        Node* newNode = rmNode;
        newNode->key = key;
        newNode->value = value;
        newNode->calls = 1;
        if(frequencyMapHead.find(1)!=frequencyMapHead.end()){
            Node* head = frequencyMapHead[1];
            newNode->next = head;
            head->prev = newNode;
            frequencyMapHead[1]=newNode;
        }else{
            frequencyMapHead[1]=newNode;
            frequencyMapTail[1]=newNode;
        }
        minFrequency = 1;
        cache[key] = newNode;
    }
}

int LFUcache::get(int key) {
    if(cache.find(key)==cache.end()) return -1;
    Node* node = cache[key];
    if(node->next==NULL && node->prev==NULL){
        frequencyMapHead.erase(node->calls);
        frequencyMapTail.erase(node->calls);
    }else if(node->next==NULL){
        frequencyMapTail[node->calls] = frequencyMapTail[node->calls]->prev;
        node->prev->next = NULL;
        node->prev = NULL;
    }else if(node->prev==NULL){
        frequencyMapHead[node->calls] = frequencyMapHead[node->calls]->next;
        node->next->prev = NULL;
        node->next = NULL;
    }else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next=NULL;
        node->prev=NULL;
    }
    node->calls++;
    if(frequencyMapHead.find(node->calls)!=frequencyMapHead.end()){
        Node* head = frequencyMapHead[node->calls];
        node->next = head;
        head->prev = node;
        frequencyMapHead[node->calls]=node;
    }else{
        frequencyMapHead[node->calls]=node;
        frequencyMapTail[node->calls]=node;
    }
    if(frequencyMapHead.find(minFrequency)==frequencyMapHead.end()) minFrequency++;
    return node->value;
}

int main(){
    int cap, q;
    cin>>cap>>q;
    LFUcache lc(cap);
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