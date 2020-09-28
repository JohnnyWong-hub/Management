#include "mysqlite3.h"

int database_Init(sqlite3 **db)
{
    int ret;
    char *errmsg;
    char *create_table_user_sql = "CREATE TABLE IF NOT EXISTS USER(\
            TYPE INTEDER,\
            NAME CHAR PRIMARY KEY,\
            PASSWORD CHAR);";
    char *create_table_employee_sql = "CREATE TABLE IF NOT EXISTS EMPLOYEE(\
            NAME CHAR,\
            ADDR CHAR,\
            AGE INTEDER,\
            LEVEL INTEDER,\
            ID INTEDER PRIMARY KEY,\
            PHONENUM CHAR,\
            SALARY REAL);";
    char *create_table_manager_sql = "CREATE TABLE IF NOT EXISTS MANAGER(\
            NAME CHAR PRIMARY KEY,\
            PASSWORD CHAR);";
    char *create_table_hisory_sql = "CREATE TABLE IF NOT EXISTS HISTORY(\
            NAME CHAR,\
            ID INTEDER PRIMARY KEY,\
            DATA CAHR);";
    char *init_user_table_sql = "UPDATE USER SET TYPE=11";

    ret = sqlite3_open("sqlite3/database.db", db);
    if (SQLITE_OK == ret)
    {
        printf("open database success\n");
    }
    else
    {
        printf("database open failed\n");
        return -1;
    }

    ret = sqlite3_exec(*db, create_table_user_sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "CREATE TABLE FAILED:%s\n", errmsg);
        return -1;
    }

    ret = sqlite3_exec(*db, create_table_employee_sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "CREATE TABLE FAILED:%s\n", errmsg);
        return -1;
    }

    ret = sqlite3_exec(*db, create_table_manager_sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "CREATE TABLE FAILED:%s\n", errmsg);
        return -1;
    }

    ret = sqlite3_exec(*db, create_table_hisory_sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "CREATE TABLE FAILED:%s\n", errmsg);
        return -1;
    }

    ret = sqlite3_exec(*db, init_user_table_sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "UPDATE TABLE FAILED:%s\n", errmsg);
        return -1;
    }
    return 0;
}

int off_LINE(sqlite3 *db, const char *name)
{
    int ret;
    char sql[SQL_LEN] = "";
    char *errmsg;
    sprintf(sql, "UPDATE USER SET TYPE=11 WHERE NAME='%s';", name);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "UPDATE OFF_LINE STATE FAILED:%s\n", errmsg);
        return -1;
    }
    return 0;
}

int on_LINE(sqlite3 *db, const char *name)
{
    int ret;
    char sql[SQL_LEN] = "";
    char *errmsg;
    sprintf(sql, "UPDATE USER SET TYPE=10 WHERE NAME='%s';", name);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if(SQLITE_OK != ret)
    {
        fprintf(stderr, "UPDATE ON_LINE STATE FAILED:%s\n", errmsg);
        return -1;
    }
    return 0;
}

int get_Login_State(sqlite3 *db, const char *name)
{
    int ret= 0, state;
    int nRow, nColumn;
    char *errmsg;
    char sql[SQL_LEN] = "";
    sprintf(sql, "SELECT TYPE FROM USER WHERE NAME='%s';", name);
    char **dResult;
    ret = sqlite3_get_table(db, sql, &dResult, &nRow, &nColumn, &errmsg);
    if( SQLITE_OK != ret || nRow == 0)
    {
        printf("CHECK LOGIN STATE FAILED:%s:USER NOT REGISTER\n", errmsg);
        return -1;
    }
    state = atoi(dResult[1]);
    printf("%d\n",state);
    return state;
}

int query_USER(sqlite3 *db, MSG_t *msg)
{
    return 0;
}


int user_Register(sqlite3 *db, MSG_t *msg)
{
    if(!strcmp("", msg->user.name))
    {
        sprintf(msg->MSG,"INVAID USERNAME");
        return FAILED;
    }
    int ret;
    char *errmsg;
    char sql[SQL_LEN] = "";
    sprintf(sql, "INSERT INTO USER(TYPE,NAME,PASSWORD) VALUES(%d, '%s', '%s');", OFFLINE, msg->user.name, msg->user.password);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if( SQLITE_OK != ret)
    {
        fprintf(stderr, "REGISTERFAILED:%s\n", errmsg);
        sprintf(msg->MSG, errmsg);
        return FAILED;
    }

    sprintf(msg->MSG, "Register SUCCESS");
    
    return SUCCESS;
}

int employee_Regitser(sqlite3 *db, MSG_t *msg)
{
    // int ret;
    // int nRow, nColumn;
    // char **dResult;
    // char *errmsg;
    // char sql[SQL_LEN] = "";
    // ret = sqlite3_get_table()
    return 0;
}
