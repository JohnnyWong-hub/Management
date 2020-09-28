#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "protocal.h"
#include "socketinit.h"
#include "menu.h"
#include "mysqlite3.h"

void *mythread_Func(void *arg);

int main(int argc, char *argv[])
{
    int fd = 0, newfd;
    struct sockaddr_in cin;
    socklen_t len = sizeof(struct sockaddr_in);
    pthread_t tid;
    sqlite3 *mydb = NULL;
    server_info_t thred_info;
    if (0 != database_Init(&mydb))
    {
        fprintf(stderr, "open database failed\n");
        return FAILED;
    }

    thred_info.db = mydb;
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
        //接受请求
        newfd = accept(fd, (struct sockaddr *)&cin, &len);
        if (newfd < 0)
        {
            perror("accept");
            return FAILED;
        }
        else
        {
            thred_info.fd = newfd;
            pthread_create(&tid, NULL, mythread_Func, &thred_info);
        }
    }

    close(fd);
    sqlite3_close(mydb);
    return 0;
}

void *mythread_Func(void *arg)
{
    int ret = 0;
    char *errmsg = NULL;
    MSG_t msg;
    int fd = ((server_info_t *)arg)->fd;
    sqlite3 *db = ((server_info_t *)arg)->db;
    pthread_detach(pthread_self());

    while (recv(fd, (MSG_t *)&msg, sizeof(MSG_t), 0))
    {
        if (QUIT == msg.cmd)
            break;
        switch (msg.cmd)
        {
        case LOGIN_CMD:
            ret = get_Login_State(db, msg.user.name);
            if (ONLINE == ret)
            {
                msg.cmd = FAILED;
                sprintf(msg.MSG, "USER HAS BEEN LOGIN");
                send(fd, &msg, sizeof(MSG_t), 0);
            }
            else if(-1 == ret){
                msg.cmd = FAILED;
                sprintf(msg.MSG, "USER NOT REGISTER");
                send(fd, &msg, sizeof(MSG_t), 0);
                printf("User named:%s Login failed\n", msg.user.name);
            }
            else
            {
                char sql[SQL_LEN] = "";
                sprintf(sql, "SELECT * FROM USER WHERE NAME = '%s' AND PASSWORD = '%s';", msg.user.name, msg.user.password);
                ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
                if (SQLITE_OK != ret)
                {
                    fprintf(stderr, "LOGIN FAILED:%s", errmsg);
                    msg.cmd = FAILED;
                    sprintf(msg.MSG, "LOGIN FAILED CHECK");
                    send(fd, &msg, sizeof(MSG_t), 0);
                }
                else
                {
                    msg.cmd = SUCCESS;
                    send(fd, &msg, sizeof(MSG_t), 0);
                    //更新登录状态
                    on_LINE(db, msg.user.name);
                    while (recv(fd, (MSG_t *)&msg, sizeof(MSG_t), 0))
                    {
                        if (QUIT == msg.cmd)
                        {
                            off_LINE(db, msg.user.name);
                            break;
                        }
                        switch (msg.cmd)
                        {
                        case USER_QUERY_CMD:
                            msg.cmd = SUCCESS;
                            send(fd, &msg, sizeof(MSG_t), 0);
                            break;
                        case USER_HISTORY_CMD:

                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            break;
        case RESET_CMD:
            printf("this is RESET_CMD\n");
            break;
        case REGISTER_CMD:
            ret = user_Register(db, &msg);
            if(FAILED == ret)
            {
                msg.cmd = FAILED;
                send(fd, (MSG_t*)&msg, sizeof(MSG_t), 0);
            }
            else if(SUCCESS == ret){
                msg.cmd = SUCCESS;
                send(fd, (MSG_t *)&msg, sizeof(MSG_t), 0);
            }
            break;
        default:
            break;
        }
    }
    off_LINE(db, msg.user.name);
    printf("Thread exited:%ld\n", pthread_self());
    close(fd);
    pthread_exit(NULL);
}

