#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

#define N 24

typedef struct user{
	int type;
	char name[N];
	char password[N];
}user_t, *puser_t;

typedef struct employee{
	char name[N];
	char addr[N]
	unsigned int age;
	unsigned int level;
	unsigned int id;
	char phonenum[N];
	double salary;
}employee_t, *pemployee_t;

typedef union{
	
}MSG;

#endif