#include "includes/Lexer.h"
#include "includes/DbHandler.h"
#include <exception>
#include <iostream>
#include <algorithm>
#include <fmt/format.h>
#include <chrono>
#include <SQLiteCpp/SQLiteCpp.h>
using namespace std;
typedef chrono::high_resolution_clock::time_point TimeVar;
#define timeNow() chrono::high_resolution_clock::now()
#define duration(t2,t1) chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
#ifdef SQLITECPP_ENABLE_ASSERT_HANDLER
namespace SQLite
{
/// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    // Print a message to the standard error output stream, and abort the program.
    std::cerr << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
    std::abort();
}
}
#endif



int main(int argc, char * argv[]){
    const string usage = "search [OPTIONS] [DIRECTORY/DATABASE] \nSearch engine for your files.Currently filters files: md,txt,html\n\
                          --index\t Index recursively given directory\n\t\t\t  --db (Requried) Sets the output database if indexing, or if alone searches the given database\n\
                          --all \t Index all files regardless of file extension. ";

    bool index = false;
    string dir;
    string db;
    bool filterExtensions = true;
    try{
        if(argc < 3){
            cout << usage;
            exit(-1);
        }
        for(int i = 1; i < argc; i++){
           if(strcmp(argv[i],"--index") == 0){
               index = true;
               dir = argv[i+1];
           }
           else if(strcmp(argv[i],"--db") == 0){
               db = argv[i+1];
           }
           else if(strcmp(argv[i],"--all") == 0){
               filterExtensions = false;
           }
        }
    }
    catch(exception& e){
        cout << "\n" << usage << endl;
    }
    if(db.empty()){
        cout << usage;
        exit(-1);
    } 
    try{
        DbHandler dbhandler = DbHandler(db);
        if(!dir.empty()){
            cout << "Indexing..." << endl;
            Lexer lexer = Lexer(dir,filterExtensions);
            int file_amount = lexer.getFiles().size();
            for(int i = 1; i <= file_amount; i++){
                TimeVar t1 = timeNow(); 
                map<string,int> tokens =  lexer.lex();
                string last_file = lexer.getLastFile();
                for (auto const& [token, amount] : tokens) {
                    dbhandler.handleToken(token, last_file, amount);
                }
                TimeVar t2 = timeNow();
                cout << fmt::format( "{} / {}  {}ms\t{}", i, file_amount,duration(t2,t1), last_file) << endl;


            }
            dbhandler.calculateIFDF();
            cout << "Done.";
        }
        cout << "Insert search terms seperated by a space." << endl;
        string terms;
        string term;
        vector<string> termv;
        while(1){
            cout << "Search: ";
            getline(cin,terms);
            for(char c : terms){
                if(c == ' ') {
                    termv.push_back(term);          
                    term = "";
                }
                else term += c;
            }
            termv.push_back(term);
            dbhandler.searchTerms(termv);
            terms.erase();
            term.erase();
            termv.clear();
        }
    } 
    
    catch(exception& e){cout << "Error: " <<  e.what() << endl;}
}
