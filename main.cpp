
#include "trie.h"


bool isLegal(char c)
{
    char legal[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','I' /*..and so on*/};
    int len = sizeof(legal)/sizeof(char);

    for (int i = 0; i < len; i++)
        if (c == legal[i])
            return true;
    return false;
}

bool compare(const pair<string,int> &a,const pair<string,int> &b)
{
    return a.second > b.second;
}


void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {

        elems.push_back(item);
    }

}


void testDryRun(Trie t){

    cout << endl << "*****************Dry-Run**************" << endl << endl;


    t.printTree();

    vector<string> user_input;
    isDebug = false;

    int count = 0;

    while(1){
        string curr_input = "";
        cout << "user: ";
        getline(cin, curr_input);

        if(curr_input.length() == 0) {
            //cout << "1" << endl;
            if(count == 2)
                t.printTree();
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


void makeTree(Trie &t){

    string unigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/unigrams.txt";
    string bigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/ngrams2.ll";
    string trigramFile = "/media/abu/DATA/fromUbuntu/Bobble/keyboard/next_word_prediction/mastodon/newdata/output/ngrams3.ll";


    //read from unigram file and add

}


int main() {

    //initialize tree
    Trie t;
    string word = "hello";

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
        cout << "predictions for hello: " << results[i].first << endl;



   /* cout << "parent of leaf node char: " << (int)wordNode->parent->value << endl;

    wordNode = wordNode->parent;

    cout << "parent of parent of leaf node char: " << (int)wordNode->parent->value << endl;*/

    //cout << "word entered in trie: " << t.getWord(wordNode) << endl;

    return 0;
}