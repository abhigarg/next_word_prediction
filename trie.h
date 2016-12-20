//
// Created by abu on 17/12/16.
//

#ifndef TESTTRIE_TRIE_H
#define TESTTRIE_TRIE_H

#endif //TESTTRIE_TRIE_H

#include <vector>
#include <string>

using namespace std;

vector<string> words;  //or could be vector of pointers to word in a dictionary

class Trie {
public:
    int parentPos;
    vector<vector<int>> childPosScore;  //<childPos_bigram><score_bigram><childPos_trigram><score_trigram><childPos_4gram><score_4gram>
};

void addWord(const string word);

void findWord(const string word);