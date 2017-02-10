#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>

void f(int signum) 
{
    if(signum == SIGINT)
    {
        printf("uid = %d\n",getuid());
        signal(SIGINT, SIG_DFL);
    }
    else if(signum == SIGTERM)
    {
        printf("pid = %d\n",getpid());
        signal(SIGTERM, SIG_DFL);
    }
}
void main()
{
	signal(SIGINT, f);
	signal(SIGTERM, f);
	while(1) {}	
}
