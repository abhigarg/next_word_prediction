
#include <assert.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>

#include "trie.h"

int maxLevel = 3;

int chknum = 7;

string allowedLetters = "aiouAI";

//check if a character is a digit or not
bool isNum(const char c)
{
    if(c == 46)
        return true;

    if(c >= 48 && c <= 57)
        return true;

    return false;
}

//check if a character is allowed or not
bool isLegal(const char c)
{
    if(c >= 97 && c <= 122)
        return true;

    if(c == 'A' || c == 'I')
        return true;

    return false;
}

//remove all non-allowed characters from a word
string removeIllegal(string word){
    string output = "";
    for(int i = 0; i < word.length(); i++)
        if(isLegal(word[i]))
            output = output+word[i];
    return output;
}

//extract digits from a string
string getNum(string numStr){
    string out = "";
    for(int i = 0; i < numStr.length(); i++)
        if(isNum(numStr[i]))
            out = out + numStr[i];

    if(out.length() > 0)
        return out;
    return "";
}

void toLowerCase(string &word){
    if(word.length()==1){
        if(word[0]!='I' && word[0] != 'A')
            word[0] = tolower(word[0]);
    }
    if(word.length() > 1){
        int i = 0;
        while(word[i]){
            word[i] = tolower(word[i]);
            i++;
        }
    }

}


bool compare(const pair<string,int> &a,const pair<string,int> &b)
{
    return a.second > b.second;
}


void splitUnigram(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        string word = removeIllegal(item);

        if(word.length()>0) {
            toLowerCase(word);
            elems.push_back(word);
        }
        string num = getNum(item);
        if(num.length()>0)
            elems.push_back(num);
    }
}

void splitBigram(const string &s, char delim1, char delim2, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    //cout << "splitting into words..." << endl;
    while (getline(ss, item, delim1)) {
        //cout << "item: " << item << endl;
        string word = removeIllegal(item);

        if(word.length()>0){
            toLowerCase(word);
            elems.push_back(word);
        }
        else{
            stringstream ss2;
            ss2.str(item);
            string item2;
            vector<string> elems2;
            while(getline(ss2, item2, delim2)){
                //cout << "item2: " << item2 << endl;
                string num = getNum(item2);
                if(num.length()>0)
                    elems2.push_back(num);
            }

            if(elems2.size()>=2)
                elems.push_back(elems2[1]);
        }
    }

    //cout << "elems size: " << elems.size() << endl;

}


void testDryRun(Trie &t){

    cout << endl << "*****************Dry-Run**************" << endl << endl;


    //t.printTree();

    vector<string> user_input;
    isDebug = false;

    int count = 0;

    while(1){
        string curr_input = "";
        cout << "user: ";
        getline(cin, curr_input);

        if(curr_input.length() == 0) {
            //cout << "1" << endl;
            /*if(count == 2)
                t.printTree();*/
            if (count >= 3)
                break;
            count++;
            user_input.clear();
        }
        else{

            count = 0;
            toLowerCase(curr_input);
            //split(curr_input, ' ', user_input); //split by space
            user_input.push_back(curr_input);

            vector<pair<string, int>> out;
            vector<string> predictions;

            if(user_input.size() == 3){

                cout << "user input order: " << user_input[0] << " " << user_input[1] << " " << user_input[2] << endl;

                t.setTrigram(user_input[0], user_input[1], user_input[2]);
            }

            while(user_input.size() > 2)
                user_input.erase(user_input.begin());

            if(user_input.size() == 2){

                cout << "user input order: " << user_input[0] << " " << user_input[1] << endl;

                if(t.getTrigram(user_input[0], user_input[1], out)){
                    cout << "Predictions for " << user_input[0] << " " << user_input[1] << endl;

                    for(int i = 0; i < out.size(); i++){
                        //cout << out[i].first << ", ";
                        predictions.push_back(out[i].first);
                        if(predictions.size()==3)
                            break;
                    }

                    if(predictions.size() < 3){
                        int n = (int)out.size();
                        cout << "value of n: " << n << endl;
                        out.clear();
                        if(t.getBigram(user_input[1], out)){
                            sort(out.begin(), out.end(), compare);
                            for(int i = 0; i < out.size(); i++){
                                //cout << out[i].first << ", ";
                                if(find(predictions.begin(), predictions.end(), out[i].first)-predictions.begin() >= predictions.size())
                                    predictions.push_back(out[i].first);

                                if(predictions.size()==3)
                                    break;
                            }
                        }
                    }

                    cout << endl;

                }
                else{
                    if(t.getBigram(user_input[1], out)){
                        sort(out.begin(), out.end(), compare);
                        cout << "Predictions for " << user_input[1] << endl;
                        for(int i = 0; i < out.size(); i++){
                            //cout << out[i].first << ", ";
                            predictions.push_back(out[i].first);
                            if(predictions.size()==3)
                                break;
                        }
                    }
                }
            }

            if(user_input.size()==1){
                if(t.getBigram(user_input[0], out)){
                    sort(out.begin(), out.end(), compare);
                    cout << "Predictions for " << user_input[0] << endl ;
                    for(int i = 0; i < out.size(); i++){
                        //cout << out[i].first << ", ";
                        predictions.push_back(out[i].first);
                        if(predictions.size()==3)
                            break;
                    }
                }
                else
                    cout << "no bigram found for word" << endl;
            }


            if(predictions.empty())
                continue;

            //show predictions
            for(int i = 0; i < predictions.size()-1; i++)
                cout << predictions[i] << ", ";
            cout << predictions[predictions.size()-1] << endl;
        }

    }
}


