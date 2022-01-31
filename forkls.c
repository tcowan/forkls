/*
	forkls - fork a child process and by default, the child calls
		ls -al.  If you specify up to two parameters, the first 
		parameter is the program to be called and the second
		will be its sole parameter passed.

	Author: Ted Cowan, January 2017

*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	int rc;
	int status;
	char *cmd;
	char *myArgv[3];

	// set default program or specified program to exec()
	if (argc > 1) {
		cmd = argv[1];
		myArgv[0] = cmd;
		myArgv[1] = argv[2];
		myArgv[2] = NULL;
	} else {
		cmd = "ls";
		myArgv[0] = cmd;
		myArgv[1] = "-al";
		myArgv[2] = NULL;
	}

	printf("Parent: I am pid %d\n", getpid());

	rc = fork();

	if (rc > 0) {			// I am the parent
		printf("Parent: waiting on pid %d\n", rc);
		waitpid(rc, &status, 0);
		printf("Parent: child has ended with status %d\n", 
			WEXITSTATUS(status));
		exit(0);
	} else if (rc == 0) {	// I am the child
		printf("Child: I am pid %d about to exec() %s\n", 
			getpid(), cmd);
		execvp(cmd, myArgv);
		perror("Child: execvp() failed");
		printf("Child: execvp() failed...exiting\n");
		exit(1);
	} else {				// an error occured, no child created
		perror("Parent: a bad thing happened:");
		exit(1);
	}
		
}
