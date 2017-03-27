#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
	char str[128];

	strcpy(str, "Welcome to Shell304!\nTell me what do you want :)\n");
	write(1, str, strlen(str));
	while (1) {
		int nread = 0;
		char buff[128];
		char *args[20];
		char *pargs;
		char *saveptr1;
		pid_t pid;
		int i = 0;

		write(1, ">", 2);
		nread = read(0, buff, 128);
		if (nread == -1) {
			strcpy(str, "Some error in \'read\' occures :(\n");
			write(2, str, strlen(str));
			exit(1);
		}
		if (nread > 0)
			buff[nread-1] = 0;
		pargs = strtok_r(buff, " ", &saveptr1);
		while (pargs != NULL) {
			args[i] = pargs;
			i++;
			pargs = strtok_r(NULL, " ", &saveptr1);
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
			}
			exit(0);
		} else {
			wait(NULL);
		}
	}
	return 0;
}
