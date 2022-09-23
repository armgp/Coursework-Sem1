#include <iostream>
using namespace std;


template<typename T> struct Node {
public:
    T val = T();
    Node* left=NULL;
    Node* right=NULL;
    int count=1;
    int leftNodes=0;
    int rightNodes=0;
    int height;
};

template<typename T> class AVLtree {

private:
    Node<T>* root=NULL;

    Node<T>* rightRotate(Node<T>* Root){
        Node<T>* x = Root->left;
        Node<T>* y = x->right;
        Root->left = y;
        if(y) Root->leftNodes = 1 + y->leftNodes + y->rightNodes;
        else Root->leftNodes=0;

        x->right = Root;
        if(Root) x->rightNodes = 1 + Root->leftNodes + Root->rightNodes;
        else x->rightNodes=0;


        int lh=-1, rh=-1;
        if(Root->right) rh = Root->right->height;
        if(Root->left) lh = Root->left->height;
        Root->height = 1+max(rh, lh);

        lh=-1, rh=-1;
        if(x->right) rh = x->right->height;
        if(x->left) lh = x->left->height;
        x->height = 1+max(rh, lh);

        return x;
    }

    Node<T>* leftRotate(Node<T>* Root){
        Node<T>* x = Root->right;
        Node<T>* y = x->left;
        Root->right = y;
        if(y) Root->rightNodes = 1 + y->leftNodes + y->rightNodes;
        else Root->rightNodes = 0;

        x->left = Root;
        if(Root) x->leftNodes = 1 + Root->leftNodes + Root->rightNodes;
        else x->leftNodes=  0;

        int lh=-1, rh=-1;
        if(Root->right) rh = Root->right->height;
        if(Root->left) lh = Root->left->height;
        Root->height = 1+max(rh, lh);

        lh=-1, rh=-1;
        if(x->right) rh = x->right->height;
        if(x->left) lh = x->left->height;
        x->height = 1+max(rh, lh);

        return x;
    }

    Node<T>* balanceAVLtree(Node<T>* Root, T e, int bf){
        
        if(bf>1 && e<=Root->left->val){
            return rightRotate(Root);
        }else if(bf>1 && e>Root->left->val){
            Root->left = leftRotate(Root->left);
            return rightRotate(Root);
        }else if(bf<-1 && e>Root->right->val){
            return leftRotate(Root);
        }else if(bf<-1 && e<=Root->right->val){
            Root->right = rightRotate(Root->right);
            return leftRotate(Root);
        }

        return Root;
    }

    Node<T>* balanceAVLtree2(Node<T>* Root, int balance, int bf){
        
        if(bf>1 && balance>0){
            return rightRotate(Root);
        }else if(bf>1 && balance<0){
            Root->left = leftRotate(Root->left);
            return rightRotate(Root);
        }else if(bf<-1 && balance<0){
            return leftRotate(Root);
        }else if(bf<-1 && balance>0){
            Root->right = rightRotate(Root->right);
            return leftRotate(Root);
        }

        return Root;
    }

    Node<T>* insertNode(Node<T>* Root, T e){ 
        if(Root==NULL){
            Node<T>* newNode = new Node<T>;
            newNode->val = e;
            newNode->height = 0;
            Root = newNode;
            return Root;
        }

        if(e > Root->val) {
            Root->rightNodes++;
            Root->right = insertNode(Root->right, e);
        }else if(e < Root->val){
            Root->leftNodes++;
            Root->left = insertNode(Root->left, e);
        } else{
            Root->count++;
            Root->leftNodes++;
        }

        int lh=-1, rh=-1;
        if(Root->right) rh = Root->right->height;
        if(Root->left) lh = Root->left->height;
        Root->height = 1+max(rh, lh);

        int balanceFactor = lh-rh;
        return balanceAVLtree(Root, e, balanceFactor);
    }

    Node<T>* deleteNode(Node<T>* Root,T e){
        if(Root == NULL) return Root;
        else if(e > Root->val) {
            Root->right = deleteNode(Root->right, e);
            if(Root->right == NULL) Root->rightNodes--;
            else Root->rightNodes = 1 + Root->right->leftNodes + Root->right->rightNodes;
        }

        else if(e < Root->val) {
            Root->left = deleteNode(Root->left, e);
            if(Root->left == NULL) Root->leftNodes--;
            else Root->leftNodes = 1 + Root->left->leftNodes + Root->left->rightNodes;
        }
        else if(Root->val == e){
            if(Root->right == NULL && Root->left==NULL){
                Root=NULL;
                return Root;
            }else if(Root->right==NULL){
                Node<T>* curr = Root->left;
                Node<T>* maxValNode = curr;
                while(curr!=NULL){
                    maxValNode=curr;
                    curr=curr->right;
                }
                Root->val=maxValNode->val;
                Root->left = deleteNode(Root->left, maxValNode->val);
                if(Root->left == NULL) Root->leftNodes--;
                else Root->leftNodes = 1 + Root->left->leftNodes + Root->left->rightNodes;
            }else {
                Node<T>* curr = Root->right;
                Node<T>* minValNode = curr;
                while(curr!=NULL){
                    minValNode=curr;
                    curr=curr->left;
                }
                Root->val=minValNode->val;
                Root->right = deleteNode(Root->right, minValNode->val);
                if(Root->right == NULL) Root->rightNodes--;
                else Root->rightNodes = 1 + Root->right->leftNodes + Root->right->rightNodes;
            }
        }

        int lh=-1, rh=-1;
        if(Root->right) rh = Root->right->height;
        if(Root->left) lh = Root->left->height;
        Root->height = 1+max(rh, lh);

        int balanceFactor = lh-rh;
        int balance = 0;

        if(balanceFactor>=0){
            lh=-1, rh=-1;
            if(Root->left && Root->left->right) rh = Root->left->right->height;
            if(Root->left && Root->left->left) lh = Root->left->left->height;
            balance = lh-rh;
        }else{
            lh=-1, rh=-1;
            if(Root->right && Root->right->right) rh = Root->right->right->height;
            if(Root->right && Root->right->left) lh = Root->right->left->height;
            balance = lh-rh;
        }
        

        return balanceAVLtree2(Root, balance, balanceFactor);
    }

    bool searchTree(Node<T>* Root, T e){
        if(Root==NULL) return false;
        else if(e < Root->val) return searchTree(Root->left, e);
        else if(e > Root->val) return searchTree(Root->right, e);
        return true;
    }

    void occurenceCount(Node<T>* Root, T e, int& count){
        if(Root==NULL) return;
        if(Root->val == e){
            count+=Root->count;
            occurenceCount(Root->left, e, count);
            occurenceCount(Root->right, e, count);
        }else if(e > Root->val){
            occurenceCount(Root->right, e, count);
        }else if(e < Root->val){
            occurenceCount(Root->left, e, count);
        }
    }

    T lowerBound(Node<T>* Root, T e){
        if(Root == NULL) return T();
        if(Root->val==e) return e;

        if(Root->left==NULL && Root->right==NULL){
            if(Root->val >= e) return Root->val;
            return T();
        }
        
        if(Root->val<e) return lowerBound(Root->right, e);
        if(Root->val>e){
            T a = lowerBound(Root->left, e);
            if(a!=T()) return a;
            return Root->val;
        }
        
        return T();
    }

    T upperBound(Node<T>* Root, T e){
        if(Root == NULL) return T();
        if(Root->left==NULL && Root->right==NULL){
            if(Root->val<=e) return T();
            return Root->val;
        }
        if(Root->val<=e) return upperBound(Root->right, e);
        else{
            T a = upperBound(Root->left, e);
            T b = Root->val;
            
            if(a>e) return a;
            else return b;
        }

        return T();
    }

    T closestElement(Node<T>* Root, T e){
        if(Root->val == e) return e;
        if(Root->left == NULL && Root->right == NULL) return Root->val;
        T a;
        if(e < Root->val){
            if(Root->left) a = closestElement(Root->left, e);
            else a = Root->val;
        }else if(e > Root->val){
            if(Root->right) a = closestElement(Root->right, e);
            else a = Root->val;
        }
        if(abs(a-e) >= abs(Root->val-e)) return Root->val;
        return a;
    }

    T kthLargest(Node<T>* Root, int k){
        if(!Root || k > 1 + Root->leftNodes + Root->rightNodes){
            return T();
        }
        if(k == Root->rightNodes+1) return Root->val;
        if(k > Root->rightNodes+1){
            k-=(Root->rightNodes+1);
            if(Root->count > 1){
                int n=0;
                if(Root->left) n+=(1+Root->left->leftNodes+Root->left->rightNodes);
                if(k>n && k-n<Root->count) return Root->val;
            }
            return kthLargest(Root->left, k);
        }
        return kthLargest(Root->right, k);
    }

    T largestElement(Node<T>* Root){
        return kthLargest(Root, 1);
    }

    T smallestElement(Node<T>* Root){
        return kthLargest(Root, 1+Root->leftNodes+Root->rightNodes);
    }

    int countRange(Node<T>* Root, T eLeft, T eRight){
        if(Root == NULL || eLeft>eRight) return 0;
        if(eLeft == eRight && Root->val == eLeft) return Root->count;
        if(Root->val > eRight){
            return countRange(Root->left, eLeft, eRight);
        }
        if(Root->val < eLeft){
            return countRange(Root->right, eLeft, eRight);
        }
        if(Root->val == eRight){
            return Root->count + countRange(Root->left, eLeft, eRight);
        }
        if(Root->val == eLeft){
            return Root->count + countRange(Root->right, eLeft, eRight);
        }
        
        int leftRange = countRange(Root, eLeft, Root->val);
        int rightRange = countRange(Root, Root->val, eRight);
        return (leftRange+rightRange-Root->count);
    }


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


/* utils */
template <typename T> Node<T>* AVLtree<T>::getRoot(){
    return root;
}

template <typename T> void printAVL(const std::string& prefix, const Node<T>* node, bool isRight){
    if( node != nullptr ){
        cout << prefix;
        cout << (isRight ? "├──" : "└──" );

        cout << node->val<<"("<<node->height<<", "<<node->leftNodes<<", "<<node->rightNodes<<", "<<node->count<<")" << endl;

        printAVL( prefix + (isRight ? "│   " : "    "), node->right, true);
        printAVL( prefix + (isRight ? "│   " : "    "), node->left, false);
    }
}

template <typename T>  void printAVL(const Node<T>* node){
    cout<<"val(height, leftnodes, rightnode, count)"<<"\n";
    printAVL("", node, false);    
}

template <typename T> void printInorder(const Node<T>* node){
    if(node == NULL) return;
    printInorder(node->left);
    for(int i=0; i<node->count; i++) cout<<node->val<<" ";
    printInorder(node->right);
}

template <typename T> void printPreorder(const Node<T>* node){
    if(node == NULL) return;
    for(int i=0; i<node->count; i++) cout<<node->val<<" ";
    printPreorder(node->left);
    printPreorder(node->right);
}

/* member functions */
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
    return countRange(root, eLeft, eRight);
}


