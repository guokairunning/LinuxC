#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<syslog.h>
#include<signal.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/wait.h>
#include<pwd.h>
char NowDir[100];
#define CD              10   //CD命令
void print_prompt(){
    struct passwd *my_name;
    my_name = getpwuid(getuid());
    char *buf,name[65];
    buf = (char *)malloc(100);
    getcwd(buf,100);
    if(strcmp(NowDir,buf) == 0){
        buf[0] = '~';
        buf[1] = '\0';
    }
    gethostname(name,sizeof(name));
    printf("%s@%s:%s$ ",my_name->pw_name,name,buf);
}
void changedir(char *path)
{
   if(chdir(path))
    {
        printf("bash:%s: 没有那个文件或目录",path);
    }
}
void input_command(char *buf) //获取输入的命令
{
    int len = 0;
    int ch;

    ch = getchar();
    while(len < 256 && ch != '\n'){
        buf[len++] = ch;
        ch = getchar();
    }
    if(len == 256){
        printf("command is too long\n");
        exit(-1);
    }
    buf[len] = '\n';
    len++;
    buf[len] = '\0';
}

void explain_input(char *buf, int *argcount, char arglist[100][256])
{
    char *p = buf;
    char *q = buf;
    int number = 0;

    while(1){
        if(p[0] == '\n')
        break;

        if(p[0] == ' ')
        p++;
        else{
            q = p;
            number = 0;
            while((q[0] != ' ') && (q[0] != '\n'))
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}

//查找命令中的可执行程序
int find_command(char *command)
{
    DIR *dp;
    struct dirent *dirp;
    char *path[] = {"./","/bin","/usr/bin",NULL};

    //使当前目录下的程序可以运行,如命令"/fork"可以被正确解释执行
    if(strncmp(command,"./",2) == 0)
    command = command + 2;

    //分别在当前目录 /bin 和 /usr/bin/ 目录查找要执行的程序
    int i = 0;
    while(path[i] != NULL)
    {
        if((dp = opendir(path[i])) == NULL)
        printf("can not open /bin\n");

        while((dirp = readdir(dp)) != NULL)
        {
            if(strcmp(dirp->d_name,command) == 0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}
void do_cmd(int argcount, char arglist[100][256])
{
    int     how  = 0;
    int     background = 0;
    int     status;
    int     i;
    int     fd;
    char   *arg[argcount + 1];
    char   *argnext[argcount + 1];
    char   *file;
    pid_t   pid;

    //将命令取出
    for(i = 0; i < argcount; i++)
    {
        arg[i] = (char *)arglist[i];
    }
    arg[argcount] = NULL;

    if(strcmp("cd",arg[0]) == 0)
    how = CD;
    if( (pid = fork()) < 0)
    {
        printf("fork error\n");
        return ;
    }

    switch(how)
    {
        case 0:
        //pid为0说明是子进程，在子进程中执行命令
        //输入的命令中不含>,<,|
        if(pid == 0)
        {
            if(!(find_command(arg[0])))
           {
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case CD:
            changedir(arg[1]);
            waitpid(pid,&status,0);
            return ;
        default:
            break;
    }

    //父进程等待子进程结束
    if(waitpid(pid,&status,0) == -1)
    printf("wait for child process error\n");
}

int main(int argc,char *argv[]){
    int i;
    char *buf = NULL;
    int argcount = 0;
    char  arglist[100][256];

    getcwd(NowDir,sizeof(NowDir));
    buf = (char *)malloc(256);
    if(buf == NULL){
        perror("malloc error");
        exit(-1);
    }

    while(1){
        memset(buf,0,256);
        print_prompt();
        input_command(buf);
        if((strcmp(buf,"exit\n")) == 0){
            break;
        }
        for(i = 0;i < 100; i++){
            arglist[i][0] = '\0';
        }
        argcount = 0;
        explain_input(buf, &argcount, arglist);
        do_cmd(argcount, arglist);
    }
    if(buf != NULL){
        free(buf);
        buf = NULL;
    }
    exit(0);
}

