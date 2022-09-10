#include <iostream>
#include<unordered_map>
#include <set>
using namespace std;

struct Node {
   int data;
   int calls=0;
   struct Node* next;
   struct Node* prev;
};

class LFUcache {
    int capacity;
    unordered_map<int, int> cache;
    Node* head=NULL;
    Node* tail=NULL;

public:
    LFUcache(int c);
    void set(int key, int value);
    int get(int key);
};

LFUcache::LFUcache(int c){
    capacity = c;
}

void LFUcache::set(int key, int value){
    if(cache.find(key)!=cache.end()){
        cache[key]=value;
        Node* curr = head;
        while(curr!=NULL){
            if(curr->data == key){
                break;
            }
            curr = curr->next;
        }
  
        curr->calls++;
        Node* displacedNode = curr;
        
        if(curr != head){
            curr = curr->prev;
            if(curr->calls > displacedNode->calls) return;
            while(curr!=NULL && curr->calls<=displacedNode->calls){
                curr = curr->prev;
            }

            displacedNode->prev->next = displacedNode->next;
            if(displacedNode->next) displacedNode->next->prev = displacedNode->prev;
            if(curr == NULL){
                displacedNode->next = head;
                displacedNode->prev = NULL;
                head->prev = displacedNode;
                head = displacedNode;
            }else{
                displacedNode->next = curr->next;
                curr->next->prev=displacedNode;
                curr->next = displacedNode;
                displacedNode->prev = curr;
            }
        }

    }else if(cache.size()<capacity){
        cache[key]=value;
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->data = key;
        newNode->calls++;
        if(head==NULL){
            head = newNode;
            tail = newNode;
        }else{
            Node* curr = tail;
            while(curr!=NULL && curr->calls==newNode->calls){
                curr = curr->prev;
            }

            if(curr==NULL){
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }else{
                newNode->next = curr->next;
                newNode->prev = curr;
                curr->next = newNode;
                if(newNode->next) newNode->next->prev = newNode;
                else tail = newNode;
            }
        }
    }else if(cache.size() == capacity){
        int rmKey = tail->data;
        cache.erase(rmKey);
        cache[key] = value;
        tail->data = key;
        tail->calls = 1;

        Node* newNode = tail;
        Node* curr = tail->prev;
        while(curr!=NULL && curr->calls==newNode->calls){
            curr = curr->prev;
        }

        if(curr==NULL){
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }else{
            if(curr->next == newNode) return;
            newNode->next = curr->next;
            newNode->prev = curr;
            curr->next = newNode;
            if(newNode->next) newNode->next->prev = newNode;
            else tail = newNode;
        }
    }
}

int LFUcache::get(int key){
    if(cache.find(key)==cache.end()) return -1;

    Node* curr = head;
    while(curr!=NULL){
        if(curr->data == key){
            break;
        }
        curr = curr->next;
    }
    curr->calls++;
    Node* displacedNode = curr;
        
    if(curr != head){
        curr = curr->prev;
        if(curr->calls > displacedNode->calls) return cache[key];
        while(curr!=NULL && curr->calls<=displacedNode->calls){
            curr = curr->prev;
        }
        displacedNode->prev->next = displacedNode->next;
        if(displacedNode->next) displacedNode->next->prev = displacedNode->prev;
        if(curr == NULL){
            displacedNode->next = head;
            displacedNode->prev = NULL;
            head->prev = displacedNode;
            head = displacedNode;
        }else{
            displacedNode->next = curr->next;
            curr->next->prev=displacedNode;
            curr->next = displacedNode;
            displacedNode->prev = curr;
        }
    }

    return cache[key];

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