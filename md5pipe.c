#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "md5.h"
#include <fcntl.h>
#include <sys/uio.h>
#include <signal.h>
#include <sys/fcntl.h>

char input[20];

void checkLength(int pid)
{

	if (strlen(input) == 32)
	{
		printf("the string is %s\n", input);
		kill(pid, SIGQUIT);
	}
}

void killChild(int pid)
{
	exit(0);
}

int main()
{

	int fd1[2];
	int fd2[2];
	signal(SIGHUP, checkLength);
	signal(SIGQUIT, killChild);

	pid_t p;

	if (pipe(fd1) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	if (pipe(fd2) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}

	fgets(input, 20, stdin);

	p = fork();

	if (p < 0)
	{
		fprintf(stderr, "fork Failed");
		return 1;
	}

	// Parent process
	else if (p > 0)
	{
		close(fd1[0]);

		// pipe.
		write(fd1[1], input, strlen(input) + 1);
		close(fd1[1]);

		close(fd2[1]);

		read(fd2[0], input, 32);
				wait(NULL);

		close(fd2[0]);
	}

	// child process
	else
	{
		close(fd1[1]);

		read(fd1[0], input, 20);
		close(fd1[0]);
		close(fd2[0]);

		
		write(fd2[1], md5(input).c_str() ,32);
		kill(getppid(), SIGHUP);

		close(fd2[1]);

		exit(0);
	}
}
