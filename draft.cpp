#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>


// All commented code will be related  to the lines after the comment

using namespace std;

// Each file and directory is represented as a node.
struct node {
    string fname;
    int fsize;
    vector<node *> children;
    struct node *parent;
    bool isDirectory;
};


// Different structures depending on 
struct node *newdirectory(string name) {
    struct node *tmp = new struct node(); 
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

// Could not exactly implement Singleton, so had to utilize these global nodes.
struct node *root = newdirectory("root");
struct node *curr = root; 

// Function that returns a node that has the same name as the string passed.
node *traverseTree (node *root, string traverseString) {
    queue < node *> myqueue;
    myqueue.push(root);
    while (!myqueue.empty()) {
        int temp = myqueue.size();
        while ( temp > 0 ) {
            node *tmp = myqueue.front();
            myqueue.pop();
            if (tmp->fname != traverseString){
                for (int i =0; i < tmp->children.size(); ++i) {
                    if (tmp->children.at(i)->fname == traverseString) {
                        return tmp;
                    }
                }
            } 
            for (int i =0; i < tmp->children.size(); ++i) {
                myqueue.push(tmp->children.at(i));
            }
            --temp;
        }
    }
}

// My builder class
class scriptParser { 
    public:
        int size;
        virtual void extractString(string name){}
        virtual void makeDirectory(string name){}
        virtual void createFile(string name, int filesize){}
        virtual void changeDirectory(string name){}

        /* Replaced the following functions with the Visitor implentation

        virtual void deletefd(string name){}
        virtual void fdsize(string name){}
        virtual void list(string name){}
        
        */
        
        virtual void accept (class Visitor*) = 0;
    protected: 
};

// Builder derived class, one file system.

class fileSystem: public scriptParser {
    public:
        void accept (Visitor*);

        // Creates a directory with string given in command line.
        void makeDirectory(string name){
            (curr->children).push_back(newdirectory(name));
            if (curr == root) {
                for (int i = 0; i < curr->children.size(); ++i) {
                    curr->children.at(i)->parent = root;
                }
            }
            cout << name << " has been added." << endl;
        }

        //Creates a file instead of a directory, pushes back a file size.
        void createFile(string name, int filesize){
            (curr->children).push_back(newfile(name, filesize));
        }

