#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

#include <sqlite3.h>

#define N 24
#define FAILED -1
#define SUCCESS 0
#define LOGIN_CMD 1
#define RESET_CMD 2
#define REGISTER_CMD 3
#define QUIT_CMD 4
#define USER_HISTORY_CMD 5
#define USER_QUERY_CMD 6
#define EMPLOYEE_REGISTER_CMD 7

#define ONLINE 10
#define OFFLINE 11
#define QUIT 12

#define ENDING 20


typedef struct user{
	int type;
	char name[N];
	char password[N];
}user_t, *puser_t;

typedef struct employee{
	char name[N];
	char addr[N];
	unsigned int age;
	unsigned int level;
	unsigned int id;
	char phonenum[N];
	double salary;
}employee_t, *pemployee_t;

typedef struct MSG{
	int cmd;
	char data[N];
	char MSG[N];
	employee_t employee;
	user_t user;
}MSG_t;

#define SQL_LEN 128
typedef struct
{
    int fd;
    sqlite3 *db;
} server_info_t;

#endif