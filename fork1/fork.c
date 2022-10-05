#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void atexitHandler() {
	printf("Terminator №%d: I'll be back\n", getpid());
}

void sigintHandler() {
	printf("Caught signal SIGINT\n");
	exit(1);
}

void sigtermHandler(int sign_no) {
	if (sign_no == SIGTERM) {
		printf("Caught signal SIGTERM\n");
		exit(1);
	}
	else {
		printf("%d", sign_no);
	}
}

void forkExe() {
	pid_t pid;
	switch(pid = fork()) {
		case -1:
			perror("forkExe()");
			exit(1);
		case 0:
			printf("Child-процесс №%d, порожденный процессом №%d\n", getpid(), getppid());
			break;
		default:
			printf("Parent-процесс №%d, порождающий child-процесс\n", getpid());
			break;
	}
}

void atExitExe() {
	if (atexit(atexitHandler)) {
		exit(1);
	}
}

void signalHandler() {
	signal(SIGINT, sigintHandler);
	struct sigaction action;
	action.sa_handler = sigtermHandler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGTERM, &action, 0);
}

int main() {
	forkExe(); //Вызов fork() и корректная обработка всех возвращаемых им значений.
	atExitExe(); // Использование своего обработчика atexit().
	signalHandler(); // переопределение обработчика сигнала SIGINT при помощи системного выхова signal() и обработчика сигнала SIGTERM при помощи вызова sigaction(). (вывод описания полученного сигнала).
	while (1);
	return 0;
}
