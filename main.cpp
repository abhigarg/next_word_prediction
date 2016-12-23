
#include <assert.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */

#include "trie.h"


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
        if(word.length()>0)
            elems.push_back(word);
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
        t.getSetWordNode(unigram[1], isWordExist, cnode);
        cnode->unigramWeight = score;
        linecount++;
        //cout << "added word: " << unigram[1] << endl;
        //cout << "word count: " << linecount << endl;
    }

    //cout << "words in trie: " << endl;
    //t.printTree();

    cout << "total number of words: " << t.trieWords.size() << endl;
}


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

        //break;

    }

    cout << "no of bigrams: " << linecount << endl;

}

void makeTree(Trie &t){

    string unigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/unigrams.txt";
    string bigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/ngrams2.ll";
    string trigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/ngrams3.ll";

    addUnigrams(t, unigramFile);

    addBigrams(t, bigramFile);

}


int main() {

    //initialize tree
    Trie t;
    /*string word = "hello";

    std::shared_ptr<cNode> parentNode = t.root;

    for(int i = 0; i < word.length(); i++){
        std::shared_ptr<cNode> cnode = std::make_shared<cNode>();
        cnode->value = word[i];
        cnode->parent = parentNode;
        parentNode->children.push_back(cnode);
        parentNode = cnode;

        //cout << "added: " << cnode->value << endl;
        //cout << "parent: " << cnode->parent->value << endl;
    }
    t.trieWords.push_back(parentNode);
    //cout << "checking for added word" << endl;

    bool isWordExist = false;
    std::shared_ptr<cNode> wordNode = t.root;
    t.getSetWordNode("hello", isWordExist, wordNode);

    //cout << "leaf node char: " << wordNode->value << endl;

    vector<pair<string, int>> results;
    if(!t.getTrigram("hello", "world", results))
        t.getBigram("hello", results);

    for(int i = 0; i < results.size(); i++)
        cout << "predictions for hello: " << results[i].first << endl;*/

    makeTree(t);

    cout << "Tree is filled with words: " << endl;

    testDryRun(t);

   /* cout << "parent of leaf node char: " << (int)wordNode->parent->value << endl;

    wordNode = wordNode->parent;

    cout << "parent of parent of leaf node char: " << (int)wordNode->parent->value << endl;*/

    //cout << "word entered in trie: " << t.getWord(wordNode) << endl;

    return 0;
}