#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;

//SECTION 0 CUSTOM UTILITIES

template<typename T> void Swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template<typename T> const T& Max(const T& a, const T& b) { return a > b ? a : b; }
template<typename T> const T& Min(const T& a, const T& b) { return a < b ? a : b; }

template<typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;
};

template<typename T>
void ReverseArray(T* arr, int sz) {
    int i = 0, j = sz - 1;
    while(i < j) { Swap(arr[i], arr[j]); i++; j--; }
}

void ReverseString(string& s) {
    int i = 0, j = s.size() - 1;
    while(i < j) { Swap(s[i], s[j]); i++; j--; }
}

template<typename T>
class Stack {
    T* arr;
    int cap, sz;
public:
    Stack() : cap(10), sz(0) { arr = new T[cap]; }
    ~Stack() { delete[] arr; }

    void push(const T& x) {
        if (sz == cap) {
            cap *= 2;
            T* tmp = new T[cap];
            for (int i = 0; i < sz; ++i) tmp[i] = arr[i];
            delete[] arr; arr = tmp;
        }
        arr[sz++] = x;
    }
    void pop() { if (sz > 0) sz--; }
    T& top() { return arr[sz - 1]; }
    const T& top() const { return arr[sz - 1]; }
    bool empty() const { return sz == 0; }
    int size() const { return sz; }
};


template<typename T>
class Queue {
    class Node {
    public:
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node *hd, *tl; int sz;
public:
    Queue() : hd(nullptr), tl(nullptr), sz(0) {}
    ~Queue() { while(hd) { Node* t = hd->next; delete hd; hd = t; } }

    void push(const T& x) {
        Node* n = new Node(x);
        if(!tl) hd = tl = n; else { tl->next = n; tl = n; }
        sz++;
    }
    void pop() {
        if(!hd) return;
        Node* t = hd; hd = hd->next;
        if(!hd) tl = nullptr;
        delete t; sz--;
    }
    T& front() { return hd->data; }
    bool empty() const { return sz == 0; }
};

//SECTION 1 LineBuffer using Doubly Linked List

class LineNode {
public:
    string text;
    LineNode* prev;
    LineNode* next;
    LineNode(const string& t) : text(t), prev(nullptr), next(nullptr) {}
};

class LineBuffer {
public:
    LineNode* head;
    LineNode* tail;
    int sz;

    LineBuffer() : head(nullptr), tail(nullptr), sz(0) {}
    ~LineBuffer() { clear(); }

    void clear() {
        LineNode* cur = head;
        while(cur) { LineNode* t = cur->next; delete cur; cur = t; }
        head = tail = nullptr;
        sz = 0;
    }

    void insertLine(int pos, const string& text) {
        if (pos < 0 || pos > sz) pos = sz;
        LineNode* n = new LineNode(text);

        if (sz == 0) {
            head = tail = n;
        } else if (pos == 0) {
            n->next = head; head->prev = n; head = n;
        } else if (pos == sz) {
            tail->next = n; n->prev = tail; tail = n;
        } else {
            LineNode* cur = head;
            for(int i = 0; i < pos - 1; i++) cur = cur->next;
            n->next = cur->next;
            n->prev = cur;
            if(cur->next) cur->next->prev = n;
            cur->next = n;
        }
        sz++;
    }

    void deleteLine(int pos) {
        if (pos < 0 || pos >= sz) { cout << "[ERR] Invalid line.\n"; return; }
        LineNode* cur = head;
        for(int i = 0; i < pos; i++) cur = cur->next;

        if (cur->prev) cur->prev->next = cur->next;
        else head = cur->next;

        if (cur->next) cur->next->prev = cur->prev;
        else tail = cur->prev;

        delete cur;
        sz--;
    }

    string getLine(int pos) const {
        if (pos < 0 || pos >= sz) return "";
        LineNode* cur = head;
        for(int i = 0; i < pos; i++) cur = cur->next;
        return cur->text;
    }

