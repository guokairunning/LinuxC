#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<syslog.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<time.h>
#include<sys/param.h>
#include<unistd.h>
int init_daemon(void)
{
    int pid;
    int i;

    //忽略终端IO信号 stop信号
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    pid = fork();
    if(pid > 0){
        exit(0);//结束父进程，使子进程成为后台
    }
    else if(pid < 0){
        return -1;
    }
    //建立一个新的进程组,在这个新的进程组中,子进程成为这个进程组的首进程
    setsid();

    pid = fork();
    if(pid > 0){
        exit(0);
    }
    else if(pid < 0){
        return -1;
    }

    for(i = 0; i < NOFILE;close(i++));
    
    chdir("/");
    umask(0);
    signal(SIGCHLD, SIG_IGN);
    return 0;

    
}

int main(){
    time_t now;
   int i = init_daemon();
    syslog(LOG_USER|LOG_INFO,"测试守护进程!\n");
    while(1){
        sleep(8);
        time(&now);
        syslog(LOG_USER|LOG_INFO,"系统时间:\t%s\t\t\n",ctime(&now));
    }
}