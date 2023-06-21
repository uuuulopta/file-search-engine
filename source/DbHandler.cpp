#include "../includes/DbHandler.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <string_view>
DbHandler::DbHandler(string database) : db(database,SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE){
    db.exec("CREATE TABLE IF NOT EXISTS Files(`file_id` INTEGER PRIMARY KEY AUTOINCREMENT, file_name TEXT UNIQUE collate nocase)");    
    db.exec("CREATE TABLE IF NOT EXISTS Tokens(`token_id` INTEGER PRIMARY KEY AUTOINCREMENT, token_name TEXT UNIQUE collate nocase)");    
    db.exec("CREATE TABLE IF NOT EXISTS Tokens_Files(`token_id` INTEGER ,file_id INTEGER,amount INTEGER,PRIMARY KEY (token_id,file_id) FOREIGN KEY (token_id) REFERENCES Tokens (token_id), FOREIGN KEY (file_id) REFERENCES Files (file_id));");    
}

int DbHandler::getTokenIdByName(string token_name){
   try{
string query = fmt::format("SELECT `token_id` FROM Tokens WHERE token_name='{}'",token_name);
       return db.execAndGet(  query );}
   catch(exception& e){
    return -1;
   }
}
int DbHandler::getFileIdByName(string file_name){
   try{
       return db.execAndGet(fmt::format("SELECT `file_id` FROM Files WHERE file_name='{}'",file_name));
   }
   catch(exception& e){
    return -1;
   }
}

int DbHandler::getAmount(int token_id,int file_id){
   try{
       return db.execAndGet(fmt::format("SELECT 'amount' FROM Tokens_Files WHERE token_id='{}' AND file_id='{}'",token_id,file_id));
   }
   catch(exception& e){
    return -1;
   }
}
void DbHandler::inputFile(string file_name){
string query = fmt::format("INSERT INTO Files (file_name) VALUES ('{}')",file_name);
    db.tryExec(query);
}

void DbHandler::inputToken(string token){
    string query = fmt::format("INSERT INTO Tokens (token_name) VALUES ('{}')",token);
     db.exec(query); }

void DbHandler::handleToken(string token_name, string file_name,int amount){
    int file_id = getFileIdByName(file_name);
    int token_id = getTokenIdByName(token_name);
    if(file_id == -1){
        inputFile(file_name);
        file_id = getFileIdByName(file_name);
    }

    if(token_id == -1){
        inputToken(token_name);
        token_id = getTokenIdByName(token_name);
    }
    string query = fmt::format( "INSERT INTO Tokens_Files (`token_id`, `file_id`,'amount')\
                VALUES ('{}', '{}', '{}')\
                ON CONFLICT(token_id,file_id) DO UPDATE SET amount = amount + 1;" 
                ,token_id,file_id,amount);
    db.exec(query);
}
void DbHandler::calculateIFDF(){
    // select sum(amount) from 
}
void DbHandler::printAll(){
    SQLite::Statement query(db,"SELECT `token_name`,`amount`,`file_name` FROM Tokens_Files natural join Tokens natural join Files");
    cout << "printall";
        
    while(query.executeStep()){
            cout << ("execute!");
        
        std::cout << "row: " << query.getColumn(0)<< ", " << query.getColumn(1) << ", " << query.getColumn(2)<<endl;
    
    }
}
DbHandler::~DbHandler(){
    db.backup("dbsaved",SQLite::Database::Save);
}