vector<shared_ptr<cNode>> topWordNodes;

void addUnigrams(Trie &t, const string unigramFile){

    cout << "adding unigrams to trie .. " << endl;
    //read from unigram file and add to trie
    FILE *uniFile = fopen(unigramFile.c_str(), "r");
    assert(uniFile);

    char line[256];
    int linecount = 0;
    int maxScore = 0;
    //cout << "Reading unigrams line by line" << endl;
    while(fgets(line, sizeof(line), uniFile)){
        //cout << "line: " << line << endl;
        //cout << "linecount: " << linecount << endl;
        string linestr(line);

        if(linestr.empty()) {
            linecount++;
            continue;
        }

        linestr.erase(remove(linestr.begin(), linestr.end(), '\n'), linestr.end());
        linestr.erase(remove(linestr.begin(), linestr.end(), '\r'), linestr.end());
        //cout << "line string: " << linestr << endl;

        vector<string> unigram; //word weight
        splitUnigram(linestr, ' ', unigram);

        if(unigram.size() != 2)
            continue;

        //cout << unigram[0] << endl;

        /*if(linecount ==1)
            break;*/
        int score = atoi(unigram[0].c_str());

        //cout << "score: " << score << endl;

        if(linecount==0)
            maxScore = score;

        //cout << "max score: " << maxScore;
        //cout << "word: " << unigram[1] << " score: " << (int)(atoi(unigram[0].c_str())*255/maxScore) << endl;

        //add word to trie
        shared_ptr<cNode> cnode;
        bool isWordExist = false;

        //cout << "length of word: " << unigram[1].length() << endl;

        if(unigram[1].length() <= maxLevel) {
            //cout << "chk1" << endl;
            t.getSetWordNode(unigram[1], isWordExist, cnode);
            //cout << "chk6" << endl;
            //cout << "unigram weight: " << score*255/maxScore << endl;
            cnode->unigramWeight = (score*255/maxScore); //(unsigned char)
            linecount++;
            //cout << "chk" << endl;
        }

        if(topWordNodes.size()<3) {
            topWordNodes.push_back(cnode);
            cout << "top word node value: " << cnode->value << endl;
        }

        //cout << "added word: " << unigram[1] << endl;
        //cout << "word count: " << linecount << endl;
    }

    //cout << "words in trie: " << endl;
    //t.printTree();

    cout << "total number of words: " << trieWords.size() << endl;
}

vector<shared_ptr<cNode>> nodesWithBigrams;

void addBigrams(Trie &t, string bigramFile){

    //read from unigram file and add to trie
    FILE *biFile = fopen(bigramFile.c_str(), "r");
    assert(biFile);

    char line[256];
    int linecount = 0;
    int maxScore = 0;

    //cout << "Reading unigrams line by line" << endl;
    while(fgets(line, sizeof(line), biFile)) {

        linecount++;

        if(linecount==1)
            continue;
        //cout << "line: " << line << endl;
        string linestr(line);

        if(linestr.empty())
            continue;

        linestr.erase(remove(linestr.begin(), linestr.end(), '\n'), linestr.end());
        linestr.erase(remove(linestr.begin(), linestr.end(), '\r'), linestr.end());

        vector<string> bigram; //word1 word2 weight
        splitBigram(linestr, '>', ' ', bigram);

        if(bigram.size() < 3)
            continue;

        if(bigram[0].length() == 0 || bigram[1].length()==0 || bigram[2].length()==0)
            continue;


        if(bigram[0].length() > maxLevel || bigram[1].length() > maxLevel)
            continue;

        //allow bigrams for only some single character words like a,i,o,u,A,I
        if(bigram[0].length()==1){
            if(allowedLetters.find(bigram[0]) == string::npos )
                continue;
            //cout << "bigrams: " << bigram[0] << ", " << bigram[1] << ", " << bigram[2] << endl;
        }

        //cout << "bigrams: " << bigram[0] << ", " << bigram[1] << ", " << bigram[2] << endl;

        int score = atoi(bigram[2].c_str());

        if(linecount==2)
            maxScore = score;

        shared_ptr<cNode> cnode1, cnode2;
        bool isWord1Exist = false, isWord2Exist = false;
        t.getSetWordNode(bigram[0], isWord1Exist, cnode1);
        t.getSetWordNode(bigram[1], isWord2Exist, cnode2);

        /*if(bigram[0].length() == 1)
            cout << "bigrams: " << bigram[0] << ", " << bigram[1] << ", " << bigram[2] << endl;*/

        //check if bigram has to be added or not
        //add bigram only if currently there are lesss than 3 bigrams for cnode1 --bigrams are already sorted in the list

        if(cnode1->bigrams.size() < 3) {

            cnode1->bigrams.push_back(cnode2);
            cnode1->bigramWeights.push_back((unsigned char)(score * 255 / maxScore));
            trieBigrams.push_back(make_pair(cnode2, cnode1));

            if (nodesWithBigrams.empty())
                nodesWithBigrams.push_back(cnode1);
            //else if (find(nodesWithBigrams.begin(), nodesWithBigrams.end(), cnode1) - nodesWithBigrams.begin() >= nodesWithBigrams.size())
            else if(cnode1->bigrams.size()==1)
                nodesWithBigrams.push_back(cnode1);
        }
    }

    numBigrams = (int)nodesWithBigrams.size();
    cout << "no. of bigrams: " << numBigrams << endl;
}

