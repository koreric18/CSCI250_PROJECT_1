#include <iostream>

using namespace std;

// My parser will retrieve the commands and constructs object using Builder interface
class parseInput { //director
    public: 
        void setBuilder () {

        }
        void parse();     
    }
    */
    private:
        Builder *builder; 
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
    virtual void makeDirectory(dirname);
    virtual void createFile(filname, fsize);
    virtual void changeDirectory(dirname);
    virtual void deletefd(fdname);
    virtual void size(fdname);
    virtual void list(fdname);
};

class fileSystem: public scriptParser {
    public:
        fileSystem() {

        }
        void makeDirectory(dirname){

        }
        void createFile(filname, fsize){

        }
        void changeDirectory(dirname) {

        }
        void deletefd(fdname) {

        }
        void size(fdname) {

        }
        void list(fdname) {

        }
} 
int main() {
//singleton, should access instance of class through its pointer and not the actual instance
}