class myclass{
public:
    int a;
    string b;
    myclass(){};
    myclass(int A, string B){
        a=A;
        b=B;
    };
    int getA() {return a;}
    string getB() {return b;}

    bool operator<(myclass const &c){
        return a<c.a;
    }
    bool operator>(myclass const &c){
        return a>c.a;
    }
    bool operator<=(myclass const &c){
        return a<=c.a;
    }
    bool operator>=(myclass const &c){
        return a>=c.a;
    }
    bool operator==(myclass const &c){
        return a<c.a;
    }
    bool operator!=(myclass const &c){
        return a!=c.a;
    }
};

ostream &operator<<(ostream &out, myclass const &c){
    out<<"("<<c.a<<", "<<c.b<<")";
    return out;
}


/* testing */
void intTest(){
    AVLtree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(7);
    tree.insert(7);
    // tree.insert(61);
    // tree.insert(83);
    // tree.insert(89);
    // tree.insert(50);
    
    // tree.Delete(50);
    // tree.Delete(86);
    // tree.Delete(88);
    // tree.Delete(61);
    // tree.Delete(89);
    // tree.Delete(100);
    // tree.Delete(54);
    // tree.Delete(83);

     printAVL(tree.getRoot());

     cout<<tree.count_occurence(7);
    
    // int n=89;
    // if(tree.search(n)) cout<<n<<" is present"<<"\n";
    // else cout<<n<<" is not present"<<"\n";

    // n=50;
    // cout<<n<<" occurs "<<tree.count_occurence(n)<<" times. \n";

    // n=62;
    // cout<<"The lower_bound of "<<n<<" = "<<tree.lower_bound(n)<<"\n";

    // n=54;
    // cout<<"The upper_bound of "<<n<<" = "<<tree.upper_bound(n)<<"\n";

    // n=45;
    // cout<<"The closest element to "<<n<<" is: "<<tree.closest_element(n)<<"\n";

    // int k = 11;
    // cout<<"The "<<k<<"th largest element is: "<<tree.Kth_largest(k)<<"\n"; 

    // int st=50, ed=83;
    // cout<<"No: of elements in the range ("<<st<<", "<<ed<<") = "<<tree.count_range(st, ed)<<"\n";

    // printInorder(tree.getRoot());
    // cout<<"\n";
    // printPreorder(tree.getRoot());
}

