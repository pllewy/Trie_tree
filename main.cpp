#include <iostream>

using namespace std;

struct Node {
    Node *left;
    Node *right;
    int key;
    bool leaf;

    Node() {
        left = right = nullptr;
        key = 0;
        leaf = true;
    }
};

struct Trie {
    Node *root;
    int maxb;

    Trie() {
        root = new Node();
        root->left = root->right = nullptr;
        root->leaf = false;
        maxb = 4;
    }
};

bool bit(int v, int b) {
    return (v & (1 << b)) != 0;
}

void Insert(Trie *T, int v) {
    int b = T->maxb;
    Node *x = T->root;
    Node *y = nullptr;

    cout << "Inserting " << v << endl;
    while(x != nullptr && x->key != v && b>0) {
//        TODO pozbyć się b z whila, wskaźniki nie prowadzą
        cout << b;
        y = x;
        if (bit(v, b)) {
            cout << "turn right";
            x = x->right;
        } else {
            cout << "turn left";
            x = x->left;
        }
        b--;
    }

    cout<<"after a while";

    if (x == nullptr) {
        Node *z = new Node();
        z->key = v;
        z->left = z -> right = nullptr;

        if (y != nullptr) {
            if (bit(v, b + 1)) {
                y->right = z;
            } else {
                y->left = z;
            }
        }
        else {
            T->root = z;
        }
    }
}

int main() {

//    cout << bit(3,2) << endl;
    Trie myTrie = Trie();

    Insert(&myTrie, 5);
    cout << "Inserted 5" << endl;

    cout << "Nodes:" <<endl;
    cout << myTrie.root << " " << myTrie.root->left << " " << myTrie.root->right << endl;

//    Insert(&myTrie, 3);
//    cout << "Inserted 3" << endl;
//    Insert(&myTrie, 9);
//    cout << "Inserted 9" << endl;
//    Insert(&myTrie, 1);
//    Insert(&myTrie, 7);

    // You can add more Insert calls to insert additional values

    // Print a message indicating successful insertion
//    std::cout << "Values inserted into the Trie." << std::endl;

    return 0;
}
