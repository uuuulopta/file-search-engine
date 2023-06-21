#pragma once
#include "SQLiteCpp/Database.h"
#include <SQLiteCpp/SQLiteCpp.h>
using namespace std;
class DbHandler{
private:
    SQLite::Database db;
public:
    DbHandler(string database);
    ~DbHandler();
    int getTokenIdByName(string token_name);
    int getFileIdByName(string file_name);
    int getAmount(int token_id,int file_id);
    void handleToken(string token_name,string file_name,int amount);
    void inputFile(string file_name);
    void inputToken(string file_name);
    void printAll();
    //get total occurances (when handling token add increase total occurance)
    //get the occurance for each file and divide it by total occurance.
    void calculateIFDF();
};

