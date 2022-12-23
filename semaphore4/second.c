#include<time.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>

const size_t size = 256;
const int id = 228;
const char* path = "./shmem";
const char* semName = "/yomom";

char* getCurrentTime() {
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);
	return asctime(info);
}

void secondOld() {
	key_t key = ftok(path, id);
	int shmid = shmget(key, size, 0);
	if (shmid == -1) {
		printf("Запустите передающий процесс!\n");
		exit(1);
	}
	sem_t* sem = sem_open(semName, 0);
	printf("Second: sem_wait()\n");
	sem_wait(sem);
	char* mem = (char*) shmat(shmid, NULL, 0);
	printf("%d: %s%s", getpid(), getCurrentTime(), mem);
	shmdt(mem);
	sleep(5);
	printf("Second: sem_post()\n");
	sem_post(sem);
}

void second() {
	key_t key = ftok(path, id);
	int semid = semget(key, 1, 0);

	int shmid = shmget(key, size, 0);
	if (shmid == -1 && semid == -1) {
		printf("Запустите передающий процесс!\n");
		exit(1);
	}

	struct sembuf wait, plus, minus;
	wait.sem_num = 0;
	wait.sem_op = 0;
	wait.sem_flg = 0;
	plus.sem_num = 0;
	plus.sem_op = 1;
	plus.sem_flg = 0;
	minus.sem_num = 0;
	minus.sem_op = -1;
	minus.sem_flg = 0;

	char* mem = (char*) shmat(shmid, NULL, 0);
	
	printf("Second: sem wait\n");
	semop(semid, &wait, 1);

	printf("Second: +semop()\n");
	semop(semid, &plus, 1);
	printf("Second(%d): %s%s", getpid(), getCurrentTime(), mem);

	sleep(10);

	printf("Second: -semop()\n");
	semop(semid, &minus, 1);

	shmdt(mem);
}

int main() {
	second(); // Создать две независимых программы.
	          // В бесконечном цикле передавать из первого процесса (первая программа) через разделяемую память текущее время и pid передающего процесса.
	          // Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку.
	          // Обязательно:
	          // Вторую программу можно запускать параллельно несколько раз, первая программа (передающая) должна запускать только один раз.
	          // Повторный запуск первой программы, если уже есть процесс с такой программой, должен завершаться с соответствующим сообщением на экране.
	          // Для синхронизации обмена данными использовать semaphore.
	return 0;
}