void floatTest(){
    AVLtree<float> tree;
    tree.insert(54.77);
    tree.insert(43.22);
    tree.insert(8.4);
    tree.insert(23);
    tree.insert(90.32);
    tree.insert(50.00);
    tree.insert(78);
    tree.insert(90.43);
    tree.insert(50.01);
    tree.insert(61.02);
    tree.insert(23);
    tree.insert(8.4);
    tree.insert(12.11);
    
    tree.Delete(8.4);
    tree.Delete(54.77);
    tree.Delete(61.02);
    tree.Delete(23);

    printAVL(tree.getRoot());
    
    float n=90.43;
    if(tree.search(n)) cout<<n<<" is present"<<"\n";
    else cout<<n<<" is not present"<<"\n";

    n=8.4;
    cout<<n<<" occurs "<<tree.count_occurence(n)<<" times. \n";

    n=23.3;
    cout<<"The lower_bound of "<<n<<" = "<<tree.lower_bound(n)<<"\n";

    n=8.45;
    cout<<"The upper_bound of "<<n<<" = "<<tree.upper_bound(n)<<"\n";

    n=33;
    cout<<"The closest element to "<<n<<" is: "<<tree.closest_element(n)<<"\n";

    int k = 4;
    cout<<"The "<<k<<"th largest element is: "<<tree.Kth_largest(k)<<"\n"; 

    int st=11.11, ed=91.11;
    cout<<"No: of elements in the range ("<<st<<", "<<ed<<") = "<<tree.count_range(st, ed);
}

