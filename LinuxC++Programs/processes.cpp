//Andrew VanKooten
//October 19, 2015

#include <iostream>
#include <unistd.h>
#include <stdlib.h>


enum {RD, WR};

int main(int argc, char *argv[]) {

	// Exit the program if there is not one command line argument
	if (argc != 2) {
		std::cerr << "Incorrect ammount of command line arguments" << std::endl;
		exit(EXIT_FAILURE);
	}

	int p1[2], p2[2];
	pipe(p2);
	pid_t child;
	child = fork();

	// Something went wrong. oopsy daisy
	if (child < 0) {
		std::cerr << "ERROR" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Run the child processes
	else if (child == 0) {
		pipe(p1);
		child = fork();

		// Something went wrong. abandon ship!
		if (child < 0) {
			std::cerr << "ERROR" << std::endl;
			exit(EXIT_FAILURE);
		}

		// Run the grandchild processes
		// Get all the processes currently running and put them in the pipe
		else if (child == 0) {
			close(p1[RD]);
			dup2(p1[WR], STDOUT_FILENO);
			execlp("ps", "ps", "-A", NULL);
		}

		// Continue the child process
		// Wait until the grandchild is complete and then search all the current
		//	processes for the ones that contain whatever is in argv[1] and pipe it
		else {
			wait(child);
			close(p1[WR]);
			dup2(p1[RD], STDIN_FILENO);
			close(p2[RD]);
			dup2(p2[WR], STDOUT_FILENO);
			execlp("grep", "grep", argv[1], NULL);
		}
	}

	// Continue the parent processes
	// Wait until the child is complete and then count the lines
	else {
		wait(child);
		close(p2[WR]);
		dup2(p2[RD], STDIN_FILENO);
		execlp("wc", "wc", "-l", NULL);
	}

	exit(EXIT_SUCCESS);
}

