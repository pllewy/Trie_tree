#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <bitset>
#include <string>

using namespace std;

int MAXB = 240;

using namespace std;

struct Patricia_Node {
    int b;
    Patricia_Node *left;
    Patricia_Node *right;
    string key;

    Patricia_Node() {
        b = MAXB;
        left = this;
        right = this;
        key = string (MAXB, '0');
    }
};

int bit(string v, int k) {

//    ciąg 01234 ma v.length = 5, czyli
//    v[0] = 0 = v[length-5], k=4
//    v[4] = 4 = v[length-1], k=0
//    stąd dodatkowe -1

    if (v.length() <= k) {
        return 0;
    } else if (v[v.length() - 1 - k] == '1') {
        return 1;
    } else {
        return 0;
    }
//    BIT DONE, na 100% działa, no nie ma szans. Ale numeracja jest stara jak w Cormenie
}

Patricia_Node * PatriciaLocate( string v, Patricia_Node *root) {
    Patricia_Node *y = root;
    Patricia_Node *x = root->left;

//    cout << "V-locate: " << v << endl;
//    cout << "X: " << x->key << " Y: " << y->key << endl;
//    cout << "X: " << x << " Y: " << y << endl;
//    cout << "X: " << x->b << " Y: " << y->b << endl;

//    Idź w głąd bit po bicie tak długo, aż nie dotrzesz do końca szlaku (b wróci do korzenia)
    while (x->b < y->b) {
//        cout << "X: " << x->key << " Y: " << y->key << endl;
//        cout << "X: " << x->b << " Y: " << y->b << endl;
        y = x;
        if (bit(v, x->b) == 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
//    cout << "Located: " << x->key << endl << endl;
    return x;
}

bool PatriciaSearch(Patricia_Node *root, string v) {
    Patricia_Node *x = PatriciaLocate( v, root);
//    cout << "X: " << x->key << endl;
    if (x->key == v) {
//        cout << "Key '" << v << "' found in Trie Trie." << endl;
        return true;
    } else {
//        cout << "Key '" << v << "' not found in Trie Trie." << endl;
        return false;
    }
}

bool PatriciaInsert(string v, Patricia_Node *root) {
    Patricia_Node *t = PatriciaLocate(v, root);
//    cout << "T: " << t->key << endl;

    if (t->key == v) {
        cout << "Key already exists in Trie Trie." << endl;
        return false;
    }
    int i = MAXB;
//    cout << endl;
//    cout << "V: " << v << endl;
//    cout << i << endl;
//    cout << "T: " << t->key << endl;
    while (bit(v, i) == bit(t->key, i)) {
        i--;
    }

//    cout << "I: " << i  << endl;

    Patricia_Node *y = root;
    Patricia_Node *x = root->left;

    while (y->b > x->b && x->b > i) {
        y = x;
        if (bit(v, x->b) == 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

//    Linia 12
// Z oznacza nowy węzeł
    Patricia_Node *newNode = new Patricia_Node();
    newNode->key = v;
    newNode->b = i;

    if (bit(v, newNode->b) == 0) {
        newNode->left = newNode;
        newNode->right = x;
    } else {
        newNode->right = newNode;
        newNode->left = x;
    }

    if (bit(v, y->b) == 0) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    return true;
}

void NodePrint(Patricia_Node *Patricia_Node) {
    cout << "Patricia_Node: " << Patricia_Node->key << " " << Patricia_Node->b << endl;
    cout << Patricia_Node->left << " " << Patricia_Node->right << endl;
}

string stringToBinary(string s) {
    string binary;
    for (char& _char : s) {
        binary += bitset<8>(_char).to_string();
    }
    return binary;
}

string binaryToString(const std::string& binaryInput) {
    std::string result = "";

    for (size_t i = 0; i < binaryInput.length(); i += 8) {
        std::bitset<8> bits(binaryInput.substr(i, 8));
        result += static_cast<char>(bits.to_ulong());
    }

    return result;
}

int countPatriciaWords(Patricia_Node *root, int &count, Patricia_Node *tree_root) {
//    count++;
    if(root->left->b < root->b) {
        count++;
        countPatriciaWords(root->left, count, tree_root);
    }
    if (root->right->b < root->b) {
        count++;
        countPatriciaWords(root->right, count, tree_root);
    }

    return count;
}

Patricia_Node * Ploader(string filename) {
//    cout << "Loading..." << endl;
    string ch;
    Patricia_Node *root;
    root = new Patricia_Node();

    ifstream fin(filename, fstream::in);
    if (fin.is_open()) {
//        cout << "File opened." << endl;
        while (fin >> ch) {
//            cout << "Inserting: " << ch << "  " << stringToBinary(ch) << endl;

            if(stringToBinary(ch).length() >= std::numeric_limits<std::size_t>::max()-5){
                cout << "String too long" << endl;
                continue;
            }
            else if (ch.empty())
                continue;

//            cout << ch << "  " << stringToBinary(ch);
            PatriciaInsert( stringToBinary(ch), root);
//            cout << " inserted" << endl;
        }
        fin.close();
    } else {
        cerr << "Unable to open the file for loading." << endl;
    }
    return root;
}

// TRIE
struct Node {
    Node *children[27]{};
    bool leaf;

    Node() {
        *children = {nullptr};
        leaf = false;
    }
};

bool Insert(Node *root, string key) {
    Node *x = root;
    bool flag = true;

    for (char i : key) {
        int index = i - 'a';

        if (!x->children[index]) {
            x->children[index] = new Node();
            x->leaf = false;
        }
        x = x->children[index];
    }

    if (x->leaf) {
        flag = false;
    }
    x->leaf = true; // Ważne. Ustalamy flagę leaf niezależnie od tego, czy są dzieci, czy nie

    return flag;
}

bool Search(Node *root, string key) {
    Node *x = root;

    for (char i : key) {
        int index = i - 'a';

        if (!x->children[index]) {
//            cout << "Not found" << endl;
            return false;
        }
        x = x->children[index];
    }
    // Jesteśmy na końcu słowa. Jeżeli są w słowniku słowa, które zaczynają się od tego słowa, to musimy w tym punkcie mieć flagę leaf true

    if (x->leaf) {
//        cout << "Found" << endl;
        return true;
    } else {
//        cout << "Not found" << endl;
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

    for (int i = 0; i < 27; i++) {
        if (root->children[i]) {
            saveTreeToFile(root->children[i], outFile, s + static_cast<char>('a' + i));
        }
    }

    if (root->leaf) {
        outFile << s << endl;
        return;
    }
}

void saveManager(Node *root, string filename){
    ofstream outFile(filename);

    if (outFile.is_open()) {
        saveTreeToFile(root, outFile, "");
        outFile.close();
    } else {
        cerr << "Unable to open the file for saving." << endl;
    }
}

bool my_predicate(char c) {
    return c < 'a' || c > 'z';
}

void massLoader(Node *root, string filename, int maxWords, int currentWords = 0) {
    int count = currentWords;
    int c;
    string ch;

    ifstream fin(filename, fstream::in);
    if (fin.is_open()) {
        while (fin >> ch && count < maxWords) {
            for (auto& x : ch)
                x = tolower(x);

            ch.erase(std::remove_if(ch.begin(), ch.end(), my_predicate), ch.end());

            if (ch.empty())
                continue;

            // If the word is new, add count
            if (Insert(root, ch)) {
                count++;
            }

            if (count == maxWords) {
                c = 0;
                count = countWords(root, c);
            }
        }
        fin.close();
    } else {
        cerr << "Unable to open the file for loading." << endl;
    }
    if (count < maxWords) {
        cout << "Loaded: " << count << " words." << endl;
        cout << "Prev file: " << filename << endl;
//        cout << "special sign: " << char(int(filename[filename.size() - 5]) + 1) << " "  << int(filename[filename.size() - 5]) << " " << filename[filename.size() - 5] << " " << endl;
        filename[filename.size() - 5] = char(int(filename[filename.size() - 5]) + 1);
        cout << "New file: " << filename << endl;

        massLoader(root, filename, maxWords, count);
    }
}

Node * loader(string filename) {
    string ch;
    Node *root;
    root = new Node();

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
    int c;
    int prepare_tree_level = 30; // 1000, 2000, 3000, 4000, 5000
    int prepare_test_level = 4; // 10, 100, 1000, 10000
    int test_tree_level = 30;
    int test_test_level = 4;
    string work_mode = "test";

    if(work_mode == "prepare") {

//    PREPARE TREES
        for (int max_words = 1; max_words <= prepare_tree_level; max_words++) {
            c = 0;
            massLoader(root, "./data/txt_files/big.txt", max_words * 1000, countWords(root, c));

            c = 0;
            if (countWords(root, c) < max_words * 1000){
                cout << "Words: " << c << endl;
            }

            string path = "./data/Trie/" + to_string(max_words * 1000) + ".txt";
            saveManager(root, path);
        }

        Node *test_prep = new Node();
//    PREPARE TESTS
        for (int max_words = 1; max_words <= prepare_test_level; max_words++) {
            c = 0;
            int words = pow(10, max_words);
            massLoader(test_prep, "./data/txt_files/rfc1.txt", words, countWords(test_prep, c));

            string path = "./data/tests/" + to_string(words) + ".txt";
            saveManager(test_prep, path);
        }
    }

    if (work_mode == "test") {

        Node *tree = new Node();
        int result_count = 0;
        string tree_path;
        string test_path;
        string ch;
        float result_procent;
        streampos tree_size;
        string label = "";
        string batch_label = "";
        int margin = 0;
        int column_width = 20;

//    TEST TRIE
        for (int max_words = 0; max_words <= test_test_level; max_words++) {
            if (max_words == 0)
                label = "trie";
            else
                label = to_string(int(pow(10, max_words)));
            margin = column_width - label.length();
            cout << string(margin / 2, ' ') << label << string(margin - margin / 2, ' ');
        }
        cout << endl;
        for (int max_words = 1; max_words <= test_tree_level; max_words++) {
            tree_path = "./data/Trie/" + to_string(max_words * 1000) + ".txt";
            tree = loader(tree_path);


//            Get the size of the file
            ifstream file(tree_path, ios::binary | ios::ate);
            tree_size = file.tellg();
            file.close();

            label = to_string(max_words * 1000) + "W[" + to_string(tree_size) + "B]";
            margin = column_width - label.length();
            cout << label << string(margin, ' ');

            for (int z = 1; z <= test_test_level; z++) {
                auto start = std::chrono::high_resolution_clock::now();
                int num_of_tests = pow(10, z);
                test_path = "./data/tests/" + to_string(num_of_tests) + ".txt";
                result_count = 0;

//                cout<<endl;
//                cout << "Tree: " << tree_path << endl;
//                cout << "Test: " << test_path << endl;

                ifstream fin(test_path, fstream::in);
                if (fin.is_open()) {
                    while (fin >> ch) {
                        if (Search(tree, ch)) {
                            result_count++;
                        }
                    }
                    fin.close();
                } else {
                    cerr << "Unable to open the file for loading." << endl;
                }

                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

                result_procent = double(result_count) / (num_of_tests) * 100;
                batch_label = "[" + to_string(result_procent) + "%," + to_string(duration.count()) + "mis]";
                margin = column_width - batch_label.length();
                cout << string(margin / 2, ' ') << batch_label << string(margin - margin / 2, ' ');
            }
            cout << endl;
        } // end of test trie
        cout << endl << endl << "####################################################" << endl << endl;

    }
    if(work_mode == "test"){
        int result_count = 0;
        string tree_path;
        string test_path;
        string ch;
        float result_procent;
        streampos tree_size;
        string label = "";
        string batch_label = "";
        int margin = 0;
        int column_width = 20;
        string percent_label = "";

        //    TEST PATRICIA
        Patricia_Node *tree2;
        for (int max_words = 0; max_words <= test_test_level; max_words++) {
            if (max_words == 0)
                label = "Patricia";
            else
                label = to_string(int(pow(10, max_words)));
            margin = column_width - label.length();
            cout << string(margin/2, ' ') << label << string(margin-margin/2, ' ');
        }
        cout << endl;
        for (int max_words = 1; max_words <= test_tree_level; max_words++) {
            tree_path = "./data/Trie/" + to_string(max_words * 1000) + ".txt";
            tree2 = Ploader(tree_path);

//            c = 0;
//            countPatriciaWords(tree2, c, tree2);
//            cout << "Words: " << c << endl;

//            Get the size of the file
            ifstream file(tree_path, ios::binary | ios::ate);
            tree_size = file.tellg();
            file.close();

            label = to_string(max_words * 1000) + "W[" + to_string(tree_size) + "B]";
            margin = column_width - label.length();
            cout <<  label << string(margin, ' ');

//            powinien być output z samymi rozmiarami plików wejściowych i nazwami

//            CZĘŚĆ Z TESTAMI, DO ODKRYCIA PO NAPRAWIENIU ŁADOWANIA

            for (int z = 1; z <= test_test_level; z++) {
                auto start = std::chrono::high_resolution_clock::now();
                int num_of_tests = pow(10, z);
                test_path = "./data/tests/" + to_string(num_of_tests) + ".txt";
                result_count = 0;

//                cout<<endl;
//                cout << "Tree: " << tree_path << endl;
//                cout << "Test: " << test_path << endl;

                ifstream fin(test_path, fstream::in);
                if (fin.is_open()) {
                    while (fin >> ch) {

                        if(stringToBinary(ch).length() >= std::numeric_limits<std::size_t>::max()-5){
                            cout << "String too long" << endl;
                            continue;
                        }
                        else {
                            if (PatriciaSearch(tree2, stringToBinary(ch))) {
                                result_count++;
                            }
                        }
                    }
                    fin.close();
                } else {
                    cerr << "Unable to open the file for loading." << endl;
                }

                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

                result_procent = double (result_count)/(num_of_tests) * 100;
//                result_procent = result_count;

                percent_label = to_string(result_procent).substr(0, 5);
                batch_label = "[" + percent_label + "%," + to_string(duration.count()) + "mis]";

                margin = column_width - batch_label.length();

                cout << string(margin/2, ' ') << batch_label << string(margin-margin/2, ' ');

            }
            cout << endl;
        } // end of test patricia
        cout << endl << endl << "####################################################" << endl << endl;

    } // end of test mode

//    char x;
//    cin >> x;
    return 0;
}
