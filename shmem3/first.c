#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include<signal.h>
#include<string.h>

const size_t size = 256;
const int id = 228;
const char* path = "./shmem";

int whileFlag = 1;

char* getCurrentTime() {
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);
	return asctime(info);
}

void sigHandler() {
	whileFlag = 0;
}

void first() {
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	key_t key = ftok(path, id);
	int shmid = shmget(key, size, 0660 | IPC_CREAT | IPC_EXCL);
	if ((shmid == -1) && (errno == EEXIST)) {
		printf("Программа уже запущена!\n");
		exit(1);
	}
	else if (shmid == -1) {
		printf("Не удалось получить разделяемую память");
		exit(1);
	}
	char* mem = (char*) shmat(shmid, NULL, 0);
	while (whileFlag) {
		char str[size];
		sprintf(str, "%d: %s", getpid(), getCurrentTime());
		printf("%s", str);
		memset(mem, 0, size);
		strcpy(mem, str);
		sleep(5);
	}
	shmdt(mem);
	shmctl(shmid, IPC_RMID, NULL);
}

int main() {
	first(); // Создать две независимых программы.
	         // В бесконечном цикле передавать из первого процесса (первая программа) через разделяемую память текущее время и pid передающего процесса.
	         // Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку.
	         // Обязательно:
	         // Вторую программу можно запускать параллельно несколько раз, первая программа (передающая) должна запускаться только один раз.
	         // Повторный запуск первой программы, если уже есть процесс с такой программой, должен завершаться с соответствующим сообщением на экране.
	return 0;
}
