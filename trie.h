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

bool isDebug = false;

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
    int unigramWeight;
    vector<std::shared_ptr<cNode>> children; //character nodes connected to this character node
    vector<std::shared_ptr<cNode>> bigrams;  //<word1, weight1>, <word2, weight2>, ..
    vector<int> bigramWeights;
    vector<vector<std::shared_ptr<cNode>>> trigrams; //word1: <word11, weight1>, <word12, weight2>, ..
                                                 //word2: <word21, weight1>, <word21, weight2>, ..
    vector<vector<int>> trigramWeights;
    std::shared_ptr<cNode> parent; //parent node

    cNode(){
        value = 0;
        unigramWeight = 0;
        parent = nullptr;
    };
};


class Trie {
public:

    std::shared_ptr<cNode> root;

    vector<shared_ptr<cNode>> trieWords;

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

    void setTrigram(const string head1, const string head2, const string word);

    void printTree();

};


void Trie::printTree(){


    cout << "*****************TRIE******************"<< endl << endl;
    cout << "Words in Trie: " << endl << endl;

    for(int i = 0; i < this->trieWords.size(); i++) {

        shared_ptr<cNode> cnode = this->trieWords[i];

        cout << getWord(cnode) << endl;

        if(!cnode->bigrams.empty()){
            cout << "Bigrams: " << endl;
            for(int j = 0; j < cnode->bigrams.size(); j++)
                cout << "      " << getWord(cnode->bigrams[j]) << ", " << cnode->bigramWeights[j] << endl;
        }

        if(!cnode->trigrams.empty()){
            cout << "Trigrams: " << endl;
            for(int j = 0; j < cnode->trigrams.size(); j++){
                cout << getWord(cnode->trigrams[j][0]) << ": " << endl;
                for(int k = 1; k < cnode->trigrams[j].size(); k++)
                    cout << "       " << getWord(cnode->trigrams[j][k]) << "  " << cnode->trigramWeights[j][k-1] << endl;
            }
        }

    }

    cout << "**************************************"<< endl;

}

//reverse traverse tree to find word represented by a leaf node
string Trie::getWord(std::shared_ptr<cNode> cnode){

    LOG("getting word from node");

    string word = "";
    while(cnode->parent!= nullptr){
        word = word+cnode->value;

        //cout << "word: " << word << endl;

        cnode = cnode->parent;

    }

    if(word.length() == 0){
        LOG("no word found at node");
        return "";
    }

    if(word.length()==1)
        return word;

    //cout << "word: " << word << endl;

    string reverseWord = "";
    for(int i = word.length()-1; i > -1; i--)
        reverseWord = reverseWord + word[i];

    //cout << "reverse word: " << reverseWord << endl;

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
                    //cout << "child char value: " << currNode->children[j]->value << endl;
                    if (currNode->children[j]->value == c) {
                        //cout << "match found: " << endl;
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

    if(!isWordExist) {
        this->trieWords.push_back(currNode);
        currNode->unigramWeight = 1;
        cout << "word added to trie: " << word << endl;
        LOG("word added");
    }
    else
        currNode->unigramWeight = max(255, currNode->unigramWeight+1);


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
        /*else
            cout << "no bigrams found for the head: " << head << endl;*/
    }
    /*else{
        //cout << head << " does not exist in trie .. head is added to trie";
    }*/

    return !results.empty();

}

bool Trie::getTrigram(const string head1, const string head2, vector<pair<string, int>> &results) {

    bool isHead1Exist, isHead2Exist = true;

    std::shared_ptr<cNode> headNode1 = this->root;
    std::shared_ptr<cNode> headNode2 = this->root;

    getSetWordNode(head1, isHead1Exist, headNode1);
    getSetWordNode(head2, isHead2Exist, headNode2);

    if(!isHead1Exist || !isHead2Exist) {
        //cout << "head do not exist in trie ... head added to trie" << endl;
        //head 1 do not exist and is added --> add head2 as bigram for head1
        headNode1->bigrams.push_back(headNode2);
        headNode1->bigramWeights.push_back(dummy);
        //cout << "head2 is added as bigram for head1" << endl;
        return false;
    }

    if(isHead1Exist && isHead2Exist){

        if(headNode1->bigrams.empty()){
            headNode1->bigrams.push_back(headNode2);
            headNode1->bigramWeights.push_back(dummy);
        }
        else {
            //update weight for bigram of headNode1
            for (int i = 0; i < headNode1->bigrams.size(); i++) {

                long pos = find(headNode1->bigrams.begin(), headNode1->bigrams.end(), headNode2) -
                           headNode1->bigrams.begin();

                if (pos >=
                    headNode1->bigrams.size()) { //headNode2 does not exist in bigrams of headNode1 --> add to bigram
                    headNode1->bigrams.push_back(headNode2);
                    headNode1->bigramWeights.push_back(dummy);
                } else
                    headNode1->bigramWeights[pos] = max(255, headNode1->bigramWeights[pos]+1); //update weight
            }
        }

        //get trigram result
        for(int i = 0; i < headNode1->trigrams.size(); i++){
            if(headNode2 == headNode1->trigrams[i][0]){
                for(int j = 1; j < headNode1->trigrams[i].size(); j++){
                    results.push_back(make_pair(getWord(headNode1->trigrams[i][j]), headNode1->trigramWeights[i][j-1]));
                }
            }
        }
    }

   return !results.empty();
}


void Trie::setTrigram(const string head1, const string head2, const string word){

    bool isHead1Exist, isHead2Exist = true;

    std::shared_ptr<cNode> headNode1 = this->root;
    std::shared_ptr<cNode> headNode2 = this->root;

    getSetWordNode(head1, isHead1Exist, headNode1);
    getSetWordNode(head2, isHead2Exist, headNode2);

    if(!isHead1Exist || !isHead2Exist) {
        //cout << "head do not exist in trie ... head added to trie" << endl;
        //head 1 do not exist and is added --> add head2 as bigram for head1
        headNode1->bigrams.push_back(headNode2);
        headNode1->bigramWeights.push_back(dummy);
        //cout << "head2 is added as bigram for head1" << endl;
    }

    bool isWordExist = false;
    shared_ptr<cNode> wordNode = this->root;
    getSetWordNode(word, isWordExist, wordNode);

    if(headNode1->trigrams.empty()) {
        vector<shared_ptr<cNode>> trigramVec;
        trigramVec.push_back(headNode2);
        trigramVec.push_back(wordNode);
        vector<int> trigramWeights;
        trigramWeights.push_back(dummy);
        headNode1->trigrams.push_back(trigramVec);
        headNode1->trigramWeights.push_back(trigramWeights);
    }
    else{
        bool addedToTrigram = false;
        for(int i = 0; i < headNode1->trigrams.size(); i++){
            if(headNode2 == headNode1->trigrams[i][0]) {
                addedToTrigram = true;
                long pos = find(headNode1->trigrams[i].begin(), headNode1->trigrams[i].end(), wordNode)-headNode1->trigrams[i].begin();
                if( pos >= headNode1->trigrams[i].size()){
                    headNode1->trigrams[i].push_back(wordNode);
                    headNode1->trigramWeights[i].push_back(dummy);
                }
                else{
                    headNode1->trigramWeights[i][pos] = max(255, headNode1->trigramWeights[i][pos]+1); //update weight
                }
            }
        }

        if(!addedToTrigram){
            vector<shared_ptr<cNode>> newTrigram;
            newTrigram.push_back(headNode2);
            newTrigram.push_back(wordNode);
            headNode1->trigrams.push_back(newTrigram);
        }

    }

}