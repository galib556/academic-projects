// DreamDesk — Object-Oriented Desktop Item Manager
// Course   : CSE-1206 — Object Oriented Programming Lab
// Semester : 2nd Semester (Year 1, Semester 2)
// Student  : Farhan Shariar | Roll: 2307113 | KUET CSE
//
// Build    : g++ -std=c++17 DreamDesk.cpp -O2 -o DreamDesk
// Run      : ./DreamDesk

#include <bits/stdc++.h>
using namespace std;

namespace DreamDesk {

// ─── helpers ────────────────────────────────────────────────────────────────
static string trim(const string &s) {
    int a = 0, b = (int)s.size() - 1;
    while (a <= b && isspace((unsigned char)s[a])) ++a;
    while (b >= a && isspace((unsigned char)s[b])) --b;
    return (a <= b) ? s.substr(a, b - a + 1) : string();
}
static bool startsWith(const string &s, const string &p) {
    return s.size() >= p.size() && s.substr(0, p.size()) == p;
}
static string afterToken(const string &s) {
    auto pos = s.find(' ');
    if (pos == string::npos) return string();
    return trim(s.substr(pos + 1));
}

// ─── Abstract Base Class ─────────────────────────────────────────────────────
/**
 * DesktopItem — abstract base for every item that lives on the desktop.
 *
 * OOP concepts demonstrated:
 *   • Abstraction  — pure virtual open(), print(), clone(), save()
 *   • Encapsulation — 'name' is protected; accessed via getName() / renameTo()
 *   • Polymorphism  — derived classes override all pure virtuals
 *   • operator<<    — friend function for stream output
 *   • operator==    — equality by name
 */
class DesktopItem {
protected:
    string name;
public:
    explicit DesktopItem(const string &n) : name(n) {}
    virtual ~DesktopItem() {}

    string getName()          const { return name; }
    void   renameTo(const string &n) { name = n;  }

    virtual void open()                               const = 0;
    virtual void print(int indent = 0)                const = 0;
    virtual unique_ptr<DesktopItem> clone()           const = 0;
    virtual void save(ostream &os, int indent = 0)    const = 0;

    friend ostream& operator<<(ostream &os, const DesktopItem &it) {
        os << it.name; return os;
    }
    bool operator==(const DesktopItem &other) const {
        return name == other.name;
    }
};

// ─── FileItem ────────────────────────────────────────────────────────────────
/**
 * Concrete class representing a text file.
 * Inherits DesktopItem and overrides all pure virtuals.
 */
class FileItem : public DesktopItem {
    string content;
public:
    FileItem(const string &n, const string &c = "") : DesktopItem(n), content(c) {}

    void open() const override {
        cout << "[ FILE ] \"" << name << "\"\n"
             << string(40, '-') << "\n"
             << content << "\n"
             << string(40, '-') << "\n";
    }
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "- [FILE]     " << name << "\n";
    }
    unique_ptr<DesktopItem> clone() const override {
        return make_unique<FileItem>(*this);
    }
    void save(ostream &os, int indent = 0) const override {
        os << string(indent, ' ') << "FILE\n"
           << string(indent, ' ') << "NAME "    << name    << "\n"
           << string(indent, ' ') << "CONTENT " << content << "\n"
           << string(indent, ' ') << "END\n";
    }
    void   setContent(const string &c) { content = c; }
    string getContent()          const { return content; }
};

// ─── ShortcutItem ────────────────────────────────────────────────────────────
/**
 * Concrete class representing a shortcut (alias/link) to another item.
 */
class ShortcutItem : public DesktopItem {
    string targetName;
public:
    ShortcutItem(const string &n, const string &t) : DesktopItem(n), targetName(t) {}

    void open() const override {
        cout << "[ SHORTCUT ] \"" << name << "\"  →  " << targetName << "\n";
    }
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "- [SHORTCUT] " << name << "  →  " << targetName << "\n";
    }
    unique_ptr<DesktopItem> clone() const override {
        return make_unique<ShortcutItem>(*this);
    }
    void save(ostream &os, int indent = 0) const override {
        os << string(indent, ' ') << "SHORTCUT\n"
           << string(indent, ' ') << "NAME "   << name       << "\n"
           << string(indent, ' ') << "TARGET " << targetName << "\n"
           << string(indent, ' ') << "END\n";
    }
    string getTarget() const { return targetName; }
};

