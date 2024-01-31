#include <iostream>

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
        root = new Node;
        root->left = root->right = root;
        root->leaf = false;
        maxb = 5;
    }
};

bool bit(int v, int b) {
    return (v & (1 << b)) != 0;
}

void Insert(Trie *T, int v) {
    int b = T->maxb;
    Node *x = T->root;
    Node *y = nullptr;

    while(x != nullptr && x->key != v) {
        y = x;
        if (bit(v, b)) {
            x = x->right;
        } else {
            x = x->left;
        }
        b--;
    }

    if (x == nullptr) {
        Node z = Node();
        z.key = v;
        z.left = z.right = nullptr;
        if (y != nullptr) {
            if (bit(v, b + 1)) {
                y->right = &z;
            } else {
                y->left = &z;
            }
        }
        else {
            T->root = &z;
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
