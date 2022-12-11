#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

char arr[] = {'a', 0};
const size_t threadCount = 11;
pthread_mutex_t mutex;

void* writeThread() {
	while (1) {
		pthread_mutex_lock(&mutex);
		printf("Writer: pthread_mutex_lock()\n");
		arr[0] = arr[0] + 1;
		sleep(1);
		printf("Writer: pthread_mutex_unlock()\n\n");
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	return NULL;
}

void* readThread() {
	pthread_t tid = pthread_self();
	while (1) {
		pthread_mutex_lock(&mutex);
		printf("Reader: pthread_mutex_lock()\n");
		printf("%ld: %s\n", tid, arr);
		sleep(1);
		printf("Reader: pthread_mutex_unlock()\n\n");
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	return NULL;
}

void mutexLab() {
	pthread_mutex_init(&mutex, NULL);
	pthread_t threads[threadCount];
	pthread_create(&threads[0], NULL, writeThread, NULL);
	for (size_t i = 1; i < threadCount; ++i) {
		pthread_create(&threads[i], NULL, readThread, NULL);
	}
	for (size_t i = 0; i < threadCount; ++i) {
		pthread_join(threads[i], NULL);
	}
}

int main() {
	mutexLab(); // Создать 10 читающих потоков и 1 пищущий.
	            // Пищущий поток записывает в общий масссив номер записи (монотонно возрастающий счетчик), а читающие потоки выводят на экран свой tid и текущее состояние общего массива.
	            // Доступ должен быть синхронизирован с помощью mutex.
	return 0;
}