void buildTrie(Trie &t){

    string unigramFile = "/media/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/unigrams.txt";
    string bigramFile = "/media/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/ngrams2.ll";
    string trigramFile = "/media/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/ngrams3.ll";

    addUnigrams(t, unigramFile);

    addBigrams(t, bigramFile);

}


//will run as background service
void serializeTrie( Trie &t, vector<trieNode> &trieNodeArray ){

    if(t.root->children.empty()){
        cout << "Trie is empty" << endl;
        return;
    }


    cout << "number of words in trie: " << trieWords.size() << endl;
    cout << "number of children: " << numChildren << endl;
    cout << "number of bigrams: " << trieBigrams.size() << endl;
    cout << "number of nodes with bigrams: " << nodesWithBigrams.size() << endl;
    //26 nodes in first layer
    //number of children per node is not fixed --> use some arbitrary length for the char array (say 24 x 1024 x 1024)

    //size of bytearray = number of nodes x

    unsigned short int posC = 0; //position of start of children
    unsigned short int posP = 0; //position of start of parent
    int l = 1;

    vector<vector<shared_ptr<cNode>>> nodes;
    nodes.push_back(vector<shared_ptr<cNode>>());
    nodes.push_back(vector<shared_ptr<cNode>>());

    unsigned short int totNodes = 0;

    for(int i = 0; i < t.root->children.size(); i++){
        nodes[0].push_back(t.root->children[i]);
        //cout << nodes[0][i]->value << ", ";
        totNodes++;
        if(!t.root->children[i]->children.empty()){
            for(int j = 0; j < t.root->children[i]->children.size(); j++) {
                nodes[1].push_back(t.root->children[i]->children[j]);
                totNodes++;
            }
        }
    }


    while(1){
        if(nodes[l].empty()) {
            nodes.pop_back();
            break;
        }
        //cout << endl;
        nodes.push_back(vector<shared_ptr<cNode>>());
        for(int i = 0; i < nodes[l].size(); i++){
            //cout << nodes[l][i]->value << ",";
            if(!nodes[l][i]->children.empty()){
                for(int j = 0; j < nodes[l][i]->children.size(); j++) {
                    nodes[l + 1].push_back(nodes[l][i]->children[j]);
                    totNodes++;
                }
            }
        }

        l++; //move to next level in tree
    }

    //cout << endl;

    //find positions of top words
    //cout << "top word positions: ";
    vector<unsigned short int> topWordsPos;
    for(int i = 0; i < topWordNodes.size(); i++){
        shared_ptr<cNode> tmpcNode = topWordNodes[i];
        int level = 0;
        while(tmpcNode->parent!=t.root){
            level++;
            tmpcNode = tmpcNode->parent;
        }

        unsigned short int pos = 0;

        for(int j = 0; j < level; j++)
            pos = pos + (unsigned short int)(nodes[j].size());

        pos += find(nodes[level].begin(), nodes[level].end(), topWordNodes[i])-nodes[level].begin();

        topWordsPos.push_back(pos);
        //cout << pos << ", ";
    }

    //cout << endl;

    vector<shared_ptr<cNode>> nodeVec = t.root->children;

    posC = (unsigned short int)(nodeVec.size());

    //cout << "initial child pos: " << posC << endl;

    int rowLevel = 0;

    shared_ptr<cNode> node = t.root;

    while(!nodeVec.empty()){
        vector<shared_ptr<cNode>> nextLevelNodes;

        //cout << "number of cnodes: " << nodeVec.size() << endl;
        //cout << "row level: " << rowLevel << endl;

        //cout << "nodeVec size: " << nodeVec.size() << endl;

        for(int i = 0; i < nodeVec.size(); i++){

            //cout << "i: " << i << endl;
            //when parent changes
            if(nodes[rowLevel][i]->parent != node){
                posP++;  //move to next parent node position with non-zero children
                while(trieNodeArray[posP-1].nChildren==0)
                    posP++;
                node = nodes[rowLevel][i]->parent;
                //cout << "new parent node value: " << node->value << endl;
            }

            if(!nodeVec[i]->children.empty()){
                for(int j = 0; j < nodeVec[i]->children.size(); j++)
                    nextLevelNodes.push_back(nodeVec[i]->children[j]);
            }

            //cout << "new trieNode: " << endl << "value: " << nodeVec[i]->value << " parent pos: " << posP << endl;
            trieNode newNode = newTrieNode(nodeVec[i], posP, posC);

            posC += nodeVec[i]->children.size();

            //vector<pair<unsigned short int, unsigned char>> bigramsPos(3);
            int chk = 0;

            /*if(rowLevel == 0){
                cout << "value of node: " << nodeVec[i]->value << endl;
                cout << "no of bigrams: " << nodeVec[i]->bigrams.size() << endl;
            }*/

            if(!nodeVec[i]->bigrams.empty()) {

                //cout << "bigrams found " << endl;
                //cout << "number of bigrams: " << nodeVec[i]->bigrams.size() << endl;

                for (int m = 0; m < nodeVec[i]->bigrams.size(); m++) {

                    //cout << "m: " << m << endl;

                    unsigned short int bigramPos = 0;
                    //unsigned char bigramWeight = ;
                    int level = 0;
                    shared_ptr<cNode> tmpcNode = nodeVec[i]->bigrams[m];
                    while (tmpcNode->parent != t.root) {
                        level++;
                        tmpcNode = tmpcNode->parent;
                    }

                    //cout << "level: " << level << endl;

                    for (int p = 0; p < level; p++) {
                        bigramPos += (unsigned short int) nodes[p].size();
                    }

                    bigramPos += (unsigned short int) (find(nodes[level].begin(), nodes[level].end(), nodeVec[i]->bigrams[m]) - nodes[level].begin());

                    //cout << "bigramPos: " << bigramPos << endl;

                    newNode.posBigrams[m] = bigramPos;
                    //cout << "i: " << i << endl;
                    newNode.weightsBigrams[m] = nodeVec[i]->bigramWeights[m];

                    if(chk==2)
                        break;
                    chk++;
                    //cout << "chk: " << chk << endl;
                }


//#if NOTDEBUG
                if(nodeVec[i]->bigrams.size() < 3){
                    for(int m = chk; m < 3; m++) {
                        newNode.posBigrams[m] = topWordsPos[m-chk];
                        newNode.weightsBigrams[m] = (unsigned char)0;
                    }
                }
//#endif
                //newNode.posBigrams = bigramsPos;
                //cout << "number of bigrams: " << bigrams << endl;
                //cout << "chk: " << chk << endl;
            }
            else{
                for(int m = 0; m < 3; m++){
                    newNode.posBigrams[m] = 0;
                    newNode.weightsBigrams[m] = 0;
                }
            }

            //newNode.posBigrams = bigramsPos;
            trieNodeArray.push_back(newNode);

            //cout << "pushed to array" << endl;
        }

        nodeVec.clear();
        nodeVec = nextLevelNodes;
        //cout << "number of nodes in new level: " << nextLevelNodes.size() << endl;
        //posP++;
        //node = nodes[rowLevel][0];
        rowLevel++;

        //posP++;

    }


    /*vector<vector<shared_ptr<cNode>>> nodes;
    nodes.push_back(vector<shared_ptr<cNode>>());
    nodes.push_back(vector<shared_ptr<cNode>>());

    unsigned short int totNodes = 0;

    for(int i = 0; i < t.root->children.size(); i++){
        nodes[0].push_back(t.root->children[i]);
        cout << nodes[0][i]->value << ", ";
        totNodes++;
        if(!t.root->children[i]->children.empty()){
            for(int j = 0; j < t.root->children[i]->children.size(); j++) {
                nodes[1].push_back(t.root->children[i]->children[j]);
                totNodes++;
            }
        }

    }

    cout << endl;


    while(1){
        if(nodes[l].empty()) {
            nodes.pop_back();
            break;
        }
        nodes.push_back(vector<shared_ptr<cNode>>());
        for(int i = 0; i < nodes[l].size(); i++){
            if(!nodes[l][i]->children.empty()){
                for(int j = 0; j < nodes[l][i]->children.size(); j++) {
                    nodes[l + 1].push_back(nodes[l][i]->children[j]);
                    totNodes++;
                }
            }
        }

        l++; //move to next level in tree
    }

    //print number of nodes in each level
    for(int i = 0; i < nodes.size(); i++){
        cout << "number of nodes in level-" << i << " are: " << nodes[i].size() << endl;
    }

    cout << "total number of nodes in trie: " << totNodes << endl;

    //initialize pointers to children and parents
    posC = 28;

    //find positions of top words
    vector<unsigned short int> topWordsPos;
    for(int i = 0; i < topWordNodes.size(); i++){
        shared_ptr<cNode> tmpcNode = topWordNodes[i];
        int level = 0;
        while(tmpcNode->parent!=t.root){
            level++;
            tmpcNode = tmpcNode->parent;
        }

        unsigned short int pos = 0;

        for(int j = 0; j < level; j++)
            pos = pos + nodes[j].size();

        pos += find(nodes[level].begin(), nodes[level].end(), topWordNodes[i])-nodes[level].begin();

        topWordsPos.push_back(pos);
    }

    cout << "top word nodes position: " << (int)topWordsPos[0] << ", " << (int)topWordsPos[1] << ", " << (int)topWordsPos[2] << endl;

    //add nodes to array
    for(int i = 0; i < nodes.size()-1; i++){  //nodes.size()-1
        cout << "number of nodes in this level: " << nodes[i].size() << endl;
        for(int j = 0; j < nodes[i].size(); j++){
            //for(int k = 0; k < nodes[i][j]->children.size(); k++){
                cout << "i: " << i << ", j: " << j << endl;
                trieNode newNode = newTrieNode(nodes[i][j], posP, posC);
                posC = posC + (unsigned short int)(nodes[i][j]->children.size()); //move pointer to next node child
                posP++; //move parent pointer to next node
                cout << "posC: " << posC << ", pospP: " << posP << endl;
                //bigrams
                vector<pair<unsigned short int, unsigned char>> bigramsPos(3);
                *//*if(!nodes[i][j]->bigrams.empty()) {

                    cout << "bigrams found " << endl;

                    int chk = 0;
                    for (int m = 0; m < nodes[i][j]->bigrams.size(); m++) {

                        unsigned short int bigramPos = 0;
                        unsigned char bigramWeight = nodes[i][j]->bigramWeights[m];
                        int level = 0;
                        shared_ptr<cNode> tmpcNode = nodes[i][j]->bigrams[m];
                        while (tmpcNode->parent != t.root) {
                            level++;
                            tmpcNode = tmpcNode->parent;
                        }

                        for (int p = 0; p < level; p++) {
                            bigramPos += (unsigned short int) nodes[p].size();
                        }

                        bigramPos += (unsigned short int) (find(nodes[level].begin(), nodes[level].end(), nodes[i][j]->bigrams[m]) - nodes[level].begin());

                        bigramsPos[m] = make_pair(bigramPos, bigramWeight);
                        if(chk==2)
                            break;
                        chk++;
                    }

                    cout << "number of bigrams: " << nodes[i][j]->bigrams.size() << endl;
                    cout << "chk: " << chk << endl;

                    if(nodes[i][j]->bigrams.size() < 3){
                        for(int m = chk; m < 3; m++) {
                            bigramsPos[m] = make_pair(topWordsPos[m-chk], (unsigned char)dummy);
                        }
                    }
                }*//*

                newNode.posBigrams = bigramsPos;

                trieNodeArray.push_back(newNode);

            cout << "node pushed to array" << endl;
            //}
        }
    }*/


    cout << "size of trieNodeArray: " << trieNodeArray.size() << endl;

    cout << "children of first parent: " << (int)trieNodeArray[0].nChildren << endl;

    cout << "Bigrams of the first node: " << endl;

    int chk = 0;

    for(int i = 0; i < trieNodeArray.size(); i++){
        if(trieNodeArray[i].weightsBigrams[0] != 0 && trieNodeArray[i].posBigrams[0] != 0){

            chk++;

            if(chk == chknum) {
                cout << "for original trie...." << endl;
                cout << "position of node with bigram: " << i << endl;
                cout << "value of node with bigrams: " << trieNodeArray[i].value << endl;
                cout << "position of node bigram: " << trieNodeArray[i].posBigrams[0] << endl;
                cout << "value of node bigram: " << trieNodeArray[trieNodeArray[i].posBigrams[0]].value << endl;
                cout << "size of nodes: " << nodes.size() << endl;
            }
            int level = 0;
            int pos = 0;

            for(int j = 0; j < nodes.size(); j++){
                //cout << "posj: " << pos << endl;

                level = j;
                if(pos + nodes[j].size() <= trieNodeArray[i].posBigrams[0])
                    pos += nodes[j].size();
                else
                    break;
            }

            //cout << "number of nodes in nodes: " << nodes[level].size() << endl;
            //chk++;

            //cout << "triNodeArray size: " << trieNodeArray.size() << endl;
            //cout << "level: " << level << endl;
            //cout << "pos: " << pos << endl;
            //pos += (trieNodeArray[i].posBigrams[0]-pos)+1;

            //shared_ptr<cNode> cnode = ;


            if(chk == chknum) {
                cout << "bigram word is: " << t.getWord(nodes[level][trieNodeArray[i].posBigrams[0] - pos]) << endl;

                break;
            }
        }
    }

    //**********************WRITE TO FILE***********************************//

    //serialize -- dump to file
    /*cout << "writing to file" << endl;
    ofstream os ("testTrie15.bin", ios::out | ios::binary);

    vector<trieNode>::size_type size1 = trieNodeArray.size();
    os.write((char*)&size1, sizeof(size1));
    os.write((char*)&trieNodeArray[0], trieNodeArray.size() * sizeof(trieNode));


    //os.write((const char*)&trieNodeArray, sizeof(trieNodeArray));
    os.close();

    cout << "dumped to file" << endl;*/

    //*********************************************************//


    /*cout << "print words of length 14: " << endl;

    int count = 0;
    for(int i = 0; i < nodes[14].size(); i++){
        if(find(trieBigrams.begin(), trieBigrams.end(), nodes[14][i])-trieBigrams.begin() < trieBigrams.size())
            count++;

        if(find(nodesWithBigrams.begin(), nodesWithBigrams.end(), nodes[14][i])-nodesWithBigrams.begin() < nodesWithBigrams.size())
            count++;

        cout << t.getWord(nodes[14][i]) << endl;

    }

    cout << "number of valid nodes of length 14: " << count << endl;*/

   // l = 0;

    /*//fill in to the char array
    while(1){

        if(l == nodes.size())
            break;

        charArray.push_back((int)nodes[l].size());

        for(int i = 0; i < nodes[l].size(); i++){
            charArray.push_back(nodes[l][i]->value);

            charArray.push_back(nodes[l][i]->children.size());

            if(!nodes[l][i]->children.empty()){
                for(int j = 0; j < nodes[l][i]->children.size(); j++)
                    charArray.push_back(-1);
            }


        }

    }*/




}

