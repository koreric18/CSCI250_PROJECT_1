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

struct node *root = newdirectory("root");
node *curr = root; // current pointer to track where user is at

class scriptParser { // builder
    public:
        int size;
        virtual void extractString(string name){}
        virtual void makeDirectory(string name){}
        virtual void createFile(string name, int filesize){}
        virtual void changeDirectory(string name){}
        /* Replacing with visitor implementation...
        virtual void deletefd(string name){}
        virtual void fdsize(string name){}
        virtual void list(string name){}*/
        virtual void accept (class Visitor*) = 0;
    protected: 
};

class fileSystem: public scriptParser {
    public:
        void accept (Visitor*);
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
};

class Visitor {
    public:
        virtual void visit(fileSystem*) = 0;
};

class deletefd : public Visitor { // TO DO: Delete file AND cascade delete 
    public:
        string fdname;
        void visit (fileSystem*) {
            cout<< "hi you're passing fdname: " << fdname << endl;
        // delete fd algorithm??
            for (int i =0; i < curr->children.size(); ++i) {
                if (curr->children.at(i)->fname == fdname) {
                    curr->children.erase(curr->children.begin() + i);
                }
            }
            for (int i =0; i < curr->children.size(); ++i) {
                cout << curr->children.at(i)->fname << endl;
            }
        }
};

class fdsize : public Visitor {
    public: 
        string fdname;
        void visit (fileSystem*) {
            for (int i =0; i < curr->children.size(); ++i) {
                if (curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == false) {
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }
                else if (curr->children.at(i)->isDirectory == false) { // catching files in directories that are not in other directories
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }
                else if ((curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == true) || fdname == "root") {
                    cout << "hello" <<endl;
                    for (int j = 0; j < curr->children.at(i)->children.size(); ++j) {
                        if (curr->children.at(i)->children.at(j)->isDirectory == true) {
                            cout << curr->children.at(i)->children.at(j)->fname << endl;
                        }
                        else if (curr->children.at(i)->children.at(j)->isDirectory == false) {
                            cout << curr->children.at(i)->children.at(j) ->fname << " size: " << curr->children.at(i)->children.at(j)->fsize << endl;
                        }
                    }
                }
            }
        }
};

class list : public Visitor {
    public:
        string fdname;
        void visit(fileSystem*) {
            for (int i =0; i < curr->children.size(); ++i) { 
                // first case - will print name of a file and its size.
                if (curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == false) {
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }
                // if no argument is given
                else if (fdname == ""){
                    // list all files in the current directory
                    if (curr->children.at(i)->isDirectory == false) { 
                        cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                    }
                    // lists all directories in the current directory
                    else if (curr->children.at(i)->isDirectory == true) { 
                        cout << curr->children.at(i)->fname << endl;
                    }
                }
                // list all files with the given directory
                else if (curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == true) {
                    for (int j = 0; j < curr->children.at(i)->children.size(); ++j) {
                        if (curr->children.at(i)->children.at(j)->isDirectory == true) {
                            cout << "Directory: " << curr->children.at(i)->children.at(j)->fname << " ";
                        }
                        else if (curr->children.at(i)->children.at(j)->isDirectory == false) {
                            cout << "File : " << curr->children.at(i)->children.at(j) ->fname << " size: " << curr->children.at(i)->children.at(j)->fsize;
                        }
                        cout << endl;
                    }
                }
            }
    }
};

class resize : public Visitor {
    public:
        string fdname;
        int newsize;
        void visit(fileSystem*) {
            for (int i =0; i < curr->children.size(); ++i) { 
                if (curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == false) {
                    curr->children.at(i)->fsize = newsize;
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }
            }
        }

};
/* My proxy pattern implementation currently uses myExit as a wrapper class 
   FOR the deletetion operation class. The user will think they are deleting files
   but the deletetion of files do not occur until myExit is called.

*/
class myExit : public Visitor {
    resize *deleteFiles;
    public:
        void visit(fileSystem*) {
            // pretend to jump out...del doesnt occur until user uses exit command
            // current idea: keep all nodes in a container and delete them when exit is called
        }
};

void fileSystem::accept(Visitor *v) {
    v->visit(this);
}

class Director { //director
       scriptParser *builder;
    public: 
        deletefd deleteOperation;
        fdsize sizeOperation;
        list lsOperation;
        resize resizeOperation;
        myExit exitOperation;
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
                        deleteOperation.fdname=tempname;
                        builder->accept(&deleteOperation);
                    }
                    else if (command == "size") {
                        sizeOperation.fdname = tempname;
                        builder->accept(&sizeOperation);
                    }
                    else if (command == "ls") {
                        lsOperation.fdname = tempname;
                        builder->accept(&lsOperation);
                    }
                    else if (command == "resize") {
                        substring >>filesize;
                        resizeOperation.newsize = filesize;
                        resizeOperation.fdname = tempname;
                        builder->accept(&resizeOperation);
                    }
                    else if (command == "exit") {

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
