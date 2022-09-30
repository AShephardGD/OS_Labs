#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const int consoleArgumentsStart = 1;

char* filename = 0;

void grep(char* substring, FILE* from) {
	char* line = 0;
	size_t len = 0;
	ssize_t lineSize = 0;
	lineSize = getline(&line, &len, from);
	while (lineSize != -1) {
		if (strstr(line, substring)) {
			printf("%s", line);
		}
		lineSize = getline(&line, &len, from);
	}
	fclose(from);
}

void handlingArguments(int argc, char **argv) {
	opterr = 0;
	char arguments[] = "f:";
	int res = getopt(argc, argv, arguments);
	while (res != -1) {
		switch (res) {
			case 'f':
				filename = optarg;
				break;
			default:
				printf("У утилиты есть только следующие параметры: \"%s\".\n", arguments);
				exit(1);
		}
		res = getopt(argc, argv, arguments);
	}
}

int main(int argc, char **argv) {
	if (argc <= consoleArgumentsStart) {
		printf("Вы не ввели слово, которое нужно найти.\n");
		return 1;
	}
	handlingArguments(argc, argv);
	int i = consoleArgumentsStart + 2 * ((int) (filename != 0));
	for (; i < argc; ++i) {
		FILE* from = filename? fopen(filename, "r") : stdin;
		grep(argv[i], from);
	}
	return 0;
}
