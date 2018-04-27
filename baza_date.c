#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>


int callback(void *, int, char **, char **);


int main(void) {
    
    sqlite3 *db;
    char *err_msg = 0;
    char *sql;
    
    int rc = sqlite3_open("tabel_password_manager.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    sql ="DROP TABLE IF EXISTS Utilizatori;" 
         "CREATE TABLE Utilizatori(Id INTEGER PRIMARY KEY, Nume TEXT, Parola TEXT);";

    /*
    "INSERT INTO Utilizatori VALUES (1, 'Alex', 'fuckboy');"
    "INSERT INTO Utilizatori VALUES (2,'Roxana', 'simplu');"
    "INSERT INTO Utilizatori VALUES (3,'Raluca', 'dreptcivil');"
    "INSERT INTO Utilizatori VALUES (4,'Codrin', 'javascript');"
    "INSERT INTO Utilizatori VALUES (5,'Razvan', 'retelemaster');"; 
    */
        
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return 1;
    }
    else{
        printf("Ati creat cu succes tabelul Utilizatori!\n");
    }

    sql ="DROP TABLE IF EXISTS Passwords;" 
         "CREATE TABLE Passwords(Id INTEGER PRIMARY KEY, Titlu TEXT, Parola TEXT, Url TEXT, UserName TEXT, Notite TEXT);";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return 1;
    }
    else{
        printf("Ati creat cu succes tabelul Passwords!\n");
    }

    
    sqlite3_close(db);
    
    return 0;
}

int callback(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
    
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
}