// ─── FolderItem ──────────────────────────────────────────────────────────────
/**
 * Concrete class representing a folder that can contain other DesktopItems.
 *
 * Uses composition: owns a vector of unique_ptr<DesktopItem> (children).
 * Demonstrates the Composite design pattern.
 */
class FolderItem : public DesktopItem {
    vector<unique_ptr<DesktopItem>> children;
public:
    explicit FolderItem(const string &n) : DesktopItem(n) {}
    ~FolderItem() override {}          // children auto-deleted via unique_ptr

    void add(unique_ptr<DesktopItem> it) { children.push_back(move(it)); }
    bool removeAt(int idx) {
        if (idx < 0 || idx >= (int)children.size()) return false;
        children.erase(children.begin() + idx);
        return true;
    }
    DesktopItem* getChild(int idx) const {
        if (idx < 0 || idx >= (int)children.size()) return nullptr;
        return children[idx].get();
    }
    int childCount() const { return (int)children.size(); }

    void open() const override {
        cout << "[ FOLDER ] \"" << name << "\"  (" << (int)children.size() << " items)\n";
        for (int i = 0; i < (int)children.size(); ++i) {
            cout << "   [" << i << "] ";
            children[i]->print(0);
        }
    }
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "+ [FOLDER]   " << name << "\n";
        for (const auto &c : children) c->print(indent + 4);
    }
    unique_ptr<DesktopItem> clone() const override {
        auto f = make_unique<FolderItem>(name);
        for (const auto &c : children) f->add(c->clone());
        return f;
    }
    void save(ostream &os, int indent = 0) const override {
        os << string(indent, ' ') << "FOLDER\n"
           << string(indent, ' ') << "NAME "  << name                  << "\n"
           << string(indent, ' ') << "COUNT " << (int)children.size()  << "\n";
        for (const auto &c : children) c->save(os, indent + 2);
        os << string(indent, ' ') << "END\n";
    }
};

// ─── Template utility (demonstrates C++ templates) ───────────────────────────
/**
 * Generic clone helper — returns a heap-allocated copy of any copyable type.
 * Demonstrates function templates.
 */
template<typename T>
T* heapClone(const T &obj) { return new T(obj); }

// ─── Desktop ─────────────────────────────────────────────────────────────────
/**
 * Desktop — manages the collection of top-level DesktopItems.
 *
 * Demonstrates:
 *   • STL containers  — vector, unordered_map
 *   • Smart pointers  — unique_ptr with move semantics
 *   • Lambda + std::function for recursive tree traversal
 *   • dynamic_cast for safe RTTI-based downcasting
 *   • File I/O        — save/load with a simple text format
 */
class Desktop {
    vector<unique_ptr<DesktopItem>>      topItems;
    unordered_map<string, DesktopItem*>  nameIndex;   // non-owning

    void rebuildIndex() {
        nameIndex.clear();
        function<void(DesktopItem*)> visit = [&](DesktopItem* p) {
            if (!p) return;
            nameIndex[p->getName()] = p;
            if (auto fo = dynamic_cast<FolderItem*>(p))
                for (int i = 0; i < fo->childCount(); ++i)
                    visit(fo->getChild(i));
        };
        for (const auto &u : topItems) visit(u.get());
    }

public:
    Desktop()  = default;
    ~Desktop() = default;

    // ── display ──────────────────────────────────────────────────────────────
    void list() const {
        cout << "\n╔══ Desktop (" << (int)topItems.size() << " top-level items) ══╗\n";
        if (topItems.empty()) { cout << "  (empty)\n"; }
        for (int i = 0; i < (int)topItems.size(); ++i) {
            cout << "  [" << i << "] ";
            topItems[i]->print(0);
        }
        cout << "╚" << string(38, '═') << "╝\n";
    }

    // ── create ───────────────────────────────────────────────────────────────
    bool createFile(const string &name, const string &content) {
        if (nameIndex.count(name)) { cout << "[!] Name already exists.\n"; return false; }
        topItems.push_back(make_unique<FileItem>(name, content));
        rebuildIndex(); return true;
    }
    bool createFolder(const string &name) {
        if (nameIndex.count(name)) { cout << "[!] Name already exists.\n"; return false; }
        topItems.push_back(make_unique<FolderItem>(name));
        rebuildIndex(); return true;
    }
    bool createShortcut(const string &name, const string &target) {
        if (nameIndex.count(name)) { cout << "[!] Name already exists.\n"; return false; }
        topItems.push_back(make_unique<ShortcutItem>(name, target));
        rebuildIndex(); return true;
    }

