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
    bool isDirectory;
};

struct node *newdirectory(string name) {
    struct node *tmp = new struct node(); // declaring and allocating new file
    tmp -> fname = name; 
    tmp -> parent = NULL;
    tmp -> isDirectory = true;
    return (tmp);
}

struct node *newfile(string name, int size) {
    struct node *tmp = new struct node();
    tmp -> fname = name;
    tmp -> fsize = size;
    tmp -> isDirectory = false;
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
                    curr->children.at(i)->parent = root;
                }
            }
            cout << name << " has been added." << endl;
        }
        void createFile(string name, int filesize){
            (curr->children).push_back(newfile(name, filesize));
            /*for (int i = 0; i < curr->children.size(); ++i) {
                cout <<curr->children.at(i)->fname << " size: " << curr->children.at(i)-> fsize << endl;
            }*/

        }
        void changeDirectory(string name) { //
            // second case, cd ..., set curr to a child
            cout<< "you're attempting to change into: " << name << endl;
            if (name != "") {
                for (int i =0; i < curr->children.size(); ++i) {
                    if (curr->children.at(i)->fname == name) {
                        curr->children.at(i)->parent = curr;
                        curr = curr->children.at(i);
                        cout << "Your directory has been changed to: " << curr->fname << endl;  
                        break;
                    }
                }
            }
            else { // going back to parents directory,
                    if (curr != root) {
                        curr = curr->parent;
                        cout << "Your directory has been changed back to: " << curr->fname << endl;
                    }
                    else {
                        cout << "You're already in the root directory!" << endl;
                    }   
                }                
        }     
        void deletefd(string name) { // currently deletes file or directory if its in the path
            for (int i =0; i < curr->children.size(); ++i) {
                if (curr->children.at(i)->fname == name) {
                    curr->children.erase(curr->children.begin() + i);
                }
            }
            for (int i =0; i < curr->children.size(); ++i) {
                cout << curr->children.at(i)->fname << endl;
            }
        }
        void fdsize(string name) { // still need to do for size of all files
            for (int i =0; i < curr->children.size(); ++i) {
                if (curr->children.at(i)->fname == name && curr->children.at(i)->isDirectory == false) {
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize;
                }
                else if ((curr->children.at(i)->fname == name && curr->children.at(i)->isDirectory == true) || name == "root") {
                    cout << "hello" <<endl;
                    for (int j = 0; j < curr->children.at(i)->children.size(); ++j) {
                        if (curr->children.at(i)->children.at(j)->isDirectory == true) {
                            cout << curr->children.at(i)->children.at(j)->fname << endl;
                        }
                        else if (curr->children.at(i)->children.at(j)->isDirectory != true) {
                            cout << curr->children.at(i)->children.at(j) ->fname << " size: " << curr->children.at(i)->children.at(j)->fsize << endl;
                        }
                    }
                }
            }
        }
/*        void list(string name) { 
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
            myfile.open("script1.txt");
            if (myfile) {
                cout<< "File is being read." << endl;
                while (getline(myfile,line)){//.eof()) {
                    substring.clear();
                    string command, tempname;
                    int filesize = 0;
                    substring << line;
                    substring >> command >> tempname;
                    if (command == "mkdir") {
                        builder->makeDirectory(tempname);
                    }
                    else if (command == "create") {
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
    return 0;
}
