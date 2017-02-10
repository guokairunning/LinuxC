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
        //������ܵ��ն��ź�
        signal(SIGTTOU,SIG_IGN);
        signal(SIGTTIN,SIG_IGN);
        signal(SIGTSTP,SIG_IGN);
        signal(SIGHUP,SIG_IGN);
        //�����ӽ��̣��������˳�
        if( pid=fork() ){
                exit(EXIT_SUCCESS);
        }
        else if( pid < 0){
                perror("fork");
                exit(EXIT_FAILURE);
        }
        setsid(); //�����µĻỰ�鳤���½����鳤�����ն�
        if(pid=fork()){  //�����½��̣��ӽ��̲����������ն�
                exit(EXIT_SUCCESS);
        }
        else if(pid < 0){
                perror("fork");
                exit(EXIT_FAILURE);
        }
        //�رո����̴򿪵��ļ�������
        for(i=0; i<NOFILE; i++){
                close(i);
 }
 //�Ա�׼����ȫ���ض���./null
open("./null",O_RDONLY);
open("./null",O_RDWR);
open("./null",O_RDWR);
chdir("./Daemon"); //�޸���Ŀ¼
umask(0); //���������ļ�����
signal(SIGCHLD,SIG_IGN);  //�����ӽ����˳�
openlog(pname,LOG_PID,facility); //���ػ����̽�����ϵ�����Ͻ��̺ţ��ļ���
return;
}
int main(int argc, char* argv[]){
        FILE *fp;
        time_t ticks;
        init_Daemon(argv[0],LOG_DAEMON);   //ִ���ػ����̺���
        while(1){
                sleep(1);
                ticks=time(NULL);
                //д��־��Ϣ
                syslog(LOG_INFO,"%d",asctime(localtime(&ticks)));
        }
        return 0;
}