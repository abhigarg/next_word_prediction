
#include "trie.h"


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

        cout << "added: " << cnode->value << endl;
        cout << "parent: " << cnode->parent->value << endl;
    }

    cout << "checking for added word" << endl;

    bool isWordExist = false;
    std::shared_ptr<cNode> wordNode = t.root;
    t.getSetWordNode("hello", isWordExist, wordNode);

    cout << "leaf node char: " << wordNode->value << endl;

   /* cout << "parent of leaf node char: " << (int)wordNode->parent->value << endl;

    wordNode = wordNode->parent;

    cout << "parent of parent of leaf node char: " << (int)wordNode->parent->value << endl;*/

    //cout << "word entered in trie: " << t.getWord(wordNode) << endl;

    return 0;
}