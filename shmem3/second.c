#include<time.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

const size_t size = 256;
const int id = 228;
const char* path = "./shmem";

char* getCurrentTime() {
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);
	return asctime(info);
}

void second() {
	key_t key = ftok(path, id);
	int shmid = shmget(key, size, 0);
	if (shmid == -1) {
		printf("Запустите передающий процесс!\n");
		exit(1);
	}
	char* mem = (char*) shmat(shmid, NULL, 0);
	printf("%d: %s%s\n", getpid(), getCurrentTime(), mem);
	shmdt(mem);
}

int main() {
	second(); // Создать две независимых программы.
	          // В бесконечном цикле передавать из первого процесса (первая программа) через разделяемую память текущее время и pid передающего процесс.
	          // Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку.
	          // Обязательно:
	          // Вторую программу можно запускать параллельно несколько раз, первая программа (передающая) должна запускаться только один раз.
	          // Повторный запуск первой программы, если уже есть процесс с такой программой, должен завершаться с соответствующим сообщением на экране.
	return 0;
}
