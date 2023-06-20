#pragma once
#include "SQLiteCpp/Database.h"
#include <SQLiteCpp/SQLiteCpp.h>
using namespace std;
class DbHandler{
private:
    SQLite::Database db;
public:
    DbHandler(string database);
};

