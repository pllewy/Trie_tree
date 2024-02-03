#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

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
        int margin=0;
        int column_width = 20;

//    TESTS
        for (int max_words = 0; max_words <= test_test_level; max_words++) {
            if (max_words == 0)
                label = "trie";
            else
                label = to_string(int(pow(10, max_words)));
            margin = column_width - label.length();
            cout << string(margin/2, ' ') << label << string(margin-margin/2, ' ');
        }
        cout << endl;
        for (int max_words = 1; max_words <= test_tree_level; max_words++) {
            tree_path = "./data/Trie/" + to_string(max_words * 1000) + ".txt";
            tree = loader(tree_path);


            ifstream file(tree_path, ios::binary | ios::ate);
            tree_size = file.tellg();
            file.close();

            label = to_string(max_words * 1000) + "W[" + to_string(tree_size) + "B]";
            margin = column_width - label.length();
            cout <<  label << string(margin, ' ');

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

                result_procent = double (result_count)/(num_of_tests) * 100;
                batch_label = "[" + to_string(result_procent) + "%," + to_string(duration.count()) + "mis]";
                margin = column_width - batch_label.length();
                cout << string(margin/2, ' ') << batch_label << string(margin-margin/2, ' ');
            }
            cout << endl;
        }
    }

    return 0;
}