    void setLine(int pos, const string& text) {
        if (pos < 0 || pos >= sz) return;
        LineNode* cur = head;
        for(int i = 0; i < pos; i++) cur = cur->next;
        cur->text = text;
    }

    string* toArray() const {
        if (sz == 0) return nullptr;
        string* arr = new string[sz];
        LineNode* cur = head;
        for(int i = 0; i < sz; i++) { arr[i] = cur->text; cur = cur->next; }
        return arr;
    }

    void fromArray(string* arr, int n) {
        clear();
        for(int i = 0; i < n; i++) insertLine(i, arr[i]);
    }

    void printStats() const {
        cout << " Lines : " << sz << "\n";
        int total = 0, longest = 0;
        LineNode* cur = head;
        while(cur) {
            total += cur->text.size();
            longest = Max(longest, (int)cur->text.size());
            cur = cur->next;
        }
        cout << " Chars : " << total << "\n";
        cout << " Longest line: " << longest << " chars\n";
    }

    void print() const {
        if (sz == 0) { cout << " (empty buffer)\n"; return; }
        LineNode* cur = head;
        for (int i = 0; i < sz; i++) {
            cout << setw(4) << i+1 << " | " << cur->text << "\n";
            cur = cur->next;
        }
    }
};


//SECTION 2 — UNDO/REDO STACK & HISTORY GRAPH

class Command {
public:
    string type;
    int line;
    string before;
    string after;
    int histNodeId;
    int parentHistNodeId;
};

class UndoRedoManager {
    Stack<Command> undoStack;
    Stack<Command> redoStack;
public:
    void push(const Command& c) {
        undoStack.push(c);
        while (!redoStack.empty()) redoStack.pop();
    }
    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }

    Command popUndo() {
        Command c = undoStack.top(); undoStack.pop();
        redoStack.push(c); return c;
    }
    Command popRedo() {
        Command c = redoStack.top(); redoStack.pop();
        undoStack.push(c); return c;
    }
};

class HistNode {
public:
    int id;
    string action;
    int* adj;
    int adjCap, adjSz;

    HistNode() : id(-1) {}
    HistNode(int i, string a) : id(i), action(a), adjCap(4), adjSz(0) { adj = new int[adjCap]; }
    ~HistNode() { if(id != -1) delete[] adj; }

    void addEdge(int v) {
        if(adjSz == adjCap) {
            adjCap *= 2; int* t = new int[adjCap];
            for(int k=0; k<adjSz; k++) t[k] = adj[k];
            delete[] adj; adj = t;
        }
        adj[adjSz++] = v;
    }
};


//SECTION 3 — EXPRESSION EVALUATOR

class ExpressionEvaluator {
    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3;
        return 0;
    }

    bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }

    double applyOp(double a, double b, char op) {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return (b != 0) ? a / b : 0;
            case '^': { double r = 1; for(int i=0; i<(int)b; i++) r *= a; return r; }
        }
        return 0;
    }

    void tokenize(const string& expr, string*& tokens, int& tSz) {
        int tCap = 10;
        tSz = 0;
        tokens = new string[tCap];
        string num;

        for (int i = 0; i < (int)expr.size(); i++) {
            char c = expr[i];
            if (isspace(c)) continue;
            if (isdigit(c) || c == '.') { num += c; }
            else {
                if (!num.empty()) {
                    if (tSz == tCap) {
                        tCap *= 2; string* tmp = new string[tCap];
                        for(int k=0; k<tSz; k++) tmp[k] = tokens[k];
                        delete[] tokens; tokens = tmp;
                    }
                    tokens[tSz++] = num; num = "";
                }
                if (tSz == tCap) {
                    tCap *= 2; string* tmp = new string[tCap];
                    for(int k=0; k<tSz; k++) tmp[k] = tokens[k];
                    delete[] tokens; tokens = tmp;
                }
                tokens[tSz++] = string(1, c);
            }
        }
        if (!num.empty()) {
            if (tSz == tCap) {
                tCap *= 2; string* tmp = new string[tCap];
                for(int k=0; k<tSz; k++) tmp[k] = tokens[k];
                delete[] tokens; tokens = tmp;
            }
            tokens[tSz++] = num;
        }
    }

