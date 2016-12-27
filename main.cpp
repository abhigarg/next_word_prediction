
#include <assert.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>

#include "trie.h"


int maxLevel = 15;

//check if a character is a digit or not
bool isNum(const char c)
{
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


    //read from unigram file and add to trie
    FILE *uniFile = fopen(unigramFile.c_str(), "r");
    assert(uniFile);

    char line[256];
    int linecount = 0;
    int maxScore = 0;
    //cout << "Reading unigrams line by line" << endl;
    while(fgets(line, sizeof(line), uniFile)){
        //cout << "line: " << line << endl;
        string linestr(line);

        if(linestr.empty())
            continue;

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

        if(linecount==1)
            maxScore = score;

        //cout << "word: " << unigram[1] << " score: " << (int)(atoi(unigram[0].c_str())*255/maxScore) << endl;

        //add word to trie
        shared_ptr<cNode> cnode;
        bool isWordExist = false;

        //cout << "length of word: " << unigram[1].length() << endl;

        if(unigram[1].length() <= maxLevel) {
            t.getSetWordNode(unigram[1], isWordExist, cnode);
            cnode->unigramWeight = score;
            linecount++;
        }

        if(topWordNodes.size()<3)
            topWordNodes.push_back(cnode);

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

        //cout << "bigrams: " << bigram[0] << ", " << bigram[1] << ", " << bigram[2] << endl;

        int score = atoi(bigram[2].c_str());

        if(linecount==2)
            maxScore = score;

        shared_ptr<cNode> cnode1, cnode2;
        bool isWord1Exist = false, isWord2Exist = false;
        t.getSetWordNode(bigram[0], isWord1Exist, cnode1);
        t.getSetWordNode(bigram[1], isWord2Exist, cnode2);

        cnode1->bigrams.push_back(cnode2);
        cnode1->bigramWeights.push_back(score*255/maxScore);
        trieBigrams.push_back(make_pair(cnode2, cnode1));

        //break;

        if(nodesWithBigrams.empty())
            nodesWithBigrams.push_back(cnode1);
        else
            if(find(nodesWithBigrams.begin(), nodesWithBigrams.end(), cnode1)-nodesWithBigrams.begin() >= nodesWithBigrams.size())
                nodesWithBigrams.push_back(cnode1);

    }

    numBigrams = (int)nodesWithBigrams.size();
    cout << "no of bigrams: " << numBigrams << endl;
}

void buildTrie(Trie &t){

    string unigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/unigrams.txt";
    string bigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/ngrams2.ll";
    string trigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/testTrie/ngrams3.ll";

    addUnigrams(t, unigramFile);

    addBigrams(t, bigramFile);

}


//will run as background service
void serializeTrie( const Trie &t ){

    if(t.root->children.empty()){
        cout << "Trie is empty" << endl;
        return;
    }

    vector<trieNode> trieNodeArrays;

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


    vector<shared_ptr<cNode>> nodeVec = t.root->children;

    posC = nodeVec.size();

    while(!nodeVec.empty()){
        vector<shared_ptr<cNode>> nextLevelNodes;

        //cout << "number of cnodes: " << nodeVec.size() << endl;

        for(int i = 0; i < nodeVec.size(); i++){

            //cout << "i: " << i << endl;

            for(int j = 0; j < nodeVec[i]->children.size(); j++)
                nextLevelNodes.push_back(nodeVec[i]->children[j]);

            trieNode newNode = newTrieNode(nodeVec[i], posP, posC);
            posP++;
            posC += nodeVec[i]->children.size();

            vector<pair<unsigned short int, unsigned char>> bigramsPos(3);
            int chk = 0;
            if(!nodeVec[i]->bigrams.empty()) {

                //cout << "bigrams found " << endl;
                //cout << "number of bigrams: " << nodeVec[i]->bigrams.size() << endl;

                for (int m = 0; m < nodeVec[i]->bigrams.size(); m++) {

                    unsigned short int bigramPos = 0;
                    unsigned char bigramWeight = nodeVec[i]->bigramWeights[m];
                    int level = 0;
                    shared_ptr<cNode> tmpcNode = nodeVec[i]->bigrams[m];
                    while (tmpcNode->parent != t.root) {
                        level++;
                        tmpcNode = tmpcNode->parent;
                    }

                    for (int p = 0; p < level; p++) {
                        bigramPos += (unsigned short int) nodes[p].size();
                    }

                    bigramPos += (unsigned short int) (find(nodes[level].begin(), nodes[level].end(), nodeVec[i]->bigrams[m]) - nodes[level].begin());

                    bigramsPos[m] = make_pair(bigramPos, bigramWeight);
                    if(chk==2)
                        break;
                    chk++;
                }

                //cout << "number of bigrams: " << bigrams << endl;
                //cout << "chk: " << chk << endl;
            }


            if(nodeVec[i]->bigrams.size() < 3){
                for(int m = chk; m < 3; m++) {
                    bigramsPos[m] = make_pair(topWordsPos[m-chk], (unsigned char)dummy);
                }
            }

            newNode.posBigrams = bigramsPos;
            trieNodeArrays.push_back(newNode);

            //cout << "pushed to array" << endl;
        }

        nodeVec.clear();
        nodeVec = nextLevelNodes;

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

                trieNodeArrays.push_back(newNode);

            cout << "node pushed to array" << endl;
            //}
        }
    }*/


    cout << "size of trieNodeArrays: " << trieNodeArrays.size() << endl;


    //serialize -- dump to file
    cout << "writing to file" << endl;
    ofstream os ("testTrie15.bin", ios::binary);

    vector<trieNode>::size_type size1 = trieNodeArrays.size();
    os.write((char*)&size1, sizeof(size1));
    os.write((char*)&trieNodeArrays[0], trieNodeArrays.size() * sizeof(trieNode));


    //os.write((const char*)&trieNodeArrays, sizeof(trieNodeArrays));
    os.close();

    cout << "dumped to file" << endl;

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


int main() {

    //initialize trei
    Trie t;

    //use unigram, bigram and trigrams to build trie
    buildTrie(t);

    cout << "Trie is filled with words " << endl;

    //serialize
    //serializeTrie(t);

    cout << "writing trie to file" << endl;

    //use trie to make predictions for test chat
    testDryRun(t);



    return 0;
}