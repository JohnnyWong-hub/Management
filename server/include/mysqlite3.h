#ifndef __MYSQLITE3__H__
#define __MYSQLITE3__H__

#include <sqlite3.h>
#include "protocal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int database_Init(sqlite3 **db);
extern int off_LINE(sqlite3 *db, const char *name);
extern int on_LINE(sqlite3 *db, const char *name);
extern int get_Login_State(sqlite3 *db, const char *name);
extern int query_USER(sqlite3 *db, MSG_t *msg);
extern int user_Register(sqlite3 *db, MSG_t *msg);
extern int employee_Regitser(sqlite3 *db, MSG_t *msg);

#endif