#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>


using namespace std;

vector<string> wordList;
vector<string> parentWords;
vector<size_t> parentPos;

int dummy = 0; //to store dummy weights

vector<vector<vector<size_t>>> childPosScore;  //parent1: child1,child2..,
vector<vector<int>> scores;  //parent1: score_chain1, score_chain2, ..

void readFromFile(const string filename){};

bool isDebug = true;

size_t findWordinList(vector<string> list, string word){

    return find(list.begin(), list.end(), word) - list.begin();

}

bool compare(const pair<int, int>&i, const pair<int, int>&j)
{
    return i.second > j.second;
}

bool ifEqual(vector<string> v1, vector<string> v2){
    bool is_equal = false;

    if(v1.size() == v2.size())
        is_equal = std::equal(v1.begin(), v1.end(), v2.begin());

    return is_equal;
}

void printParents(){

    cout << "parents are: " << endl;

    for(int i = 0; i < parentWords.size()-1; i++)
        cout << parentWords[i] << ", ";

    cout << parentWords[parentWords.size()-1] << endl;

}

void printPredictions(vector<string> words){

    if(!words.empty()){
        cout << "predictions: ";

        for(int i = 0;i < words.size()-1; i++){
            if(words[i].length() > 0)
                cout << words[i] << ", ";
        }

        cout << words[words.size()-1] << endl << endl;
    }

}


void addWords(const vector<string> words, int score){  //max word length = 3

    score = min(255, max(dummy, score));

    if(isDebug)
        cout << "score: " << score << endl;

    size_t parent_pos;

    if(!parentWords.empty()){
        parent_pos = findWordinList(parentWords, words[0]);

        if(parent_pos >= parentWords.size()) { //head does not exist in parents

            size_t wordListPos = find(wordList.begin(), wordList.end(), words[0]) - wordList.begin();

            if(wordListPos >= wordList.size()){
                if(isDebug)
                    cout << "added new word to list: " << words[0] << endl;
                wordList.push_back(words[0]);
            }

            parentWords.push_back(words[0]);
            parentPos.push_back(wordListPos);
            //parent_pos = parentPos.size() - 1;
            /*if(words.size() > 1)
                childPosScore.push_back(vector<vector<size_t>>());*/
            if(isDebug)
                cout << "added new parent: " << words[0] << endl;
        }
        else
            if(isDebug)
                cout << "word: (" << words[0] << ") found in parent at position: " << parent_pos << endl;
        if(isDebug)
            cout << "parent size: " << parentWords.size() << endl;
    }
    else{
        parentWords.push_back(words[0]);
        wordList.push_back(words[0]);
        parentPos.push_back(wordList.size() - 1);
        parent_pos = 0;
        if(isDebug)
            cout << "added parent: " << words[0] << endl;
    }

    if(words.size() > 1){
        vector<size_t> child_pos;
        size_t pos;

        if(childPosScore.empty()){
            for(int i = 0; i < parentPos.size(); i++){
                childPosScore.push_back(vector<vector<size_t>>());
                scores.push_back(vector<int>());
            }
        }

        if(childPosScore[parent_pos].empty())
            childPosScore[parent_pos].push_back(vector<size_t>()); //add new gram
        //scores[parent_pos].push_back();

        for(size_t i = 1; i < words.size(); i++){

            pos = find(wordList.begin(), wordList.end(), words[i]) - wordList.begin();

            if(pos >= wordList.size()){ //word do not exist in wordList add to the list

                wordList.push_back(words[i]);

                if(isDebug)
                    cout << "added to word list: " << words[i] << endl;
            }

            childPosScore[parent_pos][childPosScore[parent_pos].size()-1].push_back(pos);

            if(isDebug)
                cout << "added child: " << words[i] << endl;
        }

        scores[parent_pos].push_back(score);

        if(!parentWords.empty()){
            cout << "----------------" << endl;
            printParents();
            cout << "----------------" << endl;
        }
    }


    //cout << "added words: " << words << endl;

}


