#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

char arr[] = {'a', 0};
char whileFlag = 1;

const size_t threadCount = 11;

pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void end() {
	whileFlag = 0;
}

void* writeThread() {
	while (whileFlag) {
		pthread_mutex_lock(&mutex);
		printf("Writer: pthread_mutex_lock()\n");
		arr[0] = arr[0] + 1;
		sleep(1);
		printf("Writer: pthread_mutex_unlock()\n\n");
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	pthread_cond_broadcast(&cond);
	return NULL;
}

void* readThread() {
	pthread_t tid = pthread_self();
	while (whileFlag) {
		pthread_mutex_lock(&mutex);
		printf("Reader: pthread_mutex_lock()\n");
		pthread_cond_wait(&cond, &mutex);
		printf("%lx: %s\n", tid, arr);
		sleep(1);
		printf("Reader: pthread_mutex_unlock()\n\n");
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	return NULL;
}

void condLab() {
	signal(SIGINT, end);
	signal(SIGABRT, end);
	pthread_mutex_init(&mutex, NULL);
	pthread_t threads[threadCount];
	pthread_create(&threads[0], NULL, writeThread, NULL);
	for (size_t i = 1; i < threadCount; ++i) {
		pthread_create(&threads[i], NULL, readThread, NULL);
	}
	for (size_t i = 0; i < threadCount; ++i) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
}

int main() {
	condLab(); // Создать 10 читающих потоков и 1 пищущий.
	           // Пищущий поток записывает в общий массив номер записи (монотонно возрастающий счетчик), а читающие потоки выводят на экран свой tid и текущее состояние общего массива.
	           // Доступ должен быть синхронизирован с помощью условных переменных.
	return 0;
}