void stringTest(){
    AVLtree<string> stree;
    
    stree.insert("54");
    stree.insert("44");
    stree.insert("86");
    stree.insert("43");
    stree.insert("46");
    stree.insert("50");
    stree.insert("78");
    stree.insert("88");
    stree.insert("50");
    stree.insert("61");
    stree.insert("83");
    stree.insert("89");
    stree.insert("50");
    
    stree.Delete("50");
    stree.Delete("86");
    stree.Delete("88");
    stree.Delete("61");
    stree.Delete("89");
    stree.Delete("100");
    stree.Delete("54");
    stree.Delete("83");

     printAVL(stree.getRoot());


    string n="89";
    if(stree.search(n)) cout<<n<<" is present"<<"\n";
    else cout<<n<<" is not present"<<"\n";

    n="50";
    cout<<n<<" occurs "<<stree.count_occurence(n)<<" times. \n";

    n="62";
    cout<<"The lower_bound of "<<n<<" = "<<stree.lower_bound(n)<<"\n";

    n="62";
    cout<<"The upper_bound of "<<n<<" = "<<stree.upper_bound(n)<<"\n";

    int k = 11;
    cout<<"The "<<k<<"th largest element is: "<<stree.Kth_largest(k)<<"\n"; 

    string st="50", ed="83";
    cout<<"No: of elements in the range ("<<st<<", "<<ed<<") = "<<stree.count_range(st, ed);

}

