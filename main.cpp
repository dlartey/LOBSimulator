#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <typeinfo>

using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

string getProjectSourceDirectory(){
    string currFilePath = __FILE__;
    __fs::filesystem::path fullPath(currFilePath);
    return fullPath.parent_path().string();
    
}

int main(int argc, const char *argv[])
{
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    
    sqlite3 *DB;
    int exit = sqlite3_open(dbFilePath.c_str(), &DB);
    string data("CALLBACK FUNCTION");

    if (exit)
    {
        cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
        return -1;
    }

    
    string sqlstatement = "SELECT * FROM book;";
    int rc = sqlite3_exec(DB, sqlstatement.c_str(), callback, (void *)data.c_str(), NULL);

    sqlite3_close(DB);
    return 0;
}
