#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>


using namespace std;

vector<string> wordList;  //list of all words
vector<string> parentWords; //list of all parents
vector<size_t> parentPos; //list of parents positions only

int dummy = 0; //to store dummy weights

vector<vector<vector<size_t>>> ngrams;  //parent1: child1,child2..,
vector<vector<int>> scores;  //parent1: score_chain1, score_chain2, ..

void readFromFile(const string filename){};

bool isDebug = true;

template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

void LOG(string msg){
    if(isDebug)
        cout << msg << endl;
}

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

    for(int i = 0; i < parentPos.size()-1; i++)
        cout << wordList[parentPos[i]] << ", ";

    cout << wordList[parentPos[parentPos.size()-1]] << endl;

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


void checkAndAddWord(string word){

    //add first word to list
    if(wordList.empty())
        wordList.push_back(word);

     //if word is not in list
    else{
        if(findWordinList(wordList, word) >= wordList.size())
            wordList.push_back(word);
    }

}


void addWords(vector<string> words, int score){

    vector<size_t> ngram;
    size_t pos;

    //add first entry to parents
    if(parentWords.empty()){
        size_t parent_pos = findWordinList(wordList, words[0]);
        if(parent_pos >= wordList.size())
            wordList.push_back(words[0]);
        parentPos.push_back(parent_pos);
        parentWords.push_back(words[0]);

        ngrams.push_back(vector<vector<size_t>>());
        scores.push_back(vector<int>());
        for(int i = 1; i < words.size(); i++){
            pos = findWordinList(wordList, words[i]);
            if(pos == wordList.size())
                wordList.push_back(words[i]);
            ngram.push_back(pos);
        }

        ngrams[0].push_back(ngram);
        scores[0].push_back(score);
    }
    else{
        /// add parent if not in parent list
        size_t parent_pos = findWordinList(parentWords, words[0]);

        if(parent_pos == parentWords.size()){
            pos = findWordinList(wordList, words[0]);
            if(pos >= wordList.size())
                wordList.push_back(words[0]);
            parentPos.push_back(pos);
            parentWords.push_back(words[0]);

            // add to ngrams
            ngrams.push_back(vector<vector<size_t>>());
            scores.push_back(vector<int>());
            for(int i = 1; i < words.size(); i++){

                pos = findWordinList(wordList, words[i]);
                if(pos >= wordList.size())
                    wordList.push_back(words[i]);

                ngram.push_back(pos);

            }

            ngrams[ngrams.size()-1].push_back(ngram);
            scores[scores.size()-1].push_back(score);
        }
        else{  //parent exists already

            pos = findWordinList(parentWords, words[0]);

            // add to ngrams
            for(int i = 1; i < words.size(); i++){

                pos = findWordinList(wordList, words[i]);
                if(pos >= wordList.size())
                    wordList.push_back(words[i]);

                ngram.push_back(pos);

            }

            ngrams[pos].push_back(ngram);
            scores[pos].push_back(score);
        }
    }

}