public:
    string infixToPostfix(const string& expr) {
        string* tokens;
        int tSz;
        tokenize(expr, tokens, tSz);

        Stack<char> ops;
        string result;
        for (int i = 0; i < tSz; i++) {
            string t = tokens[i];
            if (t == "(") { ops.push('('); }
            else if (t == ")") {
                while (!ops.empty() && ops.top() != '(') {
                    result += ops.top(); result += ' '; ops.pop();
                }
                if (!ops.empty()) ops.pop();
            } else if (t.size() == 1 && isOperator(t[0])) {
                while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(t[0])) {
                    if (t[0] == '^' && precedence(ops.top()) == precedence(t[0])) break;
                    result += ops.top(); result += ' '; ops.pop();
                }
                ops.push(t[0]);
            } else {
                result += t; result += ' ';
            }
        }
        while (!ops.empty()) { result += ops.top(); result += ' '; ops.pop(); }

        delete[] tokens;
        if (!result.empty() && result.back() == ' ') result.pop_back();

        return result;
    }

    string infixToPrefix(const string& expr) {
        string* tokens;
        int tSz;
        tokenize(expr, tokens, tSz);

        for(int i = 0; i < tSz / 2; i++) Swap(tokens[i], tokens[tSz - 1 - i]);
        for(int i = 0; i < tSz; i++) {
            if(tokens[i] == "(") tokens[i] = ")";
            else if(tokens[i] == ")") tokens[i] = "(";
        }

        Stack<char> ops;
        int rCap = 10;
        int rSz = 0;
        string* resTokens = new string[rCap];

        auto addRes = [&](const string& s) {
            if (rSz == rCap) {
                rCap *= 2; string* tmp = new string[rCap];
                for(int k=0; k<rSz; k++) tmp[k] = resTokens[k];
                delete[] resTokens; resTokens = tmp;
            }
            resTokens[rSz++] = s;
        };

        for (int i = 0; i < tSz; i++) {
            string t = tokens[i];
            if (t == "(") { ops.push('('); }
            else if (t == ")") {
                while (!ops.empty() && ops.top() != '(') {
                    addRes(string(1, ops.top())); ops.pop();
                }
                if (!ops.empty()) ops.pop();
            } else if (t.size() == 1 && isOperator(t[0])) {
                while (!ops.empty() && ops.top() != '(') {
                    int pTop = precedence(ops.top());
                    int pCur = precedence(t[0]);

                    if (t[0] == '^') {
                        if (pTop < pCur) break;
                    } else {
                        if (pTop <= pCur) break;
                    }

                    addRes(string(1, ops.top())); ops.pop();
                }
                ops.push(t[0]);
            } else {
                addRes(t);
            }
        }
        while (!ops.empty()) { addRes(string(1, ops.top())); ops.pop(); }

        string result;
        for (int i = rSz - 1; i >= 0; i--) {
            result += resTokens[i];
            if (i > 0) result += ' ';
        }

        delete[] resTokens;
        delete[] tokens;
        return result;
    }

    double evalPostfix(const string& postfix) {
        Stack<double> st;
        istringstream ss(postfix);
        string token;
        while (ss >> token) {
            if (token.size() == 1 && isOperator(token[0])) {
                if (st.size() < 2) return 0;
                double b = st.top(); st.pop();
                double a = st.top(); st.pop();
                st.push(applyOp(a, b, token[0]));
            } else { st.push(stod(token)); }
        }
        return st.empty() ? 0 : st.top();
    }

    double evaluate(const string& infix) { return evalPostfix(infixToPostfix(infix)); }
};

