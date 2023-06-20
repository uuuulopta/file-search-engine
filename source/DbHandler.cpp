#include "../includes/DbHandler.h"
#include "SQLiteCpp/Database.h"
DbHandler::DbHandler(string database) : db(database){
    db.exec("CREATE TABLE IF NOT EXISTS Files(file_id INTEGER PRIMARY KEY, file_name TEXT )");    
    db.exec("CREATE TABLE IF NOT EXISTS Tokens(token_id INTEGER PRIMARY KEY, token_name TEXT)");    
    db.exec("CREATE TABLE IF NOT EXISTS Tokens_Files(FOREIGN KEY(token_id) REFRENCES Tokens(token_id) ,FOREIGN KEY(file_id) REFRENCES Files(file_id),count INTEGER)");    

}
