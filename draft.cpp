#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// My parser will retrieve the commands and constructs object using Builder interface
struct node {
    string fname;
    int size;
    vector<node *> children;
};

node *newnode(string name) {
    node *tmp = new node;
    tmp -> fname = name; 
    return tmp;
}

class scriptParser { // builder
    public:
        int size;
        virtual void extractString(string name){}
        virtual void makeDirectory(string name){}
        virtual void createFile(string name){} // passes the command line
        virtual void changeDirectory(string name){}
        virtual void deletefd(string name){}
        virtual void fdsize(string name){}
        virtual void list(string name){}
    protected: 
    node *parent;
    node *root; // root node of the file system
    node *curr = root; // current pointer to track where user is at
};

class fileSystem: public scriptParser {
    public:
        void makeDirectory(string name){
        //    stringstream substring;
        //    substring << name; // storing whole string into ss
        //    string temp, tempname;
         //   substring >> temp >> tempname;
            cout << "hello";
            (root->children).push_back(newnode(name));
        }
/*        void createFile(string name){
            stringstream substring;
            substring << name; // storing whole string into ss
            string temp, tempname;
            int mysize;   
            substring >> temp >> tempname >> mysize;
            (curr->children).push_back(newnode(tempname));
            curr->size = mysize;
        }
        void changeDirectory(string name) { // curr pointer should only be changed here?
            // two cases
            stringstream substring;
            string temp, tempname;
            tempname = "";
            substring << name; // storing whole string into ss
            substring >> temp >> tempname;
            // second case, cd ..., set curr to a child
            if (tempname != "") {
                for (int i = 0; i < curr->children.size(); ++i) {
                    if (curr->children.at(i)->fname == tempname) {
                        parent = curr;
                        curr->children.at(i);
                    }
                }
            }
            else { // first case, cd, set the curr to the parent node.
                curr = parent; 
            }
        }
        void deletefd(string name) { // currently deletes file or directory if its in the path
            stringstream substring;
            string temp, tempname;
            substring << name;
            substring >> temp >> tempname;
            for (int i =0; i < parent->children.size(); ++i) {
                if (parent->children.at(i)->fname == tempname) {
                    parent->children.erase(parent->children.begin() + i);
                }
            }
        }
        void fdsize(string name) { // still need to do for size of all files
            stringstream substring;
            string temp, tempname;
            substring << name;
            substring >> temp >> tempname;
            for (int i =0; i < parent->children.size(); ++i) {
                if (parent->children.at(i)->fname == tempname) {
                    //if (parent->children.at(i).size() > 0) {
                    //    for (parent->children)
                    //}
                    cout << "Size: " << parent->children.at(i)->size;
                }
            }

        }
        /*traversing the vector of nodes...
        while (tempptr->name != fname) {
            for (int i =0; i < root->children.size(); ++i) {

            }
        }
        void list(string name) {
            stringstream substring;
            string temp, tempname;
            tempname = "";
            substring << name;
            substring >> temp >> tempname;  
            if (tempname == "") {
                for (int i = 0; i < parent->children.size(); ++i){
                    cout << parent->children.at(i)->fname << "size: " << parent->children.at(i)->size << endl;
                }
            }  
        }*/
};

class Director { //director
       scriptParser builder;
    public: 
        void setBuilder (scriptParser newBuilder) {
            builder = newBuilder;
        }
        void parse();     
};

void Director:: parse() {
    ifstream myfile;
    string line;
    stringstream substring;
    string command, tempname;
    myfile.open("script1.txt");
    if (myfile) {
        cout<< "yellow";
        while (getline(myfile,line)){//.eof()) {
            substring << line;
            substring >> command >> tempname;
            if (command == "mkdir") {
                cout << tempname << endl;
                builder.makeDirectory(tempname);
            }
        /*    else if (line.find("create")) {
                builder->createFile(line);
            }
            else if (line.find("cd")) {
                builder->changeDirectory(line);
            }
            else if (line.find("del")) {
                builder->deletefd(line);
            }
            else if (line.find("size")) {
                builder->fdsize(line);
            }
            else if (line.find("ls")) {
                builder->list(line);
            }*/
        }
        myfile.close();
    }
}

int main() {
//singleton, should access instance of class through its pointer and not the actual instance
    
    // Director control
    Director dir;

    //Concrete Builder 
    fileSystem myfiles;

    dir.setBuilder(myfiles);
    dir.parse();


    //Build my file system...
    cout << "Your directory " << endl;
    return 0;
}