void deserializeTrie(Trie &t, const vector<trieNode> &trieNodeArray){

    cout << "deserializing trie ... " << endl;

    cout << "trieNodeArray size: " << trieNodeArray.size() << endl;

    shared_ptr<cNode> cnode = t.root;
    vector<vector<unsigned char>> numChildren;
    int parentPos = 0;
    int level = 0;
    int maxPosCurrentLevel = 28;  //28
    int maxNumChildren = 28;  //28
    int numNodesNextLevel = 0;
    int posCurrentLevel = 0;
    int childPosCurrentLevel = 0;

    vector<vector<shared_ptr<cNode>>> nodesVec;
    nodesVec.push_back(vector<shared_ptr<cNode>>());
    numChildren.push_back(vector<unsigned char>());

    vector<shared_ptr<cNode>> nodes;

    //fill trie with nodes read from file
    for(int i = 0; i < trieNodeArray.size(); i++){

        //cout << i << "th node value: " << trieNodeArray[i].value << endl;

        shared_ptr<cNode> currNode = make_shared<cNode>();

        currNode->value = trieNodeArray[i].value;

        //cout << "node value: " << currNode->value << endl;
        currNode->unigramWeight = trieNodeArray[i].unigramWeight;
        //cout << "chk" << endl;
        currNode->parent = cnode;

        cnode->children.push_back(currNode);

        nodesVec[level].push_back(currNode);
        nodes.push_back(currNode);
//#if NOTDEBUG
        if(i == trieNodeArray.size()-1) {
            cout << "chk" << endl;
            continue;
        }
//#endif
        //if(nodesVec[level].empty())
            //numChildren = number of children not updated


        numChildren[level].push_back(trieNodeArray[i].nChildren);
        //nodesVec[level].push_back(make_pair(currNode, trieNodeArray[i].nChildren));

        numNodesNextLevel += trieNodeArray[i].nChildren;

        posCurrentLevel++; //move to next node in current level
        childPosCurrentLevel++;

        //cout << "posCurrentLevel: " << posCurrentLevel << endl;
        //cout << "childPosCurrentLevel: " << childPosCurrentLevel << endl;

        //cout << "chk2" << endl;

        //cout << "number of nodes in next level: " << numNodesNextLevel << endl;

        //change of level
        if(posCurrentLevel == maxPosCurrentLevel){ //reached to first node in next level

            //int posVal = 0;

            //if(level==0){
            /*cout << "current pos: " << posCurrentLevel << ", max pos: " << maxPosCurrentLevel << endl;
            cout << "new parent pos: " << (int)(trieNodeArray[i+1].posParent)-1 << endl;
            cout << "new parent value: " << trieNodeArray[(int)(trieNodeArray[i+1].posParent)-1].value << endl;*/

            if(maxNumChildren==0)
                cout << "number of children of new parent: " << maxNumChildren << endl;  //number of children of a parent node is coming to be zero
            //cout << "p: " << i-maxPosCurrentLevel+parentPos << endl;
            //}

            parentPos = (int)(trieNodeArray[i+1].posParent)-1;
            maxNumChildren = (int)(trieNodeArray[(int)(trieNodeArray[i+1].posParent)-1].nChildren);

            /*cout << "maxNumChildren: " << maxNumChildren << endl;
            cout << "size of nodesVec: " << nodesVec[level].size() << endl;*/

            /*parentPos = 0;
             *while(trieNodeArray[i-maxPosCurrentLevel+parentPos+1].nChildren == 0)
                parentPos++;*/

            //maxNumChildren = trieNodeArray[i-maxPosCurrentLevel+parentPos+1].nChildren;//(int)(trieNodeArray[(int)(trieNodeArray[i+1].posParent)-1].nChildren);

            cnode = nodes[parentPos]; //move the parent node pointer to the first parent node of the previous level --- parentPos correspond to tnodeArray --- change to


            /*while(cnode->children.empty())  //check for number of children from
                cnode = nodesVec[level][posVal++];*/

            //(int)numChildren[level][0];
            level++;
            //cout << "new level: " << level << endl;
            nodesVec.push_back(vector<shared_ptr<cNode>>());
            numChildren.push_back(vector<unsigned char>());
            posCurrentLevel = 0;
            maxPosCurrentLevel = numNodesNextLevel;
            //cout << "number of nodes in next level: " << numNodesNextLevel << endl;
            numNodesNextLevel = 0;
            //parentPos = 0;
            childPosCurrentLevel = 0;

        }
        else{
            //cout << "chk21" << endl;
            //cout << "child position current level: " << childPosCurrentLevel << endl;
            //cout << "total number of children for current parent: " << (int)cnode->children.size() << endl;
            if(childPosCurrentLevel == maxNumChildren){ //all children of current parent are covered -- move to next parent
                //if(level == 1)

                parentPos = (int)(trieNodeArray[i+1].posParent)-1;
                //cout << "new parent pos: " << parentPos << endl;
                //cout << "new parent value: " << trieNodeArray[(int)(trieNodeArray[i+1].posParent)-1].value << endl;
                /*if(nodesVec[level].empty()){
                    cout << "empty nodesVec" << endl;
                    //break;
                }*/
                cnode = nodes[parentPos];
                //cnode = nodesVec[level-1][parentPos];
                childPosCurrentLevel = 0;
                maxNumChildren = (int)(trieNodeArray[(int)(trieNodeArray[i+1].posParent)-1].nChildren);
                //cout << "number of children of new parent: " << maxNumChildren << endl;
            }

            //cout << "chk22" << endl;
        }

        //cout << "chk33" << endl;

    }


    cout << "chk3" << endl;

    cout << "nodesvec ..." << endl;

    for(int i = 0; i < nodesVec.size(); i++) {
        for (int j = 0; j < nodesVec[i].size(); j++)
            cout << nodesVec[i][j]->value << ", ";
        cout << endl;
    }


    //maxPosCurrentLevel = (int)t.root->children.size();
    level = 0;
    posCurrentLevel = 0;
    int chk = 0;
    //fill bigrams
    for(int i = 0; i < trieNodeArray.size(); i++){

        //cout << "i: " << i << endl;

        if(trieNodeArray[i].posBigrams[0] == 0 && trieNodeArray[i].weightsBigrams[0] == 0) {
            //cout << "no bigrams found" << endl;
            continue;
        }

//#if ISDEBUG

//#endif
        for(int j = 0; j < 3; j++){
            int pos = 0;
            int l = 0;

            for(int m = 0; m < nodesVec.size(); m++){
                if(pos+nodesVec[m].size() <= trieNodeArray[i].posBigrams[j]){
                    pos += nodesVec[m].size();
                    l++;
                }
                else
                    break;
            }

            /*cout << "pos: " << pos << endl;
            cout << "posCurrentLevel: " << posCurrentLevel << endl;
            cout << "pos bigram: " << (int)(trieNodeArray[i].posBigrams[j]) << endl;
            cout << "size of nodesVec for level-" << l << " is: " << nodesVec[l].size() << endl;*/

            nodesVec[level][posCurrentLevel]->bigrams.push_back(nodesVec[l][trieNodeArray[i].posBigrams[j]-pos]);
            nodesVec[level][posCurrentLevel]->bigramWeights.push_back(trieNodeArray[i].weightsBigrams[j]);

            /*cout << "j: " << j << endl;
            cout << "i: " << i << endl;*/

        }


        chk++;

        if(chk == chknum){
            cout << "for deserialised trie.... " << endl;
            cout << "position of node with bigrams: " << i << endl;
            cout << "value of node with bigrams: " << trieNodeArray[i].value << endl;
            cout << "position of node bigram: " << trieNodeArray[i].posBigrams[0] << endl;
            cout << "value of node bigram: " << trieNodeArray[trieNodeArray[i].posBigrams[0]].value << endl;
            chk++;

            int l = 0;
            int pos = 0;

            for(int j = 0; j < nodesVec.size(); j++){

                if(pos + nodesVec[j].size() <= trieNodeArray[i].posBigrams[0]) {
                    pos += nodesVec[j].size();
                    l++;
                }
                else
                    break;
            }

            cout << "pos: " << pos << endl;
            //cout << "l: " << l << endl;

            cout << "size of nodesvec for level: " << l << " is " << nodesVec[l].size() << endl;
            //cout << "number of nodes in nodesvec: " << nodesVec[l].size() << endl;
            cout << "pos bigram: " << trieNodeArray[i].posBigrams[0] << endl;
            //pos += (trieNodeArray[i].posBigrams[0]-pos)+1;

            //shared_ptr<cNode> cnode = ;

            cout << "bigram word is: " << t.getWord(nodesVec[l][trieNodeArray[i].posBigrams[0] - pos]) << endl;

        }

        //cout << "chk5" << endl;

        posCurrentLevel++;

        if(posCurrentLevel == nodesVec[level].size()){
            cout << "level: " << level << endl;
            level++;
            posCurrentLevel = 0;
        }


    }

    cout << "level: " << level << endl;
    cout << "posCurrentLevel: " << posCurrentLevel << endl;
    cout << "size of nodesVec for level: " << nodesVec[level].size() << endl;


}