//SECTION 4 — SORTING ALGORITHMS

class Sorter {
public:
    static void bubbleSort(string* arr, int n) {
        for (int i = 0; i < n-1; i++)
            for (int j = 0; j < n-1-i; j++)
                if (arr[j] > arr[j+1]) Swap(arr[j], arr[j+1]);
    }

    static void selectionSort(string* arr, int n) {
        for (int i = 0; i < n-1; i++) {
            int mn = i;
            for (int j = i+1; j < n; j++) if (arr[j] < arr[mn]) mn = j;
            Swap(arr[i], arr[mn]);
        }
    }

    static void insertionSort(string* arr, int n) {
        for (int i = 1; i < n; i++) {
            string key = arr[i]; int j = i-1;
            while (j >= 0 && arr[j] > key) { arr[j+1] = arr[j]; j--; }
            arr[j+1] = key;
        }
    }

    static void mergeSort(string* arr, int l, int r) {
        if (l >= r) return;
        int m = l + (r - l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        string* tmp = new string[r - l + 1];
        int i = l, j = m+1, k = 0;

        while (i <= m && j <= r) {
            if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
            else tmp[k++] = arr[j++];
        }
        while (i <= m) tmp[k++] = arr[i++];
        while (j <= r) tmp[k++] = arr[j++];

        for (int x = 0; x < k; x++) arr[l + x] = tmp[x];
        delete[] tmp;
    }

    static void quickSort(string* arr, int l, int r) {
        if (l >= r) return;
        string pivot = arr[r];
        int i = l-1;
        for (int j = l; j < r; j++)
            if (arr[j] <= pivot) { i++; Swap(arr[i], arr[j]); }
        Swap(arr[i+1], arr[r]);
        int pi = i+1;
        quickSort(arr, l, pi-1);
        quickSort(arr, pi+1, r);
    }
};

class HeapSorter {
    static void heapify(string* arr, int n, int i) {
        int largest = i, l = 2*i+1, r = 2*i+2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) { Swap(arr[i], arr[largest]); heapify(arr, n, largest); }
    }
public:
    static void heapSort(string* arr, int n) {
        for (int i = n/2-1; i >= 0; i--) heapify(arr, n, i);
        for (int i = n-1; i > 0; i--) { Swap(arr[0], arr[i]); heapify(arr, i, 0); }
    }
};


//SECTION 5 — BINARY SEARCH TREE

class BSTNode {
public:
    string word;
    int freq;
    BSTNode *left, *right;
    BSTNode(const string& w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class BST {
    BSTNode* root = nullptr;
    int nodeCount = 0;

    BSTNode* insert(BSTNode* node, const string& w) {
        if (!node) { nodeCount++; return new BSTNode(w); }
        if (w < node->word) node->left = insert(node->left, w);
        else if (w > node->word) node->right = insert(node->right, w);
        else node->freq++;
        return node;
    }

    BSTNode* search(BSTNode* node, const string& w) const {
        if (!node || node->word == w) return node;
        if (w < node->word) return search(node->left, w);
        else return search(node->right, w);
    }

    void inorderFill(BSTNode* node, Pair<string,int>* arr, int& idx) const {
        if (!node) return;
        inorderFill(node->left, arr, idx);
        arr[idx++] = {node->word, node->freq};
        inorderFill(node->right, arr, idx);
    }

    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left); destroy(node->right); delete node;
    }

public:
    ~BST() { destroy(root); }
    void insert(const string& w) { root = insert(root, w); }
    int frequency(const string& w) const {
        BSTNode* n = search(root, w);
        return n ? n->freq : 0;
    }

    void printInorder() const {
        if(nodeCount == 0) { cout << " (empty index)\n"; return; }

        Pair<string,int>* arr = new Pair<string,int>[nodeCount];
        int idx = 0;
        inorderFill(root, arr, idx);

        for (int i=0; i<nodeCount; i++) cout << "  " << left << setw(20) << arr[i].first << " x" << arr[i].second << "\n";
        delete[] arr;
    }

