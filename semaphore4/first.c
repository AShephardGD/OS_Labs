#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<errno.h>
#include<signal.h>
#include<string.h>
#include<fcntl.h>
#include<semaphore.h>

const size_t size = 256;
const int id = 228;
const char* path = "./shmem";
const char* semName = "/yomom";

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

void firstOld() {
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	key_t key = ftok(path, id);
	int shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
	if ((shmid == -1) && (errno == EEXIST)) {
		printf("Программа уже запущена!\n");
		exit(1);
	}
	else if (shmid == -1) {
		printf("Не удалось получить разделяемую память");
		exit(1);
	}
	sem_unlink(semName);
	sem_t* sem = sem_open(semName, O_CREAT, 0666, 1);
	if (sem == SEM_FAILED) {
		perror("SEM_FAILED");
		exit(1);
	}
	char* mem = (char*) shmat(shmid, NULL, 0);
	while (whileFlag) {
		printf("First: sem_wait()\n");
		sem_wait(sem);
		char str[size];
		sprintf(str, "%d: %s", getpid(), getCurrentTime());
		printf("%s", str);
		memset(mem, 0, size);
		strcpy(mem, str);
		sleep(5);
		printf("First: sem_post()\n\n");
		sem_post(sem);
		sleep(5);
	}
	shmdt(mem);
	shmctl(shmid, IPC_RMID, NULL);
}

void first() {
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	key_t key = ftok(path, id);
	int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (semid == -1) {
		printf("Не удалось создать семафор\n");
		exit(1);
	}

	int shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
	if ((shmid == -1) && (errno == EEXIST)) {
		printf("Программа уже запущена!\n");
		exit(1);
	}
	else if (shmid == -1) {
		printf("Не удалось получить разделяемую память");
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
	while (whileFlag) {
		printf("First: sem wait\n");
		semop(semid, &wait, 1);

		printf("First: +semop()\n");
		semop(semid, &plus, 1);
		
		char str[size];
		sprintf(str, "%d: %s", getpid(), getCurrentTime());
		printf("%s", str);
		memset(mem, 0, size);
		strcpy(mem, str);

		sleep(5);

		printf("First: -semop()\n");
		semop(semid, &minus, 1);
		printf("\n");

		sleep(5);
	}

	shmdt(mem);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, IPC_RMID, 0);
}

int main() {
	first(); // Создать две независимые программы.
	         // В бесконечном цикле передавать из первого процесса (первая программа) через разделяемую память текущее время и pid передающего процесса.
	         // Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку.
	         // Обязательно:
	         // Вторую программу можно запускать параллельно несколько раз, первая программа (передающая) должна запускаться только один раз.
	         // Повторный запуск первой программы, если уже есть процесс с такой программой, должен завершаться с соответствующим сообщением на экране.
	         // Для синхронизации обмена данными использовать semaphore.
	return 0;
}
