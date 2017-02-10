#include"stdio.h"
#include"stdlib.h"
#include"fcntl.h"
#include"sys/types.h"
#include"unistd.h"
#include"sys/wait.h"
void main()  
 {
    int pid,fd;
	int fds[2];
    if((pid=fork())==-1)    
	{
		 perror("fork");     
		 exit(1);   
	}
    else if(pid==0)	      
	{
          close(1);
          fd=creat("list.txt",0644);	  	
	      if(pipe(fds)==-1)
		  {
		    perror("pipe");
	 	    exit(1);
		  }
    	if (fork()== 0) 
		{
		char buf[128];
		dup2(fds[0], 0);
		close(fds[1]);			
		execlp("sort", "sort", NULL);
	
	}
	else 
	{
		if(fork() == 0) 
		{
			dup2(fds[1], 1);
			close(fds[0]);
			execlp("who", "who", NULL);
		}
		else 
		{
			close(fds[0]);
			close(fds[1]);
			wait(NULL);
			wait(NULL);
		}
	}
   }
    else if(pid!=0) 
    {
     wait(NULL);	
     system("cat list.txt");   
	}
}