    void buildIndex(string* lines, int nLines) {
        destroy(root); root = nullptr; nodeCount = 0;
        for (int i=0; i<nLines; i++) {
            istringstream ss(lines[i]);
            string word;
            while (ss >> word) {
                string clean;
                for (char c : word) if (isalpha(c)) clean += tolower(c);
                if (!clean.empty()) insert(clean);
            }
        }
    }

    void topN(int n) const {
        if (nodeCount == 0) return;
        Pair<string,int>* arr = new Pair<string,int>[nodeCount];
        int idx = 0;
        inorderFill(root, arr, idx);

        for(int i=0; i<nodeCount; ++i) {
            for(int j=i+1; j<nodeCount; ++j) {
                if(arr[j].second > arr[i].second) Swap(arr[i], arr[j]);
            }
        }

        cout << " Top " << n << " words:\n";
        for (int i = 0; i < Min(n, nodeCount); i++)
            cout << "  " << left << setw(20) << arr[i].first << arr[i].second << "\n";

        delete[] arr;
    }
};


//SECTION 6 BINARY SEARCH FIND & REPLACE

class WordLoc {
public:
    string word;
    int line;
    WordLoc() : word(""), line(0) {}
    WordLoc(string w, int l) : word(w), line(l) {}
    bool operator<=(const WordLoc& o) const { return word <= o.word; }
};

class WordLocSorter {
public:
    static void mergeSort(WordLoc* arr, int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        WordLoc* tmp = new WordLoc[r - l + 1];
        int i = l, j = m+1, k = 0;
        while (i <= m && j <= r) {
            if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
            else tmp[k++] = arr[j++];
        }
        while (i <= m) tmp[k++] = arr[i++];
        while (j <= r) tmp[k++] = arr[j++];
        for (int x = 0; x < k; x++) arr[l + x] = tmp[x];
        delete[] tmp;
    }
};

int binarySearchWord(WordLoc* arr, int n, const string& target) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (arr[m].word == target) return m;
        if (arr[m].word < target) l = m + 1;
        else r = m - 1;
    }
    return -1;
}


//TEXT EDITOR SHELL

class TextEditor {
    LineBuffer buffer;
    UndoRedoManager undoRedo;
    ExpressionEvaluator expr;
    BST wordIndex;
    string filename;

    HistNode** histGraph;
    int histV, histCap;
    int currHistNode;

    void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
        system("cls");
#else
        system("clear");
#endif
    }

    void rebuildAll() {
        string* arr = buffer.toArray();
        if (arr) {
            wordIndex.buildIndex(arr, buffer.sz);
            delete[] arr;
        }
    }

    void recordHistory(Command& c, bool isNewUserAction) {
        if (isNewUserAction) {
            if (histV == histCap) {
                histCap *= 2; HistNode** t = new HistNode*[histCap];
                for(int i=0; i<histV; i++) t[i] = histGraph[i];
                delete[] histGraph; histGraph = t;
            }
            int newNode = histV++;
            string desc = c.type + " at line " + to_string(c.line + 1);
            histGraph[newNode] = new HistNode(newNode, desc);

            histGraph[currHistNode]->addEdge(newNode);
            histGraph[newNode]->addEdge(currHistNode);

            c.parentHistNodeId = currHistNode;
            c.histNodeId = newNode;
            currHistNode = newNode;
        }
    }

    void applyCommand(Command& c, bool forward, bool isNewUserAction = true) {
        if (isNewUserAction) recordHistory(c, true);

        if (c.type=="insert") {
            if (forward) buffer.insertLine(c.line, c.after);
            else buffer.deleteLine(c.line);
        } else if (c.type=="delete") {
            if (forward) buffer.deleteLine(c.line);
            else buffer.insertLine(c.line, c.before);
        } else if (c.type=="replace") {
            if (forward) buffer.setLine(c.line, c.after);
            else buffer.setLine(c.line, c.before);
        }
        rebuildAll();
    }

