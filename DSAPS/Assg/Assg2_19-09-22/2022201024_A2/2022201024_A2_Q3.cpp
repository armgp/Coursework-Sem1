#include <iostream>
using namespace std;

template<typename T> struct Node {
public:
    T val;
    Node* left=NULL;
    Node* right=NULL;
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
    x->right = root;

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
    x->left = root;

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

    if(e > root->val) root->right = insertNode(root->right, e);
    else root->left = insertNode(root->left, e);

    int lh=-1, rh=-1;
    if(root->right) rh = root->right->height;
    if(root->left) lh = root->left->height;
    root->height = 1+max(rh, lh);

    int balanceFactor = lh-rh;
    return balanceAVLtree(root, e, balanceFactor);
}

template <typename T> Node<T>* deleteNode(Node<T>* root,T e){
    if(root == NULL) return root;
    else if(e > root->val) root->right = deleteNode(root->right, e);
    else if(e < root->val) root->left = deleteNode(root->left, e);
    else if(root->val == e){
        //to delete same value nodes(always on left)
        root->left = deleteNode(root->left, e);
        root->right = deleteNode(root->right, e);

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
        }else {
            Node<T>* curr = root->right;
            Node<T>* minValNode = curr;
            while(curr!=NULL){
                minValNode=curr;
                curr=curr->left;
            }
            root->val=minValNode->val;
            root->right = deleteNode(root->right, minValNode->val);
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
 
}

template <typename T> T AVLtree<T>::upper_bound(T e){
 
}

template <typename T> T AVLtree<T>::closest_element(T e){
 
}

template <typename T> T AVLtree<T>::Kth_largest(int k){
 
}

template <typename T> int AVLtree<T>::count_range(T eLeft, T eRight){
 
}


template <typename T> void printBT(const std::string& prefix, const Node<T>* node, bool isRight){
    if( node != nullptr ){
        cout << prefix;
        cout << (isRight ? "├──" : "└──" );

        cout << node->val<<"("<<node->height<<")" << endl;

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

    return 0;
} 
