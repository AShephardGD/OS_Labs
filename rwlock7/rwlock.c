#include<pthread.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>

char arr[] = {'a', 0};
char whileFlag = 1;

const size_t threadCount = 11;

pthread_rwlock_t lock;

void end() {
	whileFlag = 0;
}

void* writeThread() {
	while (whileFlag) {
		pthread_rwlock_wrlock(&lock);
		printf("Writer: pthread_rwlock_wrlock()\n");
		arr[0] = arr[0] + 1;
		sleep(5);
		printf("Writer: pthread_rwlock_unlock()\n");
		pthread_rwlock_unlock(&lock);
		sleep(5);
	}
	return NULL;
}

void* readThread() {
	pthread_t tid = pthread_self();
	while (whileFlag) {
		pthread_rwlock_rdlock(&lock);
		printf("Reader: pthread_rwlock_rdlock()\n");
		printf("%lx: %s\n", tid, arr);
		printf("Reader: pthread_rwlock_unlock()\n");
		sleep(1);
		pthread_rwlock_unlock(&lock);
		sleep(5);
	}
	return NULL;
}

void rwlockLab() {
	signal(SIGINT, end);
	signal(SIGABRT, end);
	pthread_rwlock_init(&lock, NULL);
	pthread_t threads[threadCount];
	pthread_create(&threads[0], NULL, writeThread, NULL);
	for (size_t i = 1; i < threadCount; ++i) {
		pthread_create(&threads[i], NULL, readThread, NULL);
	}
	for (size_t i = 0; i < threadCount; ++i) {
		pthread_join(threads[i], NULL);
	}
	pthread_rwlock_destroy(&lock);
}

int main() {
	rwlockLab(); // Сщздать 10 читающих потоков и 1 пищущий.
	             // Пищущий поток записывает в общий массив номер записи (монотонно возрастающий счетчик), а читающие потоки выводят на экран свой tid и текущее состояние общего массив.
	             // Доступ должен быть синхронизирован с помощью блокировок чтения записи.
	return 0;
}