public:
    TextEditor() {
        histCap = 10; histV = 0;
        histGraph = new HistNode*[histCap];
        int root = histV++;
        histGraph[root] = new HistNode(root, "INIT (Empty Buffer)");
        currHistNode = root;

        Command c1{"insert", 0, "", "Welcome to DSA Text Editor!"};
        applyCommand(c1, true, true);
        undoRedo.push(c1);
    }

    ~TextEditor() {
        for(int i=0; i<histV; i++) delete histGraph[i];
        delete[] histGraph;
    }

    void loadFile(const string& fname) {
        ifstream f(fname);
        if (!f) { cout << " Cannot open '" << fname << "'\n"; return; }
        buffer.clear();
        string line;
        int idx = 0;
        while (getline(f, line)) buffer.insertLine(idx++, line);
        filename = fname;
        rebuildAll();
        cout << " Loaded " << buffer.sz << " lines from '" << fname << "'\n";
    }

    void saveFile(const string& fname) {
        string fn = fname.empty() ? filename : fname;
        if (fn.empty()) { cout << " No filename specified.\n"; return; }
        ofstream f(fn);
        string* arr = buffer.toArray();
        if (arr) {
            for (int i=0; i<buffer.sz; i++) f << arr[i] << "\n";
            delete[] arr;
        }
        filename = fn;
        cout << " Saved to '" << fn << "'\n";
    }

    void insertLine(int pos, const string& text) {
        Command c; c.type = "insert"; c.line = pos; c.before = ""; c.after = text;
        applyCommand(c, true, true);
        undoRedo.push(c);
        cout << " Inserted line " << pos+1 << "\n";
    }

    void deleteLine(int pos) {
        if (pos<0||pos>=buffer.sz) { cout<<" Invalid line.\n"; return; }
        Command c; c.type = "delete"; c.line = pos; c.before = buffer.getLine(pos); c.after = "";
        applyCommand(c, true, true);
        undoRedo.push(c);
        cout << " Deleted line " << pos+1 << "\n";
    }

    void replaceLine(int pos, const string& text) {
        if (pos<0||pos>=buffer.sz) { cout<<" Invalid line.\n"; return; }
        Command c; c.type = "replace"; c.line = pos; c.before = buffer.getLine(pos); c.after = text;
        applyCommand(c, true, true);
        undoRedo.push(c);
        cout << " Replaced line " << pos+1 << "\n";
    }

    void searchReplace(const string& oldWord, const string& newWord) {
        if (buffer.sz == 0) { cout << " Buffer empty.\n"; return; }

        string target = oldWord;
        for (char& c : target) c = tolower(c);

        int totalWords = 0;
        LineNode* cur = buffer.head;
        while (cur) {
            istringstream ss(cur->text); string w;
            while(ss >> w) totalWords++;
            cur = cur->next;
        }

        if (totalWords == 0) { cout << " No words found.\n"; return; }

        WordLoc* arr = new WordLoc[totalWords];
        int idx = 0, lineNum = 0;
        cur = buffer.head;
        while (cur) {
            istringstream ss(cur->text); string w;
            while(ss >> w) {
                string clean;
                for (char c : w) if (isalpha(c)) clean += tolower(c);
                if (!clean.empty()) arr[idx++] = WordLoc(clean, lineNum);
            }
            cur = cur->next; lineNum++;
        }

        WordLocSorter::mergeSort(arr, 0, idx - 1);
        int foundIdx = binarySearchWord(arr, idx, target);

        if (foundIdx == -1) {
            cout << " '" << oldWord << "' not found in buffer.\n";
            delete[] arr; return;
        }

        int* linesToEdit = new int[idx];
        int numLines = 0;

        int l = foundIdx;
        while (l >= 0 && arr[l].word == target) {
            bool exists = false;
            for(int i=0; i<numLines; i++) if(linesToEdit[i] == arr[l].line) exists = true;
            if(!exists) linesToEdit[numLines++] = arr[l].line;
            l--;
        }
        int r = foundIdx + 1;
        while (r < idx && arr[r].word == target) {
            bool exists = false;
            for(int i=0; i<numLines; i++) if(linesToEdit[i] == arr[r].line) exists = true;
            if(!exists) linesToEdit[numLines++] = arr[r].line;
            r++;
        }

        for (int i=0; i<numLines; i++) {
            int ln = linesToEdit[i];
            string lineText = buffer.getLine(ln);

            size_t pos = 0;
            while ((pos = lineText.find(oldWord, pos)) != string::npos) {
                lineText.replace(pos, oldWord.length(), newWord);
                pos += newWord.length();
            }

            Command c; c.type = "replace"; c.line = ln;
            c.before = buffer.getLine(ln); c.after = lineText;
            applyCommand(c, true, true);
            undoRedo.push(c);
        }

        cout << " Replaced '" << oldWord << "' with '" << newWord << "' across " << numLines << " lines.\n";
        delete[] linesToEdit;
        delete[] arr;
    }

    void undo() {
        if (!undoRedo.canUndo()) { cout << " Nothing to undo.\n"; return; }
        Command c = undoRedo.popUndo();
        currHistNode = c.parentHistNodeId;
        applyCommand(c, false, false);
        cout << " Undone: " << c.type << " at line " << c.line+1 << "\n";
    }

    void redo() {
        if (!undoRedo.canRedo()) { cout << " Nothing to redo.\n"; return; }
        Command c = undoRedo.popRedo();
        currHistNode = c.histNodeId;
        applyCommand(c, true, false);
        cout << " Redone: " << c.type << " at line " << c.line+1 << "\n";
    }

    void showHistory() {
        if (histV == 0) return;

        cout << "\n--- Edit History (BFS Traversal) ---\n";
        bool* visited = new bool[histV]();
        Queue<int> q;
        q.push(0); visited[0] = true;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            string marker = (u == currHistNode) ? " <-- CURRENT STATE" : "";
            cout << " State " << u << ": " << histGraph[u]->action << marker << "\n";
            for(int i=0; i<histGraph[u]->adjSz; i++) {
                int v = histGraph[u]->adj[i];
                if(!visited[v]) { visited[v] = true; q.push(v); }
            }
        }
        delete[] visited;

        cout << "\n--- Edit History (DFS Traversal) ---\n";
        visited = new bool[histV]();
        Stack<int> st;
        st.push(0);
        while(!st.empty()) {
            int u = st.top(); st.pop();
            if(visited[u]) continue;
            visited[u] = true;
            string marker = (u == currHistNode) ? " <-- CURRENT STATE" : "";
            cout << " State " << u << ": " << histGraph[u]->action << marker << "\n";

            for(int i=histGraph[u]->adjSz-1; i>=0; i--) {
                int v = histGraph[u]->adj[i];
                if(!visited[v]) st.push(v);
            }
        }
        delete[] visited;
    }

    void findWord(const string& word) {
        string wl = word;
        for (char& c : wl) c = tolower(c);
        int freq = wordIndex.frequency(wl);
        cout << " '" << word << "' appears " << freq << " times in index.\n";

        string* arr = buffer.toArray();
        if(!arr) return;
        for (int i = 0; i < buffer.sz; i++) {
            if (arr[i].find(word) != string::npos)
                cout << " Found at line " << i+1 << ": " << arr[i] << "\n";
        }
        delete[] arr;
    }

    void sortLines(const string& algo) {
        if (buffer.sz == 0) return;
        string* arr = buffer.toArray();

        if (algo=="bubble") Sorter::bubbleSort(arr, buffer.sz);
        else if (algo=="selection") Sorter::selectionSort(arr, buffer.sz);
        else if (algo=="insertion") Sorter::insertionSort(arr, buffer.sz);
        else if (algo=="merge") Sorter::mergeSort(arr, 0, buffer.sz-1);
        else if (algo=="quick") Sorter::quickSort(arr, 0, buffer.sz-1);
        else if (algo=="heap") HeapSorter::heapSort(arr, buffer.sz);
        else { cout << " Unknown sort algorithm.\n"; delete[] arr; return; }

        buffer.fromArray(arr, buffer.sz);
        delete[] arr;

        rebuildAll();
        cout << " Sorted lines using " << algo << " sort.\n";
    }

    void evalExpr(const string& e) {
        string postfix = expr.infixToPostfix(e);
        string prefix = expr.infixToPrefix(e);
        double result = expr.evalPostfix(postfix);

        cout << " Prefix  : " << prefix << "\n";
        cout << " Postfix : " << postfix << "\n";
        cout << " Result  : " << result << "\n";
    }

    void print() { buffer.print(); }
    void printStats() { buffer.printStats(); }
    void printWordIndex() { wordIndex.printInorder(); }
    void printTopWords(int n) { wordIndex.topN(n); }

    void help() {
        cout << R"(
============================================================
             DSA TEXT EDITOR — COMMANDS
============================================================
  print                       Show buffer with line numbers
  stats                       Buffer statistics
  insert <line> <text>        Insert line at position (1-based)
  delete <line>               Delete line
  replace <line> <text>       Replace line content
  searchreplace <old> <new>   Replace word (via Binary Search)
  undo / redo                 Undo / redo last operation
  history                     Edit history via BFS & DFS
  find <word>                 Search word (BST index)
  topwords <n>                Top N frequent words
  sort <algo>                 Sort lines lexicographically
                              (bubble/selection/insertion/merge/quick/heap)
  expr <expression>           Evaluate arithmetic expression
  save [filename]             Save buffer to file
  load <filename>             Load file into buffer
  quit                        Exit editor
============================================================
)";
    }

    void run() {
        string line;

        clearScreen();
        help();
        print();

        while (true) {
            cout << "\n> ";
            if (!getline(cin, line)) break;
            if (line.empty()) continue;

            clearScreen();
            help();

            istringstream ss(line);
            string cmd; ss >> cmd;

            if (cmd=="quit"||cmd=="exit") break;
            else if (cmd=="print") print();
            else if (cmd=="stats") printStats();
            else if (cmd=="undo") undo();
            else if (cmd=="redo") redo();
            else if (cmd=="history") showHistory();
            else if (cmd=="insert") {
                int pos; ss >> pos; pos--;
                string text; getline(ss >> ws, text);
                insertLine(pos, text);
            }
            else if (cmd=="delete") {
                int pos; ss >> pos; pos--;
                deleteLine(pos);
            }
            else if (cmd=="replace") {
                int pos; ss >> pos; pos--;
                string text; getline(ss >> ws, text);
                replaceLine(pos, text);
            }
            else if (cmd=="searchreplace") {
                string o, n; ss >> o >> n;
                searchReplace(o, n);
            }
            else if (cmd=="find") { string word; ss >> word; findWord(word); }
            else if (cmd=="sort") { string algo; ss >> algo; sortLines(algo); }
            else if (cmd=="topwords") { int n=10; ss>>n; printTopWords(n); }
            else if (cmd=="expr") { string e; getline(ss >> ws, e); evalExpr(e); }
            else if (cmd=="save") { string fn; ss>>fn; saveFile(fn); }
            else if (cmd=="load") { string fn; ss>>fn; loadFile(fn); }
            else cout << " Unknown command. Type 'help' or see the command list above.\n";
        }
    }
};

// MAIN
int main() {
    TextEditor mainEditorProgram;
    mainEditorProgram.run();
    return 0;
}