void classTest(){
    AVLtree<myclass> ctree;
    
    myclass c1(54, "fiftyfour");
    ctree.insert(c1);

    myclass c2(44, "fourtyfour");
    ctree.insert(c2);

    myclass c3(86, "eightysix");
    ctree.insert(c3);

    myclass c4(43, "fourtythree");
    ctree.insert(c4);

    myclass c5(46, "fourtysix");
    ctree.insert(c5);

    myclass c6(50, "fifty");
    ctree.insert(c6);

    myclass c7(78, "seventyeight");
    ctree.insert(c7);

    myclass c8(88, "eightyeight");
    ctree.insert(c8);

    myclass c9(50, "fifty");
    ctree.insert(c9);

    myclass c10(61, "sixtyone");
    ctree.insert(c10);

    myclass c11(83, "eightythree");
    ctree.insert(c11);

    myclass c12(89, "eightynine");
    ctree.insert(c12);

    myclass c13(50, "fifty");
    ctree.insert(c13);

    printAVL(ctree.getRoot());
    
   
    int n = 89;
    if(ctree.search(c12)) cout<<c12.getA()<<" is present"<<"\n";
    else cout<<n<<" is not present"<<"\n";

    n=50;
    cout<<n<<" occurs "<<ctree.count_occurence(c13)<<" times. \n";

    n=62;
    cout<<"The lower_bound of "<<n<<" = "<<ctree.lower_bound(c10)<<"\n";

    n=54;
    cout<<"The upper_bound of "<<n<<" = "<<ctree.upper_bound(c1)<<"\n";

    int k = 11;
    cout<<"The "<<k<<"th largest element is: "<<ctree.Kth_largest(k)<<"\n"; 

    int st=50, ed=83;
    cout<<"No: of elements in the range ("<<st<<", "<<ed<<") = "<<ctree.count_range(c9, c11);
}

void mainTest(){
    AVLtree<int> tree;
    int operation;
    cin>>operation;
    int e;
    switch (operation){
        case 1:
            cin>>e;
            tree.insert(e);
            break;

        case 2:
            cin>>e;
            tree.Delete(e);
            break;

        case 3:
            cin>>e;
            tree.search(e);
            break;

        case 4:
            cin>>e;
            tree.count_occurence(e);
            break;

        case 5:
            cin>>e;
            tree.lower_bound(e);
            break;

        case 6:
            cin>>e;
            tree.upper_bound(e);
            break;

        case 7:
            cin>>e;
            tree.closest_element(e);
            break;

        case 8:
            int k;
            cin>>k;
            tree.Kth_largest(k);
            break;

        case 9:
            int left, right;
            cin>>left>>right;
            tree.count_range(left, right);
            break;
        
        case 10:
            printAVL(tree.getRoot());
            break;

        case 11:
            printInorder(tree.getRoot());
            break;

        case 12:
            printPreorder(tree.getRoot());
            break;
        
        default:
            break;
    }
}

int main(){
    intTest();
    // floatTest();
    // stringTest();
    // classTest()
    return 0;
} 