#define WRITE 1
#define NOTDEBUG 1
#define ISDEBUG 1

int main() {

#if WRITE
    //initialize trei
    Trie t;

    //use unigram, bigram and trigrams to build trie
    buildTrie(t);

    //testDryRun(t);

    cout << "Trie is filled with words " << endl;

    cout << "writing trie to file" << endl;

    //serialize
    vector<trieNode> trieNodeArray;
    serializeTrie(t, trieNodeArray);

    cout << "Done!" << endl;

#else
    cout << "loading trie from file" << endl;

    vector<trieNode> newTrieNodeArray;
    trieNode newtrieNode;

    ifstream is("testTrie15.bin", ios::in | ios::binary);


    vector<trieNode>::size_type size2 = 0;
    is.read((char*)&size2, sizeof(size2));
    newTrieNodeArray.resize(size2);
    is.read((char*)&newTrieNodeArray[0], newTrieNodeArray.size() * sizeof(trieNode));

    is.close();
    cout << "number of nodes in array read from file: " << newTrieNodeArray.size() << endl;

    cout << "creating trie from node array from file ..." << endl;
#endif

    Trie tNew;

    deserializeTrie(tNew, trieNodeArray);


    cout << endl << "********************************TESTING*********************************" << endl;

    cout << "For original trie" << endl;

    cout << "number of nodes under root: " << t.root->children.size() << endl;

    //cout << "trie deserialised .. " << endl;

    //print first layer of nodes
    for(int i = 0; i < t.root->children.size(); i++){
        cout << t.root->children[i]->value << ", ";
    }


    if(!t.root->children[0]->children.empty()){
        cout << "number of children of first node: " << t.root->children[0]->children.size() << endl;
        cout << "printing children: " << endl;

        for(int i = 0; i < t.root->children[0]->children.size(); i++){
            cout << t.root->children[0]->children[i]->value << ", ";
        }

    }
    else{
        cout << "no children found for the first node" << endl;
    }

    /*cout << "Printing original trie:" << endl;
    t.printTree();*/


    cout << endl << "For deserialised trie ------------" << endl;

    cout << "number of nodes under root: " << tNew.root->children.size() << endl;

    //cout << "trie deserialised .. " << endl;

    //print first layer of nodes
    for(int i = 0; i < tNew.root->children.size(); i++){
        cout << tNew.root->children[i]->value << ", ";
    }


    if(!tNew.root->children[0]->children.empty()){
        cout << "number of children of first node: " << tNew.root->children[0]->children.size() << endl;
        cout << "printing children: " << endl;

        for(int i = 0; i < tNew.root->children[0]->children.size(); i++){
            cout << tNew.root->children[0]->children[i]->value << ", ";
        }

    }
    else{
        cout << "no children found for the first node" << endl;
    }

    cout << endl;


    /*cout << "printing deserialised tree: " << endl;
    tNew.printTree();*/

    cout << "******************************************************" << endl;
    /*
    cout << "reading from first trieNode: " << endl;

    int p = 32;

    cout << "value: " << newTrieNodeArray[p].value << endl;
    cout << "parent: " << newTrieNodeArray[p].posParent << endl;

    if(newTrieNodeArray[p].posParent== 0)
        cout << "parent value: " << " root " << endl;
    else
        cout << "parent value: " << newTrieNodeArray[(int)newTrieNodeArray[p].posParent-1].value << endl;

    cout << "numChildren: " << (int)newTrieNodeArray[p].nChildren << endl;
    cout << "first bigram: " << newTrieNodeArray[p].posBigrams[0] << endl;
    cout << "weight of first bigram: " << (int)newTrieNodeArray[p].weightsBigrams[0] << endl;
    cout << "first bigram value: " << newTrieNodeArray[(int)(newTrieNodeArray[p].posBigrams[0])].value << endl;*/
//#endif
    /*if(!newTrieNodeArray[0].posBigrams.empty())
        cout << "numBigrams: " << newTrieNodeArray[0].posBigrams.size();
    else
        cout << "no bigrams" << endl;*/
    //use trie to make predictions for test chat
  //testDryRun(tNew);



    return 0;
}