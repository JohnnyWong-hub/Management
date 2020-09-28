#include <stdio.h>
#include <unistd.h>
#include "protocal.h"
#include "socketinit.h"
#include "menu.h"

void user_Query(int fd, MSG_t *msg);
void login_Func(int fd, MSG_t *msg);
void show_User_Info(MSG_t *msg);
void quit_Func(int fd, MSG_t *msg);
void register_Func(int fd, MSG_t *msg);
int check_Name(char *name);
int check_Password(char *password);
int employee_Register(int fd, MSG_t *msg);
int check_Age(unsigned int age);

int main(int argc, char *argv[])
{
    int fd = 0;
    int cmd = 0;
    MSG_t msg;
    if (argc < 3)
    {
        printf("Please input right argements\n");
        return FAILED;
    }

    //套接字初始化
    fd = socket_Init(argv[1], argv[2]);
    if (fd < 0)
    {
        return FAILED;
    }

    while (1)
    {
        if( cmd == QUIT_CMD)
        {
            puts("Bye~");
            break;
        }
        main_Menu();
        scanf("%d", &cmd);
        while (getchar() != '\n')
            ;

        switch (cmd)
        {
        case LOGIN_CMD:
            login_Func(fd, &msg);
            break;
        case RESET_CMD:
            break;
        case REGISTER_CMD:
            register_Func(fd, &msg);
            break;
        case QUIT_CMD:
            quit_Func(fd, &msg);
            break;
        default:
            break;
        }
    }

    return 0;
}

void login_Func(int fd, MSG_t *msg)
{
    int ret = 0;
    printf("Please input username:");
    scanf("%s", msg->user.name);
    while (getchar() != '\n')
        ;
    printf("Please input password:");
    scanf("%s", msg->user.password);
    while (getchar() != '\n')
        ;
    msg->cmd = LOGIN_CMD;
    msg->user.type = OFFLINE;
    printf("%s\n", msg->user.name);
    printf("%s\n", msg->user.password);
    do
    {
        ret = send(fd, (MSG_t *)msg, sizeof(MSG_t), 0);
    } while (ret < 0);

    ret = recv(fd, (MSG_t *)msg, sizeof(MSG_t), 0);
    if (ret < 0)
    {
        perror("recv");
        return;
    }

    if (msg->user.type == ONLINE)
    {
        printf("%s\n", msg->MSG);
        return;
    }
    else if (msg->cmd == FAILED)
    {
        printf("%s\n", msg->MSG);
        return;
    }

    printf("Login success\n");
    user_Query(fd, msg);
}

void user_Query(int fd, MSG_t *msg)
{
    int cmd = 0;
    while (1)
    {
        if(QUIT == msg->cmd)
            break;
        user_Query_Menu();
        scanf("%d", &cmd);
        while (getchar() != '\n')
            ;

        switch (cmd)
        {
        case 1:
            msg->cmd = USER_QUERY_CMD;
            send(fd, msg, sizeof(MSG_t), 0);
            recv(fd, (MSG_t *)msg, sizeof(MSG_t), 0);
            if (msg->cmd == FAILED)
            {
                printf("Query FAILED!\n");
            }
            else
            {
                show_User_Info(msg);
            }
            break;
        case 2:
            msg->cmd = USER_HISTORY_CMD;
            send(fd, msg, sizeof(MSG_t), 0);
            while (recv(fd, (MSG_t *)msg, sizeof(MSG_t), 0))
            {
                if (msg->cmd == ENDING)
                {
                    break;
                }
                printf("****************************\n");
                printf("username:%s\t, QueryData:%s\n", msg->employee.name, msg->data);
            }
        case 3:
            msg->cmd = QUIT;
            send(fd, msg, sizeof(MSG_t), 0);
            break;
        default:
            printf("please input true cmd\n");
            break;
        }
    }
}

void show_User_Info(MSG_t *msg)
{
    printf("***********************\n");
    printf("  Name:%s\n", msg->employee.name);
    printf("  Addr:%s\n", msg->employee.addr);
    printf("  Age:%d\n", msg->employee.age);
    printf("  Level:%d\n", msg->employee.level);
    printf("  Id:%d\n", msg->employee.id);
    printf("  Phonenum:%s\n", msg->employee.phonenum);
    printf("  Salary:%f\n", msg->employee.salary);
    printf("***********************\n");
    puts("");
}

void quit_Func(int fd, MSG_t *msg)
{
    msg->cmd = QUIT;
    send(fd, (MSG_t*)msg, sizeof(MSG_t), 0);
}

void register_Func(int fd, MSG_t *msg)
{
    char name[N] = "";
    char password[N] = "";
    printf("Please Input your name:");
    scanf("%s", name);
    while (getchar() != '\n');
    if(FAILED == check_Name(name))
    {
        printf("Check your name\n");
        return;
    }
    printf("Please Input your password:");
    scanf("%s", password);
    while(getchar() != '\n');

    if(FAILED == check_Password(password))
    {
        printf("Check your Password\n");
        return;
    }
    msg->cmd = REGISTER_CMD;
    sprintf(msg->user.name, "%s", name);
    sprintf(msg->user.password, "%s", password);
    send(fd, (MSG_t*)msg, sizeof(MSG_t), 0);
    recv(fd, (MSG_t*)msg, sizeof(MSG_t), 0);
    if(FAILED == msg->cmd)
    {
        printf("Register failed:%s\n", msg->MSG);
        printf("Please connect Manager help you modifi your Information\n");
    }else{
        //员工信息注册
        employee_Register(fd, &msg);
    }
}

int check_Name(char *name)
{
    if(!strcmp("", name))
    {
        return FAILED;
    }
    char *p = name;
    while(*p != '\0')
    {
        if(!((*p <='z' && *p >='a') || (*p >= 'A' && *p <= 'Z')))
        {
            return FAILED;
        }
        p++;
    }
    return SUCCESS;
}

int check_Password(char *password)
{
    if(!strcmp("", password))
    {
        return FAILED;
    }
    return SUCCESS;
}

int employee_Register(int fd, MSG_t *msg)
{
    int ret;
    do{
        printf("Please Input your name:");
        scanf("%s", msg->employee.name);
        ret = check_Name(msg->employee.name);
    }while(ret == FAILED);

    do{
        printf("Please Input your addr:");
        scanf("%s", msg->employee.addr);
    }while(0);

    do{
        printf("Please Input your age:");
        scanf("%d", msg->employee.age);
        ret = check_Age(msg->employee.age);
    }while(ret == FAILED);

    do{
        //提示等级在1-7级
        printf("Please Input your level:");
        scanf("%d",)
    }
}

int check_Age(unsigned int age)
{
    if(!(age < 0 || age >140))
    {
        return SUCCESS;   
    }
    return FAILED;
}