    // ── lookup ────────────────────────────────────────────────────────────────
    DesktopItem* findAny(const string &name) {
        auto it = nameIndex.find(name);
        return it == nameIndex.end() ? nullptr : it->second;
    }

    // ── nested file ──────────────────────────────────────────────────────────
    bool addFileToFolder(const string &folderName,
                         const string &fname, const string &content) {
        auto fo = dynamic_cast<FolderItem*>(findAny(folderName));
        if (!fo) { cout << "[!] Folder not found.\n"; return false; }
        if (nameIndex.count(fname)) { cout << "[!] Name already exists.\n"; return false; }
        fo->add(make_unique<FileItem>(fname, content));
        rebuildIndex(); return true;
    }

    // ── rename ───────────────────────────────────────────────────────────────
    bool renameItem(const string &oldName, const string &newName) {
        DesktopItem* p = findAny(oldName);
        if (!p) { cout << "[!] Item not found.\n"; return false; }
        if (nameIndex.count(newName)) { cout << "[!] New name already in use.\n"; return false; }
        p->renameTo(newName);
        rebuildIndex(); return true;
    }

    // ── delete ───────────────────────────────────────────────────────────────
    bool deleteItem(const string &name) {
        DesktopItem* target = findAny(name);
        if (!target) { cout << "[!] Item not found.\n"; return false; }
        for (int i = 0; i < (int)topItems.size(); ++i) {
            if (topItems[i].get() == target) {
                topItems.erase(topItems.begin() + i);
                rebuildIndex(); return true;
            }
        }
        for (const auto &u : topItems) {
            if (auto fo = dynamic_cast<FolderItem*>(u.get())) {
                for (int i = 0; i < fo->childCount(); ++i) {
                    if (fo->getChild(i) == target) {
                        fo->removeAt(i); rebuildIndex(); return true;
                    }
                }
            }
        }
        return false;
    }

    // ── open (view) ──────────────────────────────────────────────────────────
    bool openItem(const string &name) {
        DesktopItem* p = findAny(name);
        if (!p) { cout << "[!] Item not found.\n"; return false; }
        p->open(); return true;
    }

    // ── persistence ──────────────────────────────────────────────────────────
    bool saveTo(const string &filename) {
        ofstream ofs(filename);
        if (!ofs) { cout << "[!] Cannot open file for writing.\n"; return false; }
        ofs << "DREAMDESK_V1\n" << (int)topItems.size() << "\n";
        for (const auto &t : topItems) t->save(ofs, 0);
        ofs << "EOF\n";
        return true;
    }

    bool loadFrom(const string &filename) {
        ifstream ifs(filename);
        if (!ifs) { cout << "[!] Cannot open file for reading.\n"; return false; }

        auto nextLine = [&](string &out)->bool {
            string raw;
            while (getline(ifs, raw)) {
                out = trim(raw);
                if (!out.empty()) return true;
            }
            return false;
        };

        string header, cntLine;
        if (!nextLine(header) || header != "DREAMDESK_V1") return false;
        if (!nextLine(cntLine)) return false;
        int topCount = 0;
        try { topCount = stoi(cntLine); } catch(...) { return false; }

        function<unique_ptr<DesktopItem>()> parseItem = [&]() -> unique_ptr<DesktopItem> {
            string token;
            if (!nextLine(token)) return nullptr;

            if (token == "FILE") {
                string nLine, cLine, eLine;
                if (!nextLine(nLine) || !startsWith(nLine, "NAME "))    return nullptr;
                if (!nextLine(cLine) || !startsWith(cLine, "CONTENT ")) return nullptr;
                if (!nextLine(eLine) || eLine != "END")                  return nullptr;
                return make_unique<FileItem>(afterToken(nLine), afterToken(cLine));
            }
            if (token == "SHORTCUT") {
                string nLine, tLine, eLine;
                if (!nextLine(nLine) || !startsWith(nLine, "NAME "))   return nullptr;
                if (!nextLine(tLine) || !startsWith(tLine, "TARGET ")) return nullptr;
                if (!nextLine(eLine) || eLine != "END")                 return nullptr;
                return make_unique<ShortcutItem>(afterToken(nLine), afterToken(tLine));
            }
            if (token == "FOLDER") {
                string nLine, kLine;
                if (!nextLine(nLine) || !startsWith(nLine, "NAME "))  return nullptr;
                if (!nextLine(kLine) || !startsWith(kLine, "COUNT ")) return nullptr;
                int cnt = 0;
                try { cnt = stoi(afterToken(kLine)); } catch(...) { return nullptr; }
                auto folder = make_unique<FolderItem>(afterToken(nLine));
                for (int i = 0; i < cnt; ++i) {
                    auto child = parseItem();
                    if (!child) return nullptr;
                    folder->add(move(child));
                }
                string eLine;
                if (!nextLine(eLine) || eLine != "END") return nullptr;
                return folder;
            }
            return nullptr;
        };

        topItems.clear();
        for (int i = 0; i < topCount; ++i) {
            auto it = parseItem();
            if (!it) return false;
            topItems.push_back(move(it));
        }
        rebuildIndex();
        return true;
    }
};

