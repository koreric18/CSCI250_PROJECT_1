#include <iostream>
#include <string>
#include <vector>

using namespace std;

// My parser will retrieve the commands and constructs object using Builder interface
class parseInput { //director
    private:
        scriptParser* builder;
    public: 
        void setBuilder (scriptParser* newBuilder) {
            builder = newBuilder;
        }
        void parse();     
    }
};

void parseInput:: parse() {
    ifstream myfile;
    myfile.open("script1.txt");
    string line;
    while (!getline(myfile,line).eof()) {
        if (line.find("mkdir")) {
            scriptParser.makeDirectory(line);
        }
        else if (line.find("create")) {
            scriptParser.createFile(line);
        }
        else if (line.find("cd")) {
            scriptParser.changeDirectory(line);
        }
        else if (line.find("del")) {
            scriptParser.deletefd(line);
        }
        else if (line.find("size")) {
            scriptParser.size(line);
        }
        else if (line.find("ls")) {
            scriptParser.ls(line);
        }
    }
}

class scriptParser { // builder
    string name;
    int size;
    virtual void makeDirectory(name);
    virtual void createFile(name, size);
    virtual void changeDirectory(name);
    virtual void deletefd(name);
    virtual void size(name);
    virtual void list(name);
};

class fileSystem: public scriptParser {
    public:
        /*fileSystem() {

        }*/
        void makeDirectory(name){
            (root->children).push_back(newNode(name));
        }
        /*void createFile(name, size){

        }
        void changeDirectory(name) {

        }
        void deletefd(name) {

        }
        void size(name) {

        }
        void list(name) {

        }*/
} 
struct node {
    string name;
    vector<node *> children;
    struct node *root;
};

node *newnode(string name) {
    node *tmp = new node;
    tmp -> key = key; 
    return tmp;
} 

int main() {
//singleton, should access instance of class through its pointer and not the actual instance
    parseInput parser;
    parser.parse();
}
