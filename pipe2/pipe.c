#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

size_t bufCount = 256;

char* strcatHeap(const char* str1, const char* str2) {
	char* res = calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
	strcat(res, str1);
	strcat(res, str2);
	return res;
}

char* getCurrentTime() {
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);
	return asctime(info);
}

void readTask(int fd) {
	unsigned int sec = 14; // 1 + 3 + 3 + 7 = 14
	while ((sec = sleep(sec)));
	char buf[bufCount];
	memset(buf, 0, bufCount);
	read(fd, buf, bufCount);
	printf("Mine pid & time - %d & %sParent pid & time - %s", getpid(), getCurrentTime(), buf);
	close(fd);
}

void readFTask(int fd) {
	unsigned int sec = 14; // 1 + 3 + 3 + 7 = 14
	while ((sec = sleep(sec))) {}
	FILE* stream = fdopen(fd, "r");
	if (stream) {
		char* line = NULL;
		size_t len = 0;
		getline(&line, &len, stream);
		fclose(stream);
		printf("Mine pid & time - %d & %sParent pid & time - %s", getpid(), getCurrentTime(), line);
		free(line);
	}
}

void writeTask(int fd, int pid) {
	int pidStringLen = (int) ((ceil(log10(pid)) + 1));
	char pidString[pidStringLen];
	sprintf(pidString, "%d", pid);
	char* res = strcatHeap(pidString, " & ");
	char* res2 = strcatHeap(res, getCurrentTime());
	write(fd, res2, bufCount);
	close(fd);

}


void writeFTask(int fd, int pid) {
	int pidStringLen = (int) ((ceil(log10(pid)) + 1));
	char pidString[pidStringLen];
	sprintf(pidString, "%d", pid);
	char* res = strcatHeap(pidString, " & ");
	char* res2 = strcatHeap(res, getCurrentTime());
	FILE* stream = fdopen(fd, "w");
	if (stream) {
		fprintf(stream, res2);
		fclose(stream);
	} else {
		perror("Can't open file");
	}
	free(res);
	free(res2);
	int status;
	wait(&status);
}

void pipeTask() {
	pid_t pid;
	int pipes[2];
	
	if (pipe(pipes)) {
		perror("Can't pipe");
		exit(1);
	}
	
	switch (pid = fork()) {
		case -1:
			perror("Fork");
			exit(1);
		case 0:
			close(pipes[1]);
			readTask(pipes[0]);
			exit(0);
		default:
			close(pipes[0]);
			writeTask(pipes[1], getpid());
			wait(NULL);
	}
}

void fifoTask() {
	pid_t pid;
	int fd_fifo;
	char* fifoName = "./f";

	unlink(fifoName);
	if (mkfifo(fifoName, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) {
		perror("Не удалось создать FIFO");
		exit(1);
	}
	if ((fd_fifo = open(fifoName, O_RDWR)) == -1) {
		perror("Не удалось открыть FIFO");
		exit(1);
	}
	
	switch (pid = fork()) {
		case -1:
			perror("Fork");
			exit(1);
		case 0:
			readTask(fd_fifo);
			exit(0);
		default:
			writeTask(fd_fifo, getpid());
			wait(NULL);
			unlink(fifoName);
	}
}

int main() {
	printf("Pipe Task:\n");
	pipeTask(); // Создать pipe. Создать дочерний процесс. Передать строчку через pipe из родительского в дочерний процесс с текщим временем и pid передающего процесса. Дочерний процесс должен вывести на экран его текущее время и полученную через pipe строчку. Время из родительского и дочернего должно различаться как минимум на 5 секунд.

	unsigned int sec = 5;
	while ((sec = sleep(sec))) {}

	printf("\nFifo Task:\n");
	fifoTask(); // Аналогично предыдущему, но использовать fifo

	return 0;
}