vector<pair<size_t, int>> get_predictions(const vector<string> words){  //results: wordpos1,score1, wordpos2,score2 ..

    vector<pair<size_t, int>> results;

    //find if head is a parent if not then if it exists in word list if not then add it to the word list and return empty result
    //if head is present in the word list but is not a parent then make it a parent if there are children available
    size_t parent_pos = findWordinList(parentWords, words[0]);

    if(parent_pos >= parentWords.size()){
        if(isDebug)
            cout << "word not found in parent list .. checking in word list" << endl;
        size_t wordListPos = findWordinList(wordList, words[0]);
        if( wordListPos >= wordList.size()){
            if(isDebug)
                cout << "word not found in word list ... adding to word list" << endl;
            if(words.size()>1)
                addWords(words, dummy);
            else{
                wordList.push_back(words[0]);
                if(isDebug)
                    cout << "added new word to list: " << words[0] << endl;
            }

        }
        else{ //add to parent list if children exist in words and followed by chain of children
            if(words.size() > 1) {

                if (isDebug)
                    cout << "new parent found: " << words[0] << endl;

                parentWords.push_back(words[0]);
                parentPos.push_back(wordListPos);
                childPosScore.push_back(vector<vector<size_t>>());
                scores.push_back(vector<int>());
                if (isDebug)
                    cout << "added new parent to parent list at " << parent_pos << endl;

                //add children chain
                if (isDebug)
                    cout << "adding children at: " << childPosScore.size()-1 << endl;

                childPosScore[childPosScore.size() - 1].push_back(vector<size_t>());
                for (int i = 1; i < words.size(); i++) {
                    wordListPos = findWordinList(wordList, words[i]);
                    if (wordListPos == wordList.size())
                        wordList.push_back(words[i]);
                    childPosScore[childPosScore.size() - 1][0].push_back(wordListPos);

                    if (isDebug)
                        cout << "added child: " << words[i] << endl;
                }
                scores[childPosScore.size() - 1].push_back(dummy); //dummy score
            }
        }

        return results;
    }

    /*if(childPosScore.size() <= parent_pos) {
        if(isDebug)
            cout << "no children for parent: " << words[0] << endl;
        childPosScore.push_back(vector<vector<size_t>>());
        if(words.size() > 1){
            childPosScore[parent_pos].push_back(vector<size_t>());
            for(int i = 1; i < words.size(); i++){
                size_t wordListPos = findWordinList(wordList, words[i]);
                //check if word is present in the word list
                if(wordListPos < wordList.size())
                    childPosScore[parent_pos][0].push_back(wordListPos);
                else{
                    wordList.push_back(words[i]);
                    childPosScore[parent_pos][0].push_back(wordListPos);
                }
                scores[parent_pos].push_back(dummy);
                if(isDebug)
                    cout << "added new child: " << words[i] << endl;
            }
        }

        return results;
    }*/

    cout << "number of parents: " << parentWords.size() << endl;
    cout << "parent_pos: " << parent_pos << endl;

    if(isDebug)
        cout << "number of children-chain for parent ( " << words[0] << " ) is: " << childPosScore[parent_pos].size() << endl;

    for(int i = 0; i < childPosScore[parent_pos].size(); i++){

        if(childPosScore[parent_pos][i].empty()){
            if(isDebug)
                cout << "empty children array for parent: (" << words[0] << ") at position: " << i << endl;
            continue;
        }

        if(words.size()==1){
            if(isDebug)
                cout << "giving immediate children for only one word: " << words[0] << endl;

            results.push_back(make_pair(childPosScore[parent_pos][i][0], scores[parent_pos][i])); //word position
            //results.push_back(childPosScore[parent_pos][i][childPosScore[parent_pos][i].size()-1]); //score
            continue;
        }

        //cout << "words size: " << words.size() << endl;
        //cout << "children chain size: " << childPosScore[parent_pos][i].size() << endl;

        if(words.size()-1 >= childPosScore[parent_pos][i].size())
            continue;

        string childWord;
        //cout << "number of children in chain: " << childPosScore[parent_pos][i].size() << endl;
        //cout << "number of words in query: " << words.size()-1 << endl;
        for(int j = 0; j < childPosScore[parent_pos][i].size(); j++){
            //cout << "j: " << j << endl;
            if( j == words.size()-1 ){
                //cout << "result found" << endl;
                results.push_back(make_pair(childPosScore[parent_pos][i][j+1],scores[parent_pos][j])); //word position
                //results.push_back((size_t)()); //score
                break;
            }

            childWord = wordList[childPosScore[parent_pos][i][j]];
            if(strcmp(childWord.c_str(), words[j+1].c_str()) == 0) {
                //cout << "match found" << endl;
                if(j == words.size()-2 && childPosScore[parent_pos][i].size() == words.size()-1){
                    scores[parent_pos][i]++;
                    return results;
                }

                continue;
            }
            else
                break;

        }

        //cout << "done" << endl;
        /*if(equal(childWords.begin(), childWords.end()-2, words.begin())){
            results.push_back(childPosScore[parent_pos][i][childPosScore[parent_pos][i].size()-2]);
            results.push_back(childPosScore[parent_pos][i][childPosScore[parent_pos][i].size()-1]);
        }*/


        /*if(childPosScore[parent_pos][i].size()-2 == words.size()){
            vector<string> childWords;
            for(int j = 0; j < childPosScore[parent_pos][i].size()-2; j++)
                childWords.push_back(wordList[childPosScore[parent_pos][i][j]]);

            if(equal(childWords.begin(), childWords.end()-2, words.begin())){
                results.push_back(childPosScore[parent_pos][i][childPosScore[parent_pos][i].size()-2]);
                results.push_back(childPosScore[parent_pos][i][childPosScore[parent_pos][i].size()-1]);
            }

        }*/
    }

    if(results.empty()){
        //add children if available
        if(words.size() > 1){
            childPosScore[parent_pos].push_back(vector<size_t>());
            for(int i = 1; i < words.size(); i++){
                size_t wordListPos = findWordinList(wordList, words[i]);
                //check if word is present in the word list
                if(wordListPos < wordList.size())
                    childPosScore[parent_pos][childPosScore[parent_pos].size()-1].push_back(wordListPos);
                else{
                    wordList.push_back(words[i]);
                    childPosScore[parent_pos][childPosScore[parent_pos].size()-1].push_back(wordListPos);
                }
            }

            scores[parent_pos].push_back(dummy);
        }
    }


    return results;
}


