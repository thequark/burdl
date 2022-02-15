using namespace std;
using std::string;
using std::cout;
using std::vector;

class TrieNode;

class Rules {
    public:
    void addGreenRule(const string& pattern);
    void addYellowRule(const string& pattern);
    void addExcludedLetters(const string& letters);
    void filter(string& word, int position, TrieNode* root);
    bool checkYellow(const string& word);
    private:
    map<int, char> fixedPosition;
    map<int, vector<char>> negativePosition;
    set<char> allYellowChars;
    set<char> excludedLetters;
};

class TrieNode {
    public:
    TrieNode();
    void clearAll(const bool ignoreNonNull);
    char getChar(int position);
    void clearChar(char c);
    static void browse(TrieNode* root, Rules& rules);
    static void insert(TrieNode* root, string key);
    static const int ALPHABET_SIZE=26;
private:
    bool compareFreq(char a, char b);
    static void browse(TrieNode* root, int position, string partialWord, Rules& rules);
    
    TrieNode* children[ALPHABET_SIZE];
    int frequency[ALPHABET_SIZE];
    bool isEndOfWord;
};
