#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
init_Daemon(const char *pname, int facility){
        int pid;
        int i;
        //处理可能的终端信号
        signal(SIGTTOU,SIG_IGN);
        signal(SIGTTIN,SIG_IGN);
        signal(SIGTSTP,SIG_IGN);
        signal(SIGHUP,SIG_IGN);
        //创建子进程，父进程退出
        if( pid=fork() ){
                exit(EXIT_SUCCESS);
        }
        else if( pid < 0){
                perror("fork");
                exit(EXIT_FAILURE);
        }
        setsid(); //设置新的会话组长，新进程组长脱离终端
        if(pid=fork()){  //创建新进程，子进程不能再申请终端
                exit(EXIT_SUCCESS);
        }
        else if(pid < 0){
                perror("fork");
                exit(EXIT_FAILURE);
        }
        //关闭父进程打开的文件描述符
        for(i=0; i<NOFILE; i++){
                close(i);
 }
 //对标准输入全部重定向到./null
open("./null",O_RDONLY);
open("./null",O_RDWR);
open("./null",O_RDWR);
chdir("./Daemon"); //修改主目录
umask(0); //重新设置文件掩码
signal(SIGCHLD,SIG_IGN);  //处理子进程退出
openlog(pname,LOG_PID,facility); //与守护进程建立联系，加上进程号，文件名
return;
}
int main(int argc, char* argv[]){
        FILE *fp;
        time_t ticks;
        init_Daemon(argv[0],LOG_DAEMON);   //执行守护进程函数
        while(1){
                sleep(1);
                ticks=time(NULL);
                //写日志信息
                syslog(LOG_INFO,"%d",asctime(localtime(&ticks)));
        }
        return 0;
}