#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// My parser will retrieve the commands and constructs object using Builder interface
struct node {
    string fname;
    int fsize;
    vector<node *> children;
    struct node *parent;
};

struct node *newdirectory(string name) {
    struct node *tmp = new struct node(); // declaring and allocating new file
    tmp -> fname = name; 
    tmp -> parent = NULL;
    return (tmp);
}

struct node *newfile(string name, int size) {
    struct node *tmp = new struct node();
    tmp -> fname = name;
    tmp -> fsize = size;
    return (tmp);
}
class scriptParser { // builder
    public:
        int size;
        virtual void extractString(string name){}
        virtual void makeDirectory(string name){}
        virtual void createFile(string name, int filesize){}
        virtual void changeDirectory(string name){}
        virtual void deletefd(string name){}
        virtual void fdsize(string name){}
        virtual void list(string name){}
        struct node *root = newdirectory("root");
    protected: 
    node *curr = root; // current pointer to track where user is at
};

class fileSystem: public scriptParser {
    public:
        void makeDirectory(string name){
            (curr->children).push_back(newdirectory(name));
            if (curr == root) {
                for (int i = 0; i < curr->children.size(); ++i) {
                    curr->children.at
                }
            }
        }
        void createFile(string name, int filesize){
            (curr->children).push_back(newfile(name, filesize));
            for (int i = 0; i < curr->children.size(); ++i) {
                cout <<curr->children.at(i)->fname << " size: " << curr->children.at(i)-> fsize << endl;
            }

        }
        void changeDirectory(string name) { 
            // second case, cd ..., set curr to a child
            if (name != "") {
                for (int i =0; i < curr->children.size(); ++i) {
                    if (curr->children.at(i)->fname == name) {
                        curr->children.at(i);
                        curr->children.at(i)->parent = curr;
                    }
                }
            }
            else { // going back to parents directory
                for (int i =0; i < curr->children.size(); ++i) {
                    if (curr->children.at(i)->fsize == 0) {
                        curr = curr->children.at(i)->parent;
                    }
                };
                
                // set parent to grandparent
                
            }
        }
/*      CSCI           PHYSICS
        150 250         200
        2020
        void deletefd(string name) { // currently deletes file or directory if its in the path
            for (int i =0; i < parent->children.size(); ++i) {
                if (parent->children.at(i)->fname == name) {
                    parent->children.erase(parent->children.begin() + i);
                }
            }
        }
        void fdsize(string name) { // still need to do for size of all files
            for (int i =0; i < parent->children.size(); ++i) {
                if (parent->children.at(i)->fname == name) {
                    //if (parent->children.at(i).size() > 0) {
                    //    for (parent->children)
                    //}
                    cout << "Size: " << parent->children.at(i)->size;
                }
            }
        }
        void list(string name) { 
            if (name == "") {
                for (int i = 0; i < parent->children.size(); ++i){
                    cout << parent->children.at(i)->fname << "size: " << parent->children.at(i)->size << endl;
                }
            }  
        }*/
};

class Director { //director
       scriptParser *builder;
    public: 
        void setBuilder (scriptParser *newBuilder) {
            builder = newBuilder;
            ifstream myfile;
            string line;
            stringstream substring;
            string command, tempname;
            myfile.open("script1.txt");
            if (myfile) {
                cout<< "File is being read." << endl;
                while (getline(myfile,line)){//.eof()) {
                    substring.clear();
                    substring << line;
                    substring >> command >> tempname;
                    if (command == "mkdir") {
                        builder->makeDirectory(tempname);
                    }
                    else if (command == "create") {
                        int filesize;
                        substring >> filesize;
                        builder->createFile(tempname, filesize);
                    }
                    else if (command == "cd") {
                        builder->changeDirectory(tempname);
                    }
                    else if (command == "del") {
                        builder->deletefd(tempname);
                    }
                    else if (command == "size") {
                        builder->fdsize(tempname);
                    }
                    else if (line.find("ls")) {
                        builder->list(tempname);
                    }
                }
            myfile.close();
            }
        }
};

int main() {
//singleton, should access instance of class through its pointer and not the actual instance
    
    // Director control
    Director dir;

    //Concrete Builder 
    fileSystem myfiles;

    dir.setBuilder(&myfiles);

    //Build my file system...
    cout << "Your directory " << endl;
    return 0;
}
