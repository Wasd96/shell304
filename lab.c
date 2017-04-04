#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LENGTH 128

pid_t pid = -1;
char blocked = 1;

void sig_hndl(int sig)
{
	write(1,"**", 4);
	if (blocked == 1) {
		if (pid > 0) {
			kill(pid, SIGKILL);
			exit(0);
		}
	}
}

int main(int argc, char *argv[])
{
	char str[LENGTH];

	signal(SIGINT, sig_hndl);
	strcpy(str, "Welcome to Shell304!\nTell me what do you want :)\n");
	write(1, str, strlen(str));
	while (1) {
		int nread = 0;
		char buff[LENGTH];
		char *args[LENGTH];
		char *pargs;
		char *saveptr;
		int i = 0;

		write(1, ">", 2);
		nread = read(0, buff, LENGTH);
		if (nread == -1) {
			strcpy(str, "Some error in \'read\' occures :(\n");
			write(2, str, strlen(str));
			exit(1);
		}
		if (nread > 0)
			buff[nread-1] = 0;
		pargs = strtok_r(buff, " ", &saveptr);
		blocked = 1;
		while (pargs != NULL) {
			if (strcmp(pargs, "&") == 0) {
				blocked = 0;
			} else {
				args[i] = pargs;
				i++;
			}
			pargs = strtok_r(NULL, " ", &saveptr);
		}
		args[i] = 0;
		pid = fork();

		if (pid == -1) {
			strcpy(str, "Some error in \'fork\' occures :(\n");
			write(2, str, strlen(str));
			exit(1);
		} else if (pid == 0) {
			i = execvp(buff, args);
			if (i == -1) {
				strcpy(str, "Cant exec this! }:[\n");
				write(2, str, strlen(str));
				exit(1);
			}
		} else {
			if (blocked == 1)
			{
				int status;
				pid_t chpid;
				chpid = wait(&status);
				if (WIFEXITED(status))
					write(1, "norm", 5);
				else
					write(1, "nenorm", 7);
				write(1, "w", 2);
printf("%i %i\n", status, chpid);
			}
			else
				write(1, "nw", 3);
		}
		
	}
	return 0;
}
