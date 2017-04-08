#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define LENGTH 128

pid_t pid = -1;
char blocked = 1;

void sig_hndl(int sig)
{
	if (blocked == 1 && pid > 0)
		kill(pid, SIGKILL);
	write(1, "\n>", 3);
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
			if (strcmp(pargs, "exit") == 0)
				exit(0);
			pargs = strtok_r(NULL, " ", &saveptr);
		}
		args[i] = 0;
		pid = fork();

		if (pid == -1) {
			strcpy(str, "Some error in \'fork\' occures :(\n");
			write(2, str, strlen(str));
			exit(1);
		} else if (pid == 0) {
			if (blocked == 0)
				setsid();
			i = execvp(buff, args);
			if (i == -1) {
				strcpy(str, "Cant exec this! }:[\n");
				write(2, str, strlen(str));
				exit(1);
			}
		} else {
			if (blocked == 1) {
				pid_t chpid;

				chpid = waitpid(pid, &i, 0);
				if (chpid != pid) {
					strcpy(str, "Wrong child died\n");
					write(2, str, strlen(str));
				}
			}
			pid = 0;
		}
	}
	return 0;
}