vector<pair<size_t, int>> get_predictions(vector<string> words){

    vector<pair<size_t, int>> result;

    //only one word or multiple words
    //parent does not exist in parent list or parent does not exist in main word list


    //if only ony word --> check if exists in parents --> return immediate children from all ngrams --> sort by score and return to user
    //                 --> if not --> check if it exists in wordlist --> if not then add it and return nothing
    //

    //multiple words --> check if parent exist in parent list --> if not then check if it exists in word list --> if not then add it to word list then add to parent list --> add remaining words as child chain
    //                                                        --> if not but exist in word list --> add first word to parent list --> add remaining words to child chain
    //                                                        --> if exist in parent list --> check all current child-chains are of longer length than remaining words --> return last item


    //if wordList is empty then just fill in the word
    if(wordList.empty())
        wordList.push_back(words[0]);

    size_t parent_pos, pos;

    if(words.size()==1){ //only one word
        LOG("only one word..");

        parent_pos = findWordinList(parentWords, words[0]);
        if(parent_pos >= parentWords.size()){  //add to wordlist if not found

            LOG("word not a parent..");

            pos = findWordinList(wordList, words[0]);
            if(pos >= wordList.size()) {
                wordList.push_back(words[0]);

                LOG("word added to word list: ");
                LOG(words[0]);
            }
            return result;
        }
        else{  //found in parent list  --> return immediate children
            parent_pos = findWordinList(parentWords, words[0]);
            LOG("word is a parent");
            LOG("return children");
            vector<size_t> entries;
            for(int i = 0; i < ngrams[parent_pos].size(); i++) {

                cout << "child: " << ngrams[parent_pos][i][0] << endl;
                cout << "score: " << scores[parent_pos][i] << endl;
                if(entries.empty()) {
                    result.push_back(make_pair(ngrams[parent_pos][i][0], scores[parent_pos][i]));
                    entries.push_back(ngrams[parent_pos][i][0]);
                }
                else{
                    if(find(entries.begin(), entries.end(), ngrams[parent_pos][i][0])-entries.begin() >= entries.size()){  //new entry
                        result.push_back(make_pair(ngrams[parent_pos][i][0], scores[parent_pos][i]));
                        entries.push_back(ngrams[parent_pos][i][0]);
                    }
                    
                }

            }
            return result;
        }

    }

    else{ //multiple words
        LOG("multiple words");
        //check if parent exists in parent list
        parent_pos = findWordinList(parentWords, words[0]);
        if(parent_pos >= parentWords.size()){  //add to wordlist if not found

            LOG("word not found in parents");

            pos = findWordinList(wordList, words[0]);
            LOG("position of word is: ");
            cout << pos << endl;
            if(pos >= wordList.size()) {
                wordList.push_back(words[0]);

                LOG("word added to word list");
            }

            parentWords.push_back(words[0]);
            //LOG("check 1");
            parentPos.push_back(pos);

            //LOG("check 2");

            ngrams.push_back(vector<vector<size_t>>());

            LOG("check 3");
            LOG("scores size: ");
            LOG(to_string(scores.size()));

            scores.push_back(vector<int>());
            scores[pos].push_back(dummy);

            LOG("word added as parent: ");
            LOG(words[0]);

            vector<size_t> ngram;

            for(int i = 1; i < words.size(); i++){

                pos = findWordinList(wordList, words[i]);

                if(pos >= wordList.size())
                    wordList.push_back(words[i]);
                ngram.push_back(pos);
            }

            ngrams[parent_pos].push_back(ngram);

            LOG("new ngram added from input words");

            return result;
        }
        else{ //return next word from child chain if exists otherwise add a new chain to parent
            LOG("word found in parents");

            bool addNewChildChain = true;

            for(int i = 0; i < ngrams[parent_pos].size(); i++){

                LOG("checking in child-chain: ");
                LOG(to_string(i));

                if(words.size()-1 > ngrams[parent_pos][i].size()) //more words then children in chain --> skip
                    continue;

                for(int j = 0; j < ngrams[parent_pos][i].size(); j++){
                    LOG("j: ");
                    LOG(to_string(j));

                    if(strcmp(words[j + 1].c_str(), wordList[ngrams[parent_pos][i][j]].c_str())==0){
                        LOG("word matching");

                        if(j == words.size()-2){ //if all words matches
                            LOG("all words same..");
                            LOG("no of child in ngram: ");
                            LOG(to_string(ngrams[parent_pos][i].size()));
                            //same set of words repeated --> increase the score for the set of words
                            if(ngrams[parent_pos][i].size() == words.size()-1){
                                if(scores[parent_pos][i] != 255)
                                    scores[parent_pos][i]++;
                                LOG("score updated");
                                addNewChildChain = false;
                                break;
                            }

                            result.push_back(make_pair(ngrams[parent_pos][i][j], scores[parent_pos][i]));
                            break;
                        }

                        continue;
                    }
                    else{
                        LOG("word not matching");
                        break; //children chain does not matches with the input words completely -- skip
                    }

                }

            }

            //add child-chain to parent in case no matches are found for the input words
            if(result.empty() && addNewChildChain){
                vector<size_t> ngram;
                for(int i = 1; i < words.size(); i++){
                    ngram.push_back(findWordinList(wordList, words[i]));
                }

                ngrams[parent_pos].push_back(ngram);
                scores[parent_pos].push_back(dummy);
                LOG("new child-chain added");
            }

            return result;

        }

    }

};


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


        int count = 0;

        for(int i = 0; i < results.size(); i++) {

            if(count == 5)
                break;
            count++;
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

    /*vector<string> sentence1, sentence2, sentence3;
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
    output.clear();*/

    cout << "*****************Dry-Run**************" << endl << endl;

    vector<string> user_input;
    isDebug = true;

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

            LOG("querying ... ");

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