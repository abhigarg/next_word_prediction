//
// Created by abu on 17/12/16.
//

#ifndef TESTTRIE_TRIE_H
#define TESTTRIE_TRIE_H

#endif //TESTTRIE_TRIE_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <memory>

using namespace std;

vector<string> words;  //or could be vector of pointers to word in a dictionary

int dummy = 0; //dummy weight

bool isDebug = true;

template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

void LOG(char msg){
    if(isDebug)
        cout << msg << endl;
}

void LOG(string msg){
    if(isDebug)
        cout << msg << endl;
}

class cNode{
public:
    char value;  // character value of node
    vector<std::shared_ptr<cNode>> children; //character nodes connected to this character node
    vector<std::shared_ptr<cNode>> bigrams;  //<word1, weight1>, <word2, weight2>, ..
    vector<int> bigramWeights;
    vector<vector<std::shared_ptr<cNode>>> trigrams; //word1: <word11, weight1>, <word12, weight2>, ..
                                                 //word2: <word21, weight1>, <word21, weight2>, ..
    vector<vector<int>> trigramWeights;
    std::shared_ptr<cNode> parent; //parent node

    cNode(){
        value = 0;
        parent = nullptr;
    };
};


class Trie {
public:

    std::shared_ptr<cNode> root;

    Trie(){

        LOG("trie initiated");
        root = std::make_shared<cNode>();

        LOG("root:");
        LOG(root->value);

        if(root->parent== nullptr)
            LOG("parent is null");

    };

    string getWord(std::shared_ptr<cNode> cnode);
    void getSetWordNode(const string word, bool &isWordExist, std::shared_ptr<cNode> &wordnode);

    bool getBigram(const string head, vector<pair<string, int>> &results);

    bool getTrigram(const string head1, const string head2, vector<pair<string, int>> &results);

};

//reverse traverse tree to find word represented by a leaf node
string Trie::getWord(std::shared_ptr<cNode> cnode){

    LOG("getting word from node");

    string word = "";
    while(cnode->parent!= nullptr){
        word = word+cnode->value;

        cout << "word: " << word << endl;

        cnode = cnode->parent;

    }

    if(word.length() == 0){
        LOG("no word found at node");
        return "";
    }

    if(word.length()==1)
        return word;

    string reverseWord = "";
    for(int i = word.length()-1; i == 0; i--)
        reverseWord = reverseWord + word[i];

    return reverseWord;
}

void Trie::getSetWordNode(const string word, bool &isWordExist, std::shared_ptr<cNode> &currNode){
    isWordExist = true;
    currNode = this->root;

    LOG("looking for word: ");
    LOG(word);

    //const char* wordchar = word.c_str();

    for(int i = 0; i < word.length(); i++){

        char c = word[i];

        LOG("finding node for char: ");
        LOG(c);

        if(currNode->children.empty()) {//no child node exist --> add child node

            LOG("no children found .. adding new child");

            std::shared_ptr<cNode> cnode = std::make_shared<cNode>();
            cnode->value = word[i];
            LOG("value for new node: ");
            LOG(cnode->value);
            cnode->parent = currNode;
            currNode->children.push_back(cnode);
            currNode = cnode;
            isWordExist = false;
            LOG("current node is positioned at: ");
            LOG(currNode->value);
            LOG("parent of current node: ");
            LOG(currNode->parent->value);
            continue;
        }
        else{
            LOG("children found for current node: ");
            LOG(currNode->value);
            int count = 0;
            LOG("number of children for cnode are: ");
            LOG(to_string(currNode->children.size()));

            cNode childNode = *(currNode->children[0]);
            LOG("first child: ");

            LOG(childNode.value);
            if(currNode->children.size() > 0) {
                for (int j = 0; j < currNode->children.size(); j++) {
                    cout << "child char value: " << currNode->children[j]->value << endl;
                    if (currNode->children[j]->value == c) {
                        cout << "match found: " << endl;
                        break;
                    }
                    count++;
                }

                LOG("count is: ");
                LOG(to_string(count));
                LOG("number of children for cnode are: ");
                LOG(to_string(currNode->children.size()));

                if(count == currNode->children.size()) {//value does not exist in children --> add child node for value
                    LOG("char not found in children: ");
                    LOG(c);
                    std::shared_ptr<cNode> cnode = std::make_shared<cNode>();
                    cnode->value = c;
                    currNode->children.push_back(cnode);
                    cnode->parent = currNode;
                    currNode = cnode;
                    isWordExist = false;
                    continue;
                }
                else {
                    currNode = currNode->children[count];
                    continue;
                }
            }
            else{
                LOG("no children found for char: ");
                LOG(c);
                std::shared_ptr<cNode> cnode = std::make_shared<cNode>();
                cnode->value = c;
                currNode->children.push_back(cnode);
                cnode->parent = currNode;
                currNode = cnode;
                isWordExist = false;
                continue;
            }
        }
    }

    LOG("word added");

    LOG("parent of leaf node: ");
    LOG(currNode->parent->value);
}

bool Trie::getBigram(const string head, vector<pair<string, int>> &results) {
    bool isHeadExist;

    std::shared_ptr<cNode> headNode = this->root;
    getSetWordNode(head, isHeadExist, headNode);

    if(isHeadExist){
        if(!headNode->bigrams.empty())
            for(int i = 0; i < headNode->bigrams.size(); i++)
                results.push_back(make_pair(getWord(headNode->bigrams[i]), headNode->bigramWeights[i]));
        else
            cout << "no bigrams found for the head: " << head << endl;
    }
    else{
        cout << head << " does not exist in trie .. head is added to trie";
    }


    return !results.empty();

}

bool Trie::getTrigram(const string head1, const string head2, vector<pair<string, int>> &results) {

    bool isHead1Exist, isHead2Exist = true;

    std::shared_ptr<cNode> headNode1 = this->root;
    std::shared_ptr<cNode> headNode2 = this->root;

    getSetWordNode(head1, isHead1Exist, headNode1);
    getSetWordNode(head2, isHead2Exist, headNode2);

    if(!isHead1Exist || !isHead2Exist) {
        cout << "head do not exist in trie ... head added to trie" << endl;
        //head 1 do not exist and is added --> add head2 as bigram for head1
        headNode1->bigrams.push_back(headNode2);
        headNode1->bigramWeights.push_back(dummy);
        cout << "head2 is added as bigram for head1" << endl;
        return false;
    }

    if(isHead1Exist && isHead2Exist){
        for(int i = 0; i < headNode1->trigrams.size(); i++){
            if(headNode2 == headNode1->trigrams[i][0]){
                for(int j = 1; j < headNode1->trigrams[i].size(); j++){
                    results.push_back(make_pair(getWord(headNode1->trigrams[i][j]), headNode1->trigramWeights[i][j]));
                }
            }
        }
    }

   return !results.empty();
}