// ─── CLI ─────────────────────────────────────────────────────────────────────
void showBanner() {
    cout << R"(
╔═══════════════════════════════════════════╗
║          D R E A M D E S K               ║
║    Object-Oriented Desktop Item Manager  ║
║    CSE-1206 · OOP Lab · KUET CSE         ║
╚═══════════════════════════════════════════╝
)";
}

void showMenu() {
    cout <<
    "\n┌─ Menu ──────────────────────────────┐\n"
    "│  1. List desktop                    │\n"
    "│  2. Create file (top-level)         │\n"
    "│  3. Create folder (top-level)       │\n"
    "│  4. Add file inside a folder        │\n"
    "│  5. Create shortcut (top-level)     │\n"
    "│  6. Open / view an item             │\n"
    "│  7. Rename item                     │\n"
    "│  8. Delete item                     │\n"
    "│  9. Save desktop to file            │\n"
    "│  0. Load desktop from file          │\n"
    "│  q. Quit                            │\n"
    "└─────────────────────────────────────┘\n"
    "Choice: ";
}

} // namespace DreamDesk

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    using namespace DreamDesk;
    Desktop desk;
    const string DEFAULT_SAVE = "dreamdesk_save.txt";

    showBanner();

    string input;
    while (true) {
        showMenu();
        if (!getline(cin, input)) break;
        input = trim(input);
        if (input.empty()) continue;
        if (input == "q" || input == "Q") { cout << "\nGoodbye!\n"; break; }

        int cmd = -1;
        try { cmd = stoi(input); } catch(...) { cout << "[!] Invalid choice.\n"; continue; }

        switch (cmd) {
            case 1: desk.list(); break;
            case 2: {
                cout << "File name    : "; string n; getline(cin, n);
                cout << "Content      : "; string c; getline(cin, c);
                if (desk.createFile(n, c)) cout << "[+] File created.\n";
                break;
            }
            case 3: {
                cout << "Folder name  : "; string n; getline(cin, n);
                if (desk.createFolder(n)) cout << "[+] Folder created.\n";
                break;
            }
            case 4: {
                cout << "Folder name  : "; string fn; getline(cin, fn);
                cout << "File name    : "; string fi; getline(cin, fi);
                cout << "Content      : "; string c;  getline(cin, c);
                if (desk.addFileToFolder(fn, fi, c)) cout << "[+] File added to folder.\n";
                break;
            }
            case 5: {
                cout << "Shortcut name: "; string n; getline(cin, n);
                cout << "Target name  : "; string t; getline(cin, t);
                if (desk.createShortcut(n, t)) cout << "[+] Shortcut created.\n";
                break;
            }
            case 6: {
                cout << "Item name    : "; string n; getline(cin, n);
                desk.openItem(n);
                break;
            }
            case 7: {
                cout << "Old name     : "; string o; getline(cin, o);
                cout << "New name     : "; string ne; getline(cin, ne);
                if (desk.renameItem(o, ne)) cout << "[+] Renamed.\n";
                break;
            }
            case 8: {
                cout << "Item name    : "; string n; getline(cin, n);
                if (desk.deleteItem(n)) cout << "[+] Deleted.\n";
                break;
            }
            case 9: {
                cout << "Save to (Enter = default): "; string f; getline(cin, f);
                if (f.empty()) f = DEFAULT_SAVE;
                if (desk.saveTo(f)) cout << "[+] Saved to \"" << f << "\".\n";
                break;
            }
            case 0: {
                cout << "Load from (Enter = default): "; string f; getline(cin, f);
                if (f.empty()) f = DEFAULT_SAVE;
                if (desk.loadFrom(f)) cout << "[+] Loaded from \"" << f << "\".\n";
                else cout << "[!] Load failed.\n";
                break;
            }
            default: cout << "[!] Unknown option.\n";
        }
    }
    return 0;
}
