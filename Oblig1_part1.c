#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* fork */
#include <sys/wait.h> /* waitpid */
#include <sys/types.h> /* pid_t */

void process(int number, int time) {
	printf("Prosess %d kjorer\n", number);
	sleep(time);
	printf("Prosess %d kjorte i %d sekunder\n", number, time);
}

int main(void) {
	pid_t pid0, pid1, pid2, pid3, pid4, pid5;

	pid0 = fork();
	if (!pid0) {
	  process(0, 1);
	  exit(0);
	}
	
	pid2 = fork();
	if (!pid2) {
	  process(2, 3);
	  exit(0);
	}
	
	waitpid(pid0, NULL, 0);
	
	pid1 = fork();
	if (!pid1) {
	  process(1, 2);
	  exit(0);
	}
	
	pid4 = fork();
	if (!pid4) {
	  process(4, 3);
	  exit(0);
	}

	waitpid(pid2, NULL, 0);

	pid3 = fork();
	if (!pid3) {
	  process(3, 2);
	  exit(0);
	}
	
	waitpid(pid4, NULL, 0);

	pid5 = fork();
	if (!pid5) {
	  process(5, 3);
	  exit(0);
	}
	
	waitpid(pid5, NULL, 0);
}
