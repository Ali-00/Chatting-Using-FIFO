#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define size 1020
#define FIFO_PERM (S_IRUSR | S_IWUSR)

int main(int argc, char *argv[]) {
   	int fd,re,ch1,ch2,ch3;
   	char buff[size];
   	fd_set rs;

	re=mkfifo(argv[1],FIFO_PERM);
	if(re==-1)
	{
		if(errno != EEXIST)
		{
			perror("Error: \n");
			return -1;
		}
	}
	fd=open(argv[1],O_RDWR);
	if(fd==-1)
	{
		perror("Error: \n");
		return -1;
	}
	sleep(5);
	while(1)
	{
		FD_ZERO(&rs);
		FD_SET(fd,&rs);
		FD_SET(STDIN_FILENO,&rs);
		ch1 = select(fd+1,&rs,NULL,NULL,NULL);
		if(ch1==-1)
		{
			perror("Error: \n");
			return -1;
		}
		if(FD_ISSET(STDIN_FILENO,&rs))
		{
			ch2=read(STDIN_FILENO,buff,sizeof(buff));
			ch3=write(fd,buff,ch2);
			if(ch3==-1)
			{
				perror("Error: \n");
				return -1;
			}
			sleep(5);
			continue;
		}
		if(FD_ISSET(fd,&rs))
		{
			ch2=read(fd,buff,sizeof(buff));
			if(ch2==-1)
			{
				perror("Error: \n");
				return -1;
			}
			else if(ch2==0)
                	{
                		continue;
                	}
            		printf("Recieved: %s",buff);

		}
	}
   return 0;
}
