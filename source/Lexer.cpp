#include "../includes/Lexer.h"
#include <cctype>
#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
using namespace std;
using namespace icu;
namespace fs = filesystem;
Lexer::Lexer(string dir,bool filterExtensions, size_t max_mem){
    this->max_mem = max_mem;
    for (const auto & entry : fs::recursive_directory_iterator(dir))
        if(!entry.is_directory() && ( regex_match(entry.path().u8string(),regex( "^.*\.(html|md|txt)$" ) ) || !filterExtensions))
            this->files.push_back(entry.path().u8string() );
    
    if(files.empty()) throw invalid_argument("Directory is empty!");
    printFiles();
}

void Lexer::addToken(string& currentToken,map<string,int>& tokens){
    // TODO: add checks for how much memory is being used.
    if(currentToken.size() > 1000){
        cout << "Token too big ( > 1000 ) skipping it." << endl;
        currentToken = "";
    }
    else if(!currentToken.empty()){

        UnicodeString ucs = UnicodeString::fromUTF8(StringPiece(currentToken.c_str())).toLower();
        string lowered_token;
        ucs.toUTF8String(lowered_token);
        tokens[lowered_token]++;
        //cout << "Lowered: " << lowered_token << endl;
        lowered_token = "";
    }
    currentToken = "";
}
// Tokenizes the first file in the vector and pops it.
// Returns the map of tokens and amount of their occurances 
map<string,int> Lexer::lex(){
    if(files.empty()) throw out_of_range("There is no file to lex!");
    map<string,int> tokens = {};
    ifstream  file(files.back()); 
    this->lastFile = files.back();
    files.pop_back();
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    char current;
    string currentToken;
    for(const auto st : buffer.str()){
        if(isspace(st)){
            addToken(currentToken,tokens);
            continue;
        }
        else if(isalnum(st)){
            currentToken += st;
        }
        else if(string(".,\n").find(st) != string::npos){
            continue;
        }
        else if(string("'").find(st) != string::npos){
            addToken(currentToken,tokens);
            // "" = " IN SQLITE!
            currentToken+= st;
            currentToken+= st;
            continue;
        }
        else if(string("-\"@#%&*()_+<>:;|?!{}[]\\/`~^=").find(st) != string::npos){
            addToken(currentToken,tokens);
            currentToken+= st;
            // "" = " IN SQLITE!
            if(string("\"").find(st) != string::npos) currentToken+="\"";
            addToken(currentToken,tokens);
            continue;
        }
        else currentToken+=st;

    }
    return tokens;
}


vector<string> Lexer::getFiles(){
    return this->files;
}
string Lexer::getLastFile(){
    return this->lastFile;
}
void Lexer::printFiles(){
    cout << "There are: " << files.size() << " files." << endl;;
    for(const string file : this->files)
        cout << file << endl;
}


