#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/* utils */
bool isPrime(int n)
{
    int s = sqrt(n);
    for (int i = 2; i <= s; i++)
        if (n % i == 0)
            return false;
    return true;
}
int getNextPrimeNumber(int n)
{
    n++;
    while (!isPrime(n))
        n++;
    return n;
}

/* Unorderedmap code */
template <typename K, typename V>
struct HashNode
{
public:
    K key = K();
    V value = V();
    int hashCode = 0;
    HashNode *left = NULL;
    HashNode *right = NULL;

    HashNode() {}
    HashNode(K _key, V _value, int _hashCode)
    {
        key = _key;
        value = _value;
        hashCode = _hashCode;
    }
};

template <typename K, typename V>
class UnorderedMap
{
public:
    vector<HashNode<K, V> *> bucket;
    int capacity;
    int currSize;

    UnorderedMap()
    {
        bucket.resize(11, NULL);
        capacity = 11;
        currSize = 0;
    }

    size_t getHash(K key)
    {
        hash<K> keyHash;
        return keyHash(key);
    }

    int getBucketNumber(size_t hashVal)
    {
        return hashVal % capacity;
    }

    void insert(K key, V value)
    {
        size_t hashVal = getHash(key);
        int bucketNumber = getBucketNumber(hashVal);
        if (bucket[bucketNumber] == NULL)
        {
            HashNode<K, V> *newNode = new HashNode<K, V>(key, value, hashVal);
            bucket[bucketNumber] = newNode;
            currSize++;
        }
        else
        {
            HashNode<K, V> *prevNode = NULL;
            HashNode<K, V> *currNode = bucket[bucketNumber];
            while (currNode != NULL)
            {
                if (currNode->key == key)
                {
                    currNode->value = value;
                    return;
                }
                prevNode = currNode;
                currNode = currNode->right;
            }
            HashNode<K, V> *newNode = new HashNode<K, V>(key, value, hashVal);
            prevNode->right = newNode;
            newNode->left = prevNode;
            currSize++;
        }

        float loadfactor = (float)currSize / (float)capacity;
        if (loadfactor > 0.7)
        {
            // capacity = getNextPrimeNumber(capacity);
            capacity = 2*capacity;
            currSize = 0;
            vector<HashNode<K, V> *> tempBucket = bucket;
            bucket.assign(capacity, NULL);
            for (HashNode<K, V> *node : tempBucket)
            {
                HashNode<K, V> *currNode = node;
                while (currNode != NULL)
                {
                    insert(currNode->key, currNode->value);
                    currNode = currNode->right;
                }
            }
        }
        
    }

    void erase(K key)
    {
        size_t hashVal = getHash(key);
        int bucketNumber = getBucketNumber(hashVal);
        if (bucket[bucketNumber] == NULL)
            return;
        else
        {
            HashNode<K, V> *prevNode = NULL;
            HashNode<K, V> *currNode = bucket[bucketNumber];
            while (currNode != NULL)
            {
                if (currNode->key == key)
                {
                    if (!prevNode && !currNode->right)
                    {
                        bucket[bucketNumber] = NULL;
                        currSize--;
                        return;
                    }
                    if (prevNode) {
                        prevNode->right = currNode->right;
                    }
                    if (currNode->right)
                    {
                        currNode->right->left = prevNode;
                        if(!prevNode) bucket[bucketNumber] = currNode->right;
                    }
                    currSize--;
                    return;
                }
                prevNode = currNode;
                currNode = currNode->right;
            }
        }
    }

    int find(K key)
    {
        size_t hashVal = getHash(key);
        int bucketNumber = getBucketNumber(hashVal);
        if (bucket[bucketNumber] == NULL)
            return 0;
        else
        {
            HashNode<K, V> *currNode = bucket[bucketNumber];
            while (currNode != NULL)
            {
                if (currNode->key == key)
                {
                    return 1;
                }
                currNode = currNode->right;
            }
            return 0;
        }
        return 0;
    }

    V operator[](K key)
    {
        size_t hashVal = getHash(key);
        int bucketNumber = getBucketNumber(hashVal);
        if (bucket[bucketNumber] == NULL)
            return V();
        else
        {
            HashNode<K, V> *currNode = bucket[bucketNumber];
            while (currNode != NULL)
            {
                if (currNode->key == key)
                {
                    return currNode->value;
                }
                currNode = currNode->right;
            }
            return V();
        }
        return V();
    }
};

int main(){
    int n, k;
    cin>>n>>k;
    vector<int> arr(n, 0);
    for(int i=0; i<n; i++){
        cin>>arr[i];
    }

    if(k>n){
        cout<<"K value cannot be greater than N value\n";
        return 0;
    }

    vector<int> res;
    UnorderedMap<int, int> map;
    int st=0, ed=k-1;
    for(int i=0; i<k; i++){
        if(map.find(arr[i]) == 1){
            map.insert(arr[i], map[arr[i]]+1);
        }else{
            map.insert(arr[i], 1);
        }
    }

    res.push_back(map.currSize);

    while(ed!=n-1){
        if(map[arr[st]] < 2){
            map.erase(arr[st]);
        }else{
            map.insert(arr[st], map[arr[st]]-1);
        }
        
        st++;
        ed++;
        if(map.find(arr[ed]) == 1){
            map.insert(arr[ed], map[arr[ed]]+1);
        }else{
            map.insert(arr[ed], 1);
        }
        res.push_back(map.currSize);
    }

    for(int i : res){
        cout<<i<<" ";
    }

    return 0;
}