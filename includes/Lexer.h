#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

enum fileTypes{Markdown};

class Lexer{
private:
    vector<string> files = {};
    size_t max_mem;
    size_t used_mem;
    void addToken(string& currentToken,map<string,int>& tokens);
    string lastFile;
public:
    Lexer(string dir,bool filterExtensions, size_t max_ram = 1024 * 1024 * 100);
    //after each file is done pass tokens to function
    map<string,int> lex();
    void printFiles();
    vector<string> getFiles();
    string getLastFile();
};

