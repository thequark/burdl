#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include <fstream>
#include <unistd.h>

#include "wordleTrie.hpp"

using namespace std;

using std::string;
using std::cout;
using std::vector;

void Rules::addGreenRule(const string& pattern) {
    for(int i=0;i<pattern.size();i++) {
        if (pattern[i] != '-') {
            fixedPosition[i] = pattern[i];
        }
    }
}

void Rules::addYellowRule(const string& pattern) {
    for(int i=0;i<pattern.size();i++) {
        if (pattern[i] != '-') {
            negativePosition[i].push_back(pattern[i]);
            allYellowChars.insert(pattern[i]);
        }
    }
}

void Rules::addExcludedLetters(const string& letters) {
    for(char letter : letters) {
        excludedLetters.insert(letter);
    }
}

void Rules::filter(string& word, int position, TrieNode* node) {
    // apply green rule. On this position
    if (fixedPosition.find(position) != fixedPosition.end()) {
        char greenLetter = fixedPosition[position];

         for(int i=0;i<TrieNode::ALPHABET_SIZE;i++) {
             char c = node->getChar(i);
             if (c && c != greenLetter) {
                node->clearChar(c);
             }
         }
    }

    // grey rule
    for(int index=0;index<TrieNode::ALPHABET_SIZE;index++) {
        char c = node->getChar(index);
        if (c && excludedLetters.find(c) != excludedLetters.end()) {
            node->clearChar(c);
        }
    }
    
    // yellow rule
    for(char excludedChar : negativePosition[position]) {
        for(int index=0;index<TrieNode::ALPHABET_SIZE;index++) {
            char c = node->getChar(index);
            if (c && c == excludedChar) {
                node->clearChar(c);
            }
        }
    }
}

bool Rules::checkYellow(const string& word) {
    for(auto c : allYellowChars) {
        if (word.find(c) == word.npos) {
            return false;
        }
    }
    return true;
}

TrieNode::TrieNode()
    : isEndOfWord(false) {
        clearAll(true);
    }

void TrieNode::clearAll(const bool ignoreNonNull) {
    for(int i=0;i<ALPHABET_SIZE;i++) {
        if (children[i] && !ignoreNonNull) {
            children[i]->clearAll(ignoreNonNull);
        }
        children[i] = nullptr;
        frequency[i] = 0;
    }
}

void TrieNode::browse(TrieNode* root, Rules& rules) {
    string word;
    browse(root, -1, word, rules);
}

void TrieNode::insert(TrieNode* root, string key) {
    TrieNode* curr = root;
    for(int i=0;i<key.length();i++) {
        int index = key[i]-'a';
        if (curr->children[index] == nullptr) {
            curr->children[index] = new TrieNode();
        }
        curr->frequency[index]++;
        curr = curr->children[index];
    }
    curr->isEndOfWord=true;
}

bool TrieNode::compareFreq(char a, char b) {
    return true;
}

void TrieNode::browse(TrieNode* root, int position, string partialWord, Rules& rules) {
    if (root->isEndOfWord && rules.checkYellow(partialWord)) {
        cout << partialWord << endl;
        return;
    }

    position++;

    rules.filter(partialWord, position, root);

    vector<pair<int, char>> frequentChars;

    for(int index=0;index<ALPHABET_SIZE;index++) {
        char c = (char)('a'+index);
        if (root->children[index] != nullptr)
            frequentChars.push_back({root->frequency[index], c});
    }

    sort(frequentChars.begin(), frequentChars.end(), std::greater<pair<int, char>>());

    for(auto p : frequentChars) {
        char c = p.second;
        partialWord.push_back(c);
        int index = c - 'a';

        browse(root->children[index], position, partialWord, rules);

        partialWord.erase(partialWord.size()-1);
    }

}

char TrieNode::getChar(int position) {
    if (children[position]) {
        return 'a' + position;
    }
    return 0;
}

void TrieNode::clearChar(char c) {
    int index = c - 'a';
    if (children[index]) {
        children[index]->clearAll(false);
        children[index] = nullptr;
        frequency[index] = 0;
    }
}

int main(int argc, char**argv) {
    int c;
    char *cvalue;
    int index;

    Rules r = Rules();
    while ((c = getopt (argc, argv, "p:g:y:")) != -1) {
        switch (c)
        {
            case 'p':
            r.addGreenRule(optarg);
            break;
            case 'g':
            r.addExcludedLetters(optarg);
            break;
            case 'y':
            r.addYellowRule(optarg);
            break;
        }
    }

    std::ifstream infile("words.txt");
    string word;
    
    TrieNode root = TrieNode();
    
    while(infile >> word) {
        //cout << word << endl;
        TrieNode::insert(&root, word);
    }

    TrieNode::browse(&root, r);
}
