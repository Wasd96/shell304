#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void exec(char *const argv[], char *str, char *buff)
{
	strcpy(str, "/bin/");
	strcat(str, buff);
	execv(str, argv);
	exit(0);
}

int main(void)
{
	char str[128];

	strcpy(str, "Welcome to Shell304!\nI'll perform all you wishes...\n");
	write(1, str, strlen(str));
	while (1) {
		int nread = 0;
		char buff[128];
		char args[20][20];
		char *pargs;
		pid_t pid;
		int i = 0;

		write(1, ">", 2);
		nread = read(0, buff, 128);
		if (nread == -1) {
			strcpy(str, "Some error in \'read\' occures :(");
			write(2, str, strlen(str));
			exit(1);
		}
		buff[nread-1] = 0;
		pargs = strtok(buff, " ");
		while (pargs != NULL) {
			pargs = strtok(NULL, " ");
			args[i][0] = pargs;
			//write(1, args[i], strlen(args[i]));
			write(1, pargs, strlen(pargs));
			i++;
		}
		args[i][0] = 0;
		pid = fork();
		if (pid == -1) {
			strcpy(str, "Some error in \'fork\' occures :(");
			write(2, str, strlen(str));
			exit(1);
		} else if (pid == 0) {
			write(1, buff, strlen(buff));
			write(1, str, strlen(str));
			exec(args, str, buff);
		} else {
			wait(NULL);
		}
	}
	return 0;
}
