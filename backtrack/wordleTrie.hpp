using namespace std;
using std::string;
using std::cout;
using std::vector;

class TrieNode;

/*
** Enforce Wordle rules on trie nodes. Multiple calls to add rules can be made.
** filter() reduces the search space by pruning trie nodes based on rules.
** check*() function checks if given word does not violate any rules
*/
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

/*
** Trie data structure to build a compressed dictionary of words
** Works on fixed alphabet size consisting of lower case English alphabets
** and offers modifications to trie after it has been built
*/
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
