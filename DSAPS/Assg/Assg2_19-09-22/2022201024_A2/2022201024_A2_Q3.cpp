#include <iostream>
using namespace std;

template<typename T> struct Node {
public:
    T val;
    Node* left=NULL;
    Node* right=NULL;
    int leftNodes=0;
    int rightNodes=0;
    int height;
};

template<typename T> class AVLtree {

private:
    Node<T>* root=NULL;

public:
    AVLtree(){}
    Node<T>* getRoot();
    void insert(T e);
    void Delete(T e);
    bool search(T e);
    int count_occurence(T e);
    T lower_bound(T e);
    T upper_bound(T e);
    T closest_element(T e);
    T Kth_largest(int k);
    int count_range(T eLeft, T eRight);
};

//utils
template <typename T> void printTree(Node<T>* root){
    if(root==NULL) return;
    cout<<root->val<<"\n";
    printTree(root->left);
    printTree(root->right);
}

template <typename T> Node<T>* AVLtree<T>::getRoot(){
    return root;
}

template <typename T> Node<T>* rightRotate(Node<T>* root){
    Node<T>* x = root->left;
    Node<T>* y = x->right;
    root->left = y;
    if(y) root->leftNodes = 1 + y->leftNodes + y->rightNodes;
    else root->leftNodes=0;

    x->right = root;
    if(root) x->rightNodes = 1 + root->leftNodes + root->rightNodes;
    else x->rightNodes=0;


    int lh=-1, rh=-1;
    if(root->right) rh = root->right->height;
    if(root->left) lh = root->left->height;
    root->height = 1+max(rh, lh);

    lh=-1, rh=-1;
    if(x->right) rh = x->right->height;
    if(x->left) lh = x->left->height;
    x->height = 1+max(rh, lh);

    return x;
}

template <typename T> Node<T>* leftRotate(Node<T>* root){
    Node<T>* x = root->right;
    Node<T>* y = x->left;
    root->right = y;
    if(y) root->rightNodes = 1 + y->leftNodes + y->rightNodes;
    else root->rightNodes = 0;

    x->left = root;
    if(root) x->leftNodes = 1 + root->leftNodes + root->rightNodes;
    else x->leftNodes=  0;

    int lh=-1, rh=-1;
    if(root->right) rh = root->right->height;
    if(root->left) lh = root->left->height;
    root->height = 1+max(rh, lh);

    lh=-1, rh=-1;
    if(x->right) rh = x->right->height;
    if(x->left) lh = x->left->height;
    x->height = 1+max(rh, lh);

    return x;
}