void queryFor(vector<string> query, vector<string> &output){

    if(isDebug){
        cout << "querying for: ";

        for(int i = 0; i < query.size()-1;i++)
            cout << query[i] << ", ";

        cout << query[query.size()-1] << endl;
    }

    vector<pair<size_t, int>> results = get_predictions(query);

    if(results.empty()) {
        if (isDebug)
            cout << "no results found" << endl;
    }

    else{

        sort(results.begin(), results.end(), compare);

        for(int i = 0; i < 5; i++) {
            //cout << "wordList size: " << wordList.size() << endl;
            //cout << "result id: " << results[count] << endl;
            //cout << wordList[results[count]] << ", " << results[count+1] << endl;
            output.push_back(wordList[results[i].first]);
        }
    }


}


void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

   /* //cout << "query for: ";
    for(int i = 0; i < elems.size()-1; i++)
        cout << elems[i] << ", " << endl;

    cout << elems[elems.size()-1] << endl;*/

}


int main() {
    /*std::cout << "Hello, World!" << std::endl;
    return 0;*/

    vector<string> sentence1, sentence2, sentence3;
    vector<string>query1,query2;

    sentence1.push_back("how");
    sentence1.push_back("are");
    sentence1.push_back("you");

    sentence2.push_back("how");
    sentence2.push_back("much");

    sentence3.push_back("are");
    sentence3.push_back("you");

    query1.push_back("what");
    query2.push_back("what");
    query2.push_back("are");

    cout << "adding words .." << endl;

    addWords(sentence1, 100);
    addWords(sentence2, 50);
    addWords(sentence3, 30);

    cout << "added words!" << endl;

    cout << "************************" << endl;

    vector<string> output;

    cout << "******testing-1*********" << endl<< endl;

    queryFor(query1, output);
    printPredictions(output);
    output.clear();
    cout << "******testing-2*********" << endl<< endl;

    queryFor(query2, output);
    printPredictions(output);
    output.clear();
    cout << "******testing-3*********" << endl<< endl;

    queryFor(query1, output);
    printPredictions(output);
    output.clear();

    cout << "*****************Dry-Run**************" << endl << endl;

    vector<string> user_input;
    isDebug = false;

    int count = 0;

    while(1){
        string curr_input = "";
        cout << "user input: ";
        getline(cin, curr_input);

        if(curr_input.length() == 0) {
            //cout << "1" << endl;
            if (count >= 3)
                break;
            count++;
            user_input.clear();
        }
        else{
            if(count > 0)
                count--;
            split(curr_input, ' ', user_input); //split by space

            if(user_input.size() > 3)
                while(user_input.size() >3)
                    user_input.erase(user_input.begin());

            vector<string> out;

            if(user_input.size() == 3){
                queryFor(user_input, out);
                vector<string> tmp1;
                tmp1.push_back(user_input[1]);
                tmp1.push_back(user_input[2]);
                queryFor(tmp1, out);
                vector<string> tmp2;
                tmp2.push_back(user_input[2]);
                queryFor(tmp2, out);

            }

            if(user_input.size() == 2){
                queryFor(user_input, out);
                vector<string> tmp;
                tmp.push_back(user_input[1]);
                queryFor(tmp, out);
            }

            if(user_input.size()==1){
                queryFor(user_input, out);
            }

            printPredictions(out);
            out.clear();

        }

    }

    //readFromFile
}