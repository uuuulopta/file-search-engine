#include "includes/Lexer.h"
#include "includes/DbHandler.h"
#include <exception>
#include <iostream>
#include <algorithm>
using namespace std;
#include <SQLiteCpp/SQLiteCpp.h>
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
    try{
        Lexer lexer = Lexer("testfiles/dir1");
        lexer.lex([](map<string,int> tokens,string file)->void{
            DbHandler dbhandler = DbHandler("based");
            for (auto const& [key, val] : tokens) {
                dbhandler.handleToken(key, file, val);
            }
            dbhandler.printAll();
         });
         
    } 
    catch(exception& e){cout <<  e.what();}
}