        // Purpose of my curr global object is to keep track of what directory the user is in.
        void changeDirectory(string name) { 
            
            // Going to the directory that is given by the user ..
            if (name != "..") {
                for (int i =0; i < curr->children.size(); ++i) {
                    if (curr->children.at(i)->fname == name) {
                        curr->children.at(i)->parent = curr;
                        curr = curr->children.at(i);
                        cout << "Your directory has been changed to: " << curr->fname << endl;  
                        break;
                    }
                }
            }
            
            // Catches the case of going back to the previous directory ".."
            else { 
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
    protected:
};

// Deletes a file or directory using Visitor & Proxy implementation.
class deletefd : public Visitor { 
    public:
        string fdname;

        //Keeps track of all the files the user would like to delete.
        //Does this because if the filesystem waits to delete files until the user
        //passes <exit>, the filesystem needs to keep track of all the files.
        vector <string> filesToDelete;
        void addFilestoDelete (string &myFiles) {
            filesToDelete.push_back(myFiles);
            cout << myFiles <<endl;
        }
        void visit (fileSystem*) {
            for (int i =0; i < filesToDelete.size(); ++i) {
                node *myPtr = traverseTree(root,filesToDelete.at(i));
                for (int j=0; j< myPtr->children.size(); ++j) {
                    if (myPtr->children.at(j)->fname == filesToDelete.at(i)) {
                        cout << "Found string to delete: " << myPtr->children.at(j)->fname << endl;
                        myPtr->children.erase(myPtr->children.begin() + j);
                    }
                }
            }  
        }
};

// Size implementation using Visitor
class fdsize : public Visitor {
    public: 
        string fdname;
        void visit (fileSystem*) {
            for (int i =0; i < curr->children.size(); ++i) {

                // If its a file and its in the current directory.
                if (curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == false) {
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }

                else if (curr->children.at(i)->isDirectory == false) { 
                    cout << curr->children.at(i)->fname << " Size: " << curr->children.at(i)->fsize << endl;
                }

                //If it is a directory, then it will also list all of its children and size(s), as asked.
                else if ((curr->children.at(i)->fname == fdname && curr->children.at(i)->isDirectory == true) || fdname == "root") {
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

// Resize implemented with Visitor pattern. Attempted to also use Observer pattern, but 
// it is not properly implemented. Will resize any given file.
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

/* 
   My proxy pattern implementation currently uses myExit as a wrapper class 
   FOR the deletetion operation class. The user will think they are deleting files
   but the deletetion of files do not occur until myExit is called.
*/
class myExit : public Visitor {
    public:
        deletefd deleteOperations;
        scriptParser *builder;
        vector <string> testing;

        //Pushes the strings to delete and will pass it into the deleteOperation vector of strings.
        void addFilestoDelete (string stringtest) {
            deleteOperations.filesToDelete.push_back(stringtest);
        }

        // Once exit is finally called, that is when it will add the strings to the deleteOperation.
        // Delete operation is NOT called UNTIL we get the user to EXIT.
        void visit(fileSystem*) {
            for (int i = 0; i < testing.size(); ++i) {
                addFilestoDelete(testing.at(i));
            }
            builder->accept(&deleteOperations);
        }
};

void fileSystem::accept(Visitor *v) {
    v->visit(this);
}

// Target class for adapter pattern. 

class TreeDisplay {
    public:

    /* 
        Displays the tree using  a queue, so level-order. There is NO special formatting other than all files 
        that are in the same directory will be on the same line. For example:

        ROOT
        CSCI 250 CSCI 150
        PROJECT 1 SIZE: 20, PROJECT 2 SIZE: 20
        PROJECT 1 SIZE: 10

        The second PROJECT 1 is CSCI 150's Project 1. It will print AFTER all subdirectories and files of CSCI 250 finish printing.
        The first "PROJECT 1 SIZE: 20"  && PROJECT 2: SIZE 20 are files in CSCI250. I hope this is okay.
    */
        void display () {
            queue < node *> myqueue;
            myqueue.push(root);
            while (!myqueue.empty()) {
                int temp = myqueue.size();
                while ( temp > 0 ) {
                    node *tmp = myqueue.front();
                    myqueue.pop();
                    if (tmp->isDirectory) {
                        cout << tmp-> fname << " ";
                    }
                    else {
                        cout << tmp->fname << " " << "size: " << tmp->fsize << " ";
                    }
                    for (int i =0; i < tmp->children.size(); ++i) {
                        myqueue.push(tmp->children.at(i));
                    }
                    --temp;
                }
                cout << endl;
            }
        }
};

// Adapter, derived from target and contains obj of adaptee
class DirectoryTreeDisplay : public TreeDisplay, private fileSystem{
    public:
    virtual void adaptdisplay (){
        display();
    }
    virtual ~DirectoryTreeDisplay() {}
    virtual void displayDecorator() =0;
};

/* 
    The next 3 class are my Decorator implementations. As asked,
    there is a HEADER and FOOTER that is printed BEFORE and AFTER 
    the user calls <display>. I hope this is what you meant when 
    you asked for a header and footer. Decorator is only shown when
    the user passes <display>. 
*/
class testing: public DirectoryTreeDisplay {
    private:
        DirectoryTreeDisplay *testptr;
    public: 
        testing(DirectoryTreeDisplay *test) {  
            testptr = test;
        }
        ~testing() {
            delete testptr;
        }
        void displayDecorator() {
            testptr->displayDecorator();
        }
};

class treeHeader: public DirectoryTreeDisplay {
    public:
        void displayDecorator() {
            cout<< endl << "Your artifical file system: " << endl << endl;
            adaptdisplay();
        }
};

class theFooter: public testing {
    public:
        theFooter(DirectoryTreeDisplay *myTree) :testing(myTree){} 
        void displayDecorator() {
            testing::displayDecorator();
            cout<< endl << "This is the end of your file system. :~) " << endl;
        }
};

// My attempt at the observer pattern. Not quite there.
// Attempts to update when resize is called.
class myObserver {
    public:
        virtual void update () = 0;
};

class resizeObserver : public myObserver {
        DirectoryTreeDisplay *displayfunction;
    public:
        resizeObserver(DirectoryTreeDisplay *displayresize) {
            displayfunction = displayresize;
        }
        void update() {
            displayfunction->display();
        }
};

/* 
    My directory class from the builder implentation. Most of the magic of the filesystem
    is done through this class. I hope that this is okay to do... 

    I also attempted to encapsulate Composite methods into the builder class.
    I hope this was the way to use the composite pattern with the builder pattern.
*/
class Director { 
       scriptParser *builder;
    public: 
        deletefd deleteOperation;
        fdsize sizeOperation;
        list lsOperation;
        resize resizeOperation;
        myExit exitOperation;
        vector <string> userFiles;
        void setBuilder (scriptParser *newBuilder) {
            builder = newBuilder;
            ifstream myfile;
            string line;
            stringstream substring;
            myfile.open("script1.txt");

            if (myfile) {
            
                cout<< "File is being read." << endl;
                while (getline(myfile,line)){
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
                        exitOperation.testing.push_back(tempname);
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
                        DirectoryTreeDisplay *file;
                        resizeObserver resizedisplay(file);
                        builder->accept(&resizeOperation);
                        resizedisplay.update();
                    }

                    else if (command == "exit") {
                        cout << "Exit command was passed" << endl;
                        exitOperation.builder = newBuilder;
                        builder->accept(&exitOperation);
                    }

                    else if (command == "display") {
                        DirectoryTreeDisplay *start = new theFooter(new treeHeader);
                        start->displayDecorator();
                    }
                }
            myfile.close();
            }
        }
};

int main() {
    
    // Director control
    Director dir;

    //Concrete Builder 
    fileSystem myfiles;

    dir.setBuilder(&myfiles);

    return 0;
}
