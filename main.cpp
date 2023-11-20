#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <thread>
#include <chrono>
#include <future>

using namespace std;

int callback(void*, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++)
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    return 0;
}


void asyncFunction(string SQLStatement, sqlite3* DB){
    while(true){
        sqlite3_exec(DB, SQLStatement.c_str(), callback, nullptr, NULL);
        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}


void asyncSQLTest(string SQLStatement, sqlite3* DB){
    thread(asyncFunction, SQLStatement, DB).detach();
}

sqlite3* connectToDB(string dbFilePath){
    sqlite3* DB;
    int exit = sqlite3_open(dbFilePath.c_str(), &DB);
    if (exit != SQLITE_OK){
        sqlite3_close(DB);
        throw runtime_error("Failed to open database.");
    }
    return DB;
}

string getProjectSourceDirectory()
{
    string currFilePath = __FILE__;
    __fs::filesystem::path fullPath(currFilePath);
    return fullPath.parent_path().string();
}

int main(int argc, const char *argv[])
{
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";

    sqlite3* DB;
    try{
        DB = connectToDB(dbFilePath);
    }
    catch (const exception& e){
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }
    
    asyncSQLTest("SELECT * FROM book", DB);
    while(true){
        
    }
    sqlite3_close(DB);
    return 0;
}