template <typename T> Node<T>* balanceAVLtree(Node<T>* root, T e, int bf){
    
    if(bf>1 && e<=root->left->val){
        return rightRotate(root);
    }else if(bf>1 && e>root->left->val){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }else if(bf<-1 && e>root->right->val){
        return leftRotate(root);
    }else if(bf<-1 && e<=root->right->val){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template <typename T> Node<T>* balanceAVLtree2(Node<T>* root, int balance, int bf){
    
    if(bf>1 && balance>0){
        return rightRotate(root);
    }else if(bf>1 && balance<0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }else if(bf<-1 && balance<0){
        return leftRotate(root);
    }else if(bf<-1 && balance>0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template <typename T> Node<T>* insertNode(Node<T>* root, T e){
    
    if(root==NULL){
        Node<T>* newNode = new Node<T>;
        newNode->val = e;
        newNode->height = 0;
        root = newNode;
        return root;
    }

    if(e > root->val) {
        root->rightNodes++;
        root->right = insertNode(root->right, e);
    }
    else {
        root->leftNodes++;
        root->left = insertNode(root->left, e);
    }

    int lh=-1, rh=-1;
    if(root->right) rh = root->right->height;
    if(root->left) lh = root->left->height;
    root->height = 1+max(rh, lh);

    int balanceFactor = lh-rh;
    return balanceAVLtree(root, e, balanceFactor);
}

template <typename T> Node<T>* deleteNode(Node<T>* root,T e){
    if(root == NULL) return root;
    else if(e > root->val) {
        root->right = deleteNode(root->right, e);
        if(root->right == NULL) root->rightNodes--;
        else root->rightNodes = 1 + root->right->leftNodes + root->right->rightNodes;
    }

    else if(e < root->val) {
        root->left = deleteNode(root->left, e);
        if(root->left == NULL) root->leftNodes--;
        else root->leftNodes = 1 + root->left->leftNodes + root->left->rightNodes;
    }
    else if(root->val == e){
        //to delete same value nodes(always on left)
        root->left = deleteNode(root->left, e);
        if(root->left == NULL) root->leftNodes--;
        else root->leftNodes = 1 + root->left->leftNodes + root->left->rightNodes;

        root->right = deleteNode(root->right, e);
        if(root->right == NULL) root->rightNodes--;
        else root->rightNodes = 1 + root->right->leftNodes + root->right->rightNodes;

        if(root->right == NULL && root->left==NULL){
            root=NULL;
            return root;
        }else if(root->right==NULL){
            Node<T>* curr = root->left;
            Node<T>* maxValNode = curr;
            while(curr!=NULL){
                maxValNode=curr;
                curr=curr->right;
            }
            root->val=maxValNode->val;
            root->left = deleteNode(root->left, maxValNode->val);
            if(root->left == NULL) root->leftNodes--;
            else root->leftNodes = 1 + root->left->leftNodes + root->left->rightNodes;
        }else {
            Node<T>* curr = root->right;
            Node<T>* minValNode = curr;
            while(curr!=NULL){
                minValNode=curr;
                curr=curr->left;
            }
            root->val=minValNode->val;
            root->right = deleteNode(root->right, minValNode->val);
            if(root->right == NULL) root->rightNodes--;
            else root->rightNodes = 1 + root->right->leftNodes + root->right->rightNodes;
        }
    }

    int lh=-1, rh=-1;
    if(root->right) rh = root->right->height;
    if(root->left) lh = root->left->height;
    root->height = 1+max(rh, lh);

    int balanceFactor = lh-rh;
    int balance = 0;

    if(balanceFactor>=0){
        lh=-1, rh=-1;
        if(root->left && root->left->right) rh = root->left->right->height;
        if(root->left && root->left->left) lh = root->left->left->height;
        balance = lh-rh;
    }else{
        lh=-1, rh=-1;
        if(root->right && root->right->right) rh = root->right->right->height;
        if(root->right && root->right->left) lh = root->right->left->height;
        balance = lh-rh;
    }
    

    return balanceAVLtree2(root, balance, balanceFactor);
}

template <typename T> bool searchTree(Node<T>* root, T e){
    if(root==NULL) return false;
    else if(e < root->val) return searchTree(root->left, e);
    else if(e > root->val) return searchTree(root->right, e);
    return true;
}

template <typename T> void occurenceCount(Node<T>* root, T e, int& count){
    if(root==NULL) return;
    if(root->val == e){
        count++;
        occurenceCount(root->left, e, count);
        occurenceCount(root->right, e, count);
    }else if(e > root->val){
        occurenceCount(root->right, e, count);
    }else if(e < root->val){
        occurenceCount(root->left, e, count);
    }
}

template <typename T> T lowerBound(Node<T>* root, T e){
    if(root->val==e) return e;
    if(root->left==NULL && root->right==NULL){
        return root->val;
    }
    int a;
    if(e < root->val) a = lowerBound(root->left, e);
    else if(e > root->val) a = lowerBound(root->right, e);

    if(a==e) return e;
    
    if(root->val>e && a<e) return root->val;
    else if(root->val>e && a>e){
        if(root->val-e < a-e) return root->val;
        return a;
    }else if(root->val<e && a>e) return a;
    return 0;
}

template <typename T> T upperBound(Node<T>* root, T e){
    if(root->left==NULL && root->right==NULL){
        if(root->val<=e) return 0;
        return root->val;
    }
    if(root->val<=e) return upperBound(root->right, e);
    else{
        int a = upperBound(root->left, e);
        int b = root->val;
        
        if(a>e) return min(a,b);
        else if(a==0) return b;
    }

    return 0;
}

template <typename T> T closestElement(Node<T>* root, T e){
    if(root->val == e) return e;
    if(root->left == NULL && root->right == NULL) return root->val;
    int a;
    if(e < root->val){
        if(root->left) a = closestElement(root->left, e);
        else a = root->val;
    }else if(e > root->val){
        if(root->right) a = closestElement(root->right, e);
        else a = root->val;
    }
    if(abs(a-e) >= abs(root->val-e)) return root->val;
    return a;
}

template <typename T> T kthLargest(Node<T>* root, int k){
    if(!root || k > 1 + root->leftNodes + root->rightNodes){
        return T();
    }
    if(k == root->rightNodes+1) return root->val;
    if(k > root->rightNodes+1){
        return kthLargest(root->left, k-(root->rightNodes+1));
    }
    return kthLargest(root->right, k);
}

template <typename T> T largestElement(Node<T>* root){
    return kthLargest(root, 1);
}

template <typename T> T smallestElement(Node<T>* root){
    return kthLargest(root, 1+root->leftNodes+root->rightNodes);
}

template <typename T> int countRange(Node<T>* root, T eLeft, T eRight, T& se, T& le){
   if(root == NULL || eLeft>eRight) return 0;
   if(eLeft == eRight && root->val == eLeft) return 1;
   if(root->val > eRight){
    return countRange(root->left, eLeft, eRight, se, le);
   }
   if(root->val < eLeft){
    return countRange(root->right, eLeft, eRight, se, le);
   }
   
   int totalNodes = 1+root->leftNodes+root->rightNodes;
   int leftRange = countRange(root, se, eLeft-1, se, le);
   int rightRange = countRange(root, eRight+1, le, se, le);
   return totalNodes-(leftRange+rightRange);
}

//member functions
template <typename T> void AVLtree<T>::insert(T e){
    root = insertNode(root, e);
}

template <typename T> void AVLtree<T>::Delete(T e){
    root = deleteNode(root, e);
}

template <typename T> bool AVLtree<T>::search(T e){
    return searchTree(root, e);
}

template <typename T> int AVLtree<T>::count_occurence(T e){
    int count = 0;
    occurenceCount(root, e, count);
    return count;
}

template <typename T> T AVLtree<T>::lower_bound(T e){
    return lowerBound(root, e);
}

template <typename T> T AVLtree<T>::upper_bound(T e){
    return upperBound(root, e);
}

template <typename T> T AVLtree<T>::closest_element(T e){
    return closestElement(root, e);
}

template <typename T> T AVLtree<T>::Kth_largest(int k){
    return kthLargest(root, k);
}

template <typename T> int AVLtree<T>::count_range(T eLeft, T eRight){
    T se = smallestElement(root);
    T le = largestElement(root);
    return countRange(root, eLeft, eRight, se, le);
}


template <typename T> void printBT(const std::string& prefix, const Node<T>* node, bool isRight){
    if( node != nullptr ){
        cout << prefix;
        cout << (isRight ? "├──" : "└──" );

        cout << node->val<<"("<<node->height<<", "<<node->leftNodes<<", "<<node->rightNodes<<")" << endl;

        printBT( prefix + (isRight ? "│   " : "    "), node->right, true);
        printBT( prefix + (isRight ? "│   " : "    "), node->left, false);
    }
}

template <typename T>  void printBT(const Node<T>* node){
    printBT("", node, false);    
}


int main(){
    AVLtree<int> tree;
    // 54 44 86 43 46 78 88 N N N 50 61 83 N 89
    // 8
    // 46 86 88 61 89 78 54 83
    tree.insert(54);
    tree.insert(44);
    tree.insert(86);
    tree.insert(43);
    tree.insert(46);
    tree.insert(50);
    tree.insert(78);
    tree.insert(88);
    tree.insert(50);
    tree.insert(61);
    tree.insert(83);
    tree.insert(89);
    tree.insert(50);
    

    // tree.Delete(46);
    // tree.Delete(86);
    // tree.Delete(88);
    // tree.Delete(61);
    // tree.Delete(89);
    // tree.Delete(78);
    // tree.Delete(54);
    // tree.Delete(83);

    printBT(tree.getRoot());
    
    int n=89;
    if(tree.search(n)) cout<<n<<" is present"<<"\n";
    else cout<<n<<" is not present"<<"\n";

    n=50;
    cout<<n<<" occurs "<<tree.count_occurence(n)<<" times. \n";

    n=62;
    cout<<"The lower_bound of "<<n<<" = "<<tree.lower_bound(n)<<"\n";

    n=54;
    cout<<"The upper_bound of "<<n<<" = "<<tree.upper_bound(n)<<"\n";

    n=45;
    cout<<"The closest element to "<<n<<" is: "<<tree.closest_element(n)<<"\n";

    int k = 11;
    cout<<"The "<<k<<"th largest element is: "<<tree.Kth_largest(k)<<"\n"; 

    int st=50, ed=78;
    cout<<"No: of elements in the range ("<<st<<", "<<ed<<") = "<<tree.count_range(st, ed);

    return 0;
} 
