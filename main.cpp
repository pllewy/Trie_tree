#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct Node {
    Node *children[27];
    bool leaf;

    Node() {
        *children = {nullptr};
        leaf = true;
    }
};

void Insert(Node *root, string key) {
    Node *x = root;

    for (char i : key) {
        int index = i - 'a';

        if (!x->children[index]) {
            x->children[index] = new Node();
            x->leaf = false;
        }
        x = x->children[index];
    }
    x->leaf = true; // Ważne. Ustalamy flagę leaf niezależnie od tego, czy są dzieci, czy nie
}

bool Search(Node *root, string key) {
    Node *x = root;

    for (char i : key) {
        int index = i - 'a';

        if (!x->children[index]) {
            cout << "Not found" << endl;
            return false;
        }
        x = x->children[index];
    }
    // Jesteśmy na końcu słowa. Jeżeli są w słowniku słowa, które zaczynają się od tego słowa, to musimy w tym punkcie mieć flagę leaf true

    if (x->leaf) {
        cout << "Found" << endl;
        return true;
    } else {
        cout << "Not found" << endl;
        return false;
    }
}

void Delete(Node *root, string key) {
    Node *x = root;

    for (char i : key) {
        int index = i - 'a';

        if (!x->children[index]) {
            cout << "Not found" << endl;
            return;
        }
        x = x->children[index];
    }
    // Jesteśmy na końcu słowa. Jeżeli są w słowniku słowa, które zaczynają się od tego słowa, to musimy w tym punkcie mieć flagę leaf true

    if (x->leaf) {
        cout << "Found" << endl;
        x->leaf = false; // Usuwamy słowo — jedyna różnica z wyszukiwaniem
    } else {
        cout << "Not found" << endl;
    }
}

int countWords(Node *root, int &count) {
//    Sprawdź, czy jesteśmy na końcu słowa
    if (root->leaf) {
        count++;
    }

//    Rekurencyjnie przejdź po wszystkich poziomach
    for (int i=0; i<27; i++) {
        if (root->children[i]) {
            countWords(root->children[i], count);
        }
    }
    return count;
}

void saveTreeToFile(Node* root, ofstream& outFile, string s) {

    if (root->leaf) {
        outFile << s << endl;
        return;
    }

    for (int i = 0; i < 27; i++) {
        if (root->children[i]) {
            saveTreeToFile(root->children[i], outFile, s + static_cast<char>('a' + i));
        }
    }

}

void saveManager(Node *root, string filename){
    ofstream outFile(filename);
    if (outFile.is_open()) {
        saveTreeToFile(root, outFile, "");
        outFile.close();
        cout << "Tree saved to file successfully." << endl;
    } else {
        cerr << "Unable to open the file for saving." << endl;
    }
}

bool my_predicate(char c) {
    return c < 'a' || c > 'z';
}

void massLoader(Node *root, string filename, int maxWords) {
    maxWords *= 1000;
    int count = 0;
    string ch;

    ifstream fin(filename, fstream::in);
    if (fin.is_open()) {
        while (fin >> ch && count < maxWords) {
            for (auto& x : ch)
                x = tolower(x);

            ch.erase(std::remove_if(ch.begin(), ch.end(), my_predicate), ch.end());
//            cout << ch << endl;
            Insert(root, ch);
            count++;
        }
        fin.close();
    } else {
        cerr << "Unable to open the file for loading." << endl;
    }
    if (count < maxWords)
        cout << "Loaded: " << count << " words." << endl;
}

Node * loader(string filename) {
    string ch;
    Node *root = new Node();

    ifstream fin(filename, fstream::in);
    if (fin.is_open()) {
        while (fin >> ch) {
            Insert(root, ch);
        }
        fin.close();
    } else {
        cerr << "Unable to open the file for loading." << endl;
    }
    return root;
}

int main() {
    Node *root = new Node();

    Insert(root, "abc");
    Insert(root, "bab");
    Insert(root, "aba");

//    cout << "Search" << endl;
//    Search(root, "abc");

//    cout << "Count" << endl;
//    int count = 0;
//    cout << countWords(root, count) << endl;


//    cout << "Save" << endl;
//    saveManager(root, "tree.txt");

    Node *root2 = loader("tree.txt");

    massLoader(root2, "./data/rfc1.txt", 15);

    cout << "Search" << endl;
    Search(root2, "abc");
    return 0;
}
