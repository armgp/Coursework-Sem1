#include <iostream>
#include <vector>
using namespace std;

struct Node{
    vector<Node*> charachters;
    bool isEnd;
    Node(){
        charachters.resize(256, NULL);
        isEnd = false;
    }
};


class Trie {
private:
    Node* root;
public:
    Trie(){
        root = new Node();
    }

    void insertWord(string word){
        Node* curr = root;
        for(char c : word){
            if(curr->charachters[c-'a'] == NULL){
                Node* newNode = new Node();
                curr->charachters[c-'a'] = newNode;
            }
            curr = curr->charachters[c-'a'];
        }
        curr->isEnd = true;
    }

    bool spellCheck(string word){
        Node* curr = root;
        for(char c : word){
            if(curr->charachters[c-'a'] == NULL) return false;
            curr = curr->charachters[c-'a'];
        }
        if(curr->isEnd) return true;
        return false;
    }

    void findAllWords(Node* curr, string currWord, vector<string>& res){
        if(curr->isEnd) res.push_back(currWord);
        for(int i=0; i<256; i++){
            Node* nextNode = curr->charachters[i];
            if(nextNode) {
                char c = 'a'+i;
                findAllWords(nextNode, currWord+c, res);
            }
        }
    }

    vector<string> autocomplete(string word){
        vector<string> res;
        Node* curr = root;
        for(char c : word){
            if (curr->charachters[c-'a'] == NULL) return res;
            curr = curr->charachters[c-'a'];
        }

        findAllWords(curr, word, res);

        return res;
    }

    int editDistance(string word1, int n, string word2, int m){
        if(n==0 || m==0) return n+m;
        if(n==m && word1==word2) return 0;
        if(word1[n-1] == word2[m-1]) return editDistance(word1, n-1, word2, m-1);
        else{
            int a = 1+editDistance(word1, n-1, word2, m-1);
            int b = 1+editDistance(word1, n-1, word2, m);
            int c = 1+editDistance(word1, n, word2, m-1);
            int d = min(a, min(b, c));
            return d;
        }
        return 0;
    }

    void getEdit3Words(Node* curr, string& ipWord, string currWord, vector<string>& res){
        if(curr->isEnd){
            int d = editDistance(ipWord, ipWord.size(), currWord, currWord.size());
            if(d<=3){
                res.push_back(currWord);
            }else{
                return;
            }
        }
        for(int i=0; i<256; i++){
            Node* nextNode = curr->charachters[i];
            if(nextNode ) {
                char c = 'a'+i;
                getEdit3Words(nextNode, ipWord, currWord+c, res);
            }
        }
    }

    vector<string> autocorrect(string word){
        vector<string> res;
        Node* curr = root;
        getEdit3Words(curr, word, "", res);
        return res;
    }
};

int main(){
    int n, q;
    cin>>n>>q;
    string inputWord;
    Trie trie;
    for(int i=0; i<n; i++){
        cin>>inputWord;
        trie.insertWord(inputWord);
    }

    int a;
    string t;
    vector<string> res;
    while(q--){
        cin>>a>>t;
        switch (a){
        case 1:
            trie.spellCheck(t) ? cout<<"1\n" : cout<<"0\n";
            break;

        case 2:
            res = trie.autocomplete(t);
            cout<<res.size()<<"\n";
            for(string word : res){
                cout<<word<<"\n";
            }
            break;

        case 3:
            res = trie.autocorrect(t);
            cout<<res.size()<<"\n";
            for(string word : res){
                cout<<word<<"\n";
            }
            break;
        
        default:
            break;
        }
    }
    return 0;
}