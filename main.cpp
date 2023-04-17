#include <filesystem>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <vector>
#include "sqlite3.h"
#include <stdio.h>

using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

static int createDB(const char* s)
{
    sqlite3* DB;

    int exit = 0;
    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

static int createTable(const char* s)
{
    sqlite3 *DB;
    char* messageError;

    string sql = "CREATE TABLE IF NOT EXISTS COLORS("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "AMOUNT INT NOT NULL);";

    try
    {
        int exit = 0;
        exit = sqlite3_open(s, &DB);
        /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error in createTable function." << endl;
            sqlite3_free(messageError);
        }
        else
            cout << "Table created Successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception& e)
    {
        cerr << e.what();
    }

    return 0;
}

static int insertData(const char* s)
{
    sqlite3* DB;
    char* messageError;

    for (int i = 0; i < 3125; i++){
        string sql("INSERT INTO COLORS (AMOUNT) VALUES(0);"
                   "INSERT INTO COLORS (AMOUNT) VALUES(0);"
                   "INSERT INTO COLORS (AMOUNT) VALUES(0);"
                   "INSERT INTO COLORS (AMOUNT) VALUES(0);"
                   "INSERT INTO COLORS (AMOUNT) VALUES(0);");

        int exit = sqlite3_open(s, &DB);
        /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error in insertData function." << endl;
            sqlite3_free(messageError);
        }
        else
            cout << "Records inserted Successfully!" << endl;
    }

    string sql("INSERT INTO COLORS (AMOUNT) VALUES(500);");

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error in insertData function." << endl;
        sqlite3_free(messageError);
    }
    else
        cout << "Records inserted Successfully!" << endl;

    return 0;
}

static int updateData(const char* s, int id, int amount)
{
    sqlite3* DB;
    char* messageError;

    string sql("SELECT AMOUNT FROM COLORS WHERE ID = " + to_string(id) + ";");

    sqlite3_stmt* stmt;

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    int newAmount = 0;
    while(sqlite3_step(stmt) != SQLITE_DONE){
        newAmount += sqlite3_column_int(stmt, 0);
    }

    cout << newAmount << endl;

    newAmount += amount;



    if (exit != SQLITE_OK) {
        cerr << "Error in selectData function." << endl;
        sqlite3_free(messageError);
    }
    else
        cout << "Records selected Successfully!" << endl;





    sql = "UPDATE COLORS SET AMOUNT = " + to_string(newAmount) + " WHERE ID = '" + to_string(id) + "'";


//    exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error in updateData function." << endl;
        sqlite3_free(messageError);
    }
    else
        cout << "Records updated Successfully!" << endl;

    return 0;
}


int main(int argc, char *argv[]) {

    int width, height, channels;
    unsigned char* img = stbi_load("../images/1216272.jpg", &width, &height, &channels, 3);
    cout << width << endl;
    cout << height<< endl;

    size_t img_size = width * height * channels;


    int red = {};
    int green = {};
    int blue = {};

    vector<int> a = {};
    for (int i = 0; i < 256; i++){
        a.emplace_back(0);
    }

    vector<vector<int>> b = {};
    for (int j = 0; j < 256; j++){
        b.emplace_back(a);
    }

    vector<vector<vector<int>>> color_bins = {};
    for (int k = 0; k < 256; k++){
        color_bins.emplace_back(b);
    }


    for (unsigned char* a = img; a != img + img_size; a += 3){
        red = (int(*a) / 10);
        green = (int(*(a + 1)) / 10);
        blue = (int(*(a + 2)) / 10);

        color_bins[red][green][blue] += 1;

    }

    createDB("colors.db");
    createTable("colors.db");
//    insertData("colors.db");




    for (int i = 0; i < 25; i++){
        for (int j = 0; j < 25; j++){
            for (int k = 0; k < 25; k++){
                if (color_bins[i][j][k] != 0){
                    updateData("colors.db", i+(j*25)+(k*625), color_bins[i][j][k]);
                }
            }
        }
    }

    int maxColor1 = 0;
    int maxColor2 = 0;
    int maxColor3 = 0;

    for (int i = 0; i < 25; i++){
        for (int j = 0; j < 25; j++){
            for (int k = 0; k < 25; k++){
                if (color_bins[i][j][k] >= maxColor1){
                    maxColor3 = maxColor2;
                    maxColor2 = maxColor1;
                    maxColor1 = color_bins[i][j][k];
                }
            }
        }
    }

    cout << "The Most Used Colors in This Image Are:" << endl <<
            "1. " + maxColor1 << endl <<
            "2. " + maxColor2 << endl <<
            "3. " + maxColor3 << endl;


    return 0;
}