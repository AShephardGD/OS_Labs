#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

const int consoleArgumentsStart = 1;

char isNonBlank = 0;
char isShowEnds = 0;
char isNumber = 0;
char isSqueezeBlank = 0;

void printNaturalNumber(const size_t num, const size_t maxOrder) {
	size_t numOrder = 1;
	while ((size_t)(num / pow(10, numOrder))) {
		++numOrder;
	}
	for (size_t i = 0; i < maxOrder - numOrder; ++i) {
		printf(" ");
	}
	printf("%ld  ", num);
}

void printC(char c, char isLineFeed, size_t lineNumber) {
	size_t maxOrder = 10;
	if (isNumber && isLineFeed) {
		printNaturalNumber(lineNumber, maxOrder);
		++lineNumber;
	}
	if (isShowEnds && (c == '\n')) {
		printf("$");
	}
	printf("%c", c);
}

void cat(const char *path) {
	FILE *fp;
	fp = fopen(path, "r");
	size_t lineNumber = 1;
	char isLineFeed = 1;
	if (fp) {
		char c = fgetc(fp);
		printf("<================================== ");
		printf(path);
		printf(" ==================================>\n");
		while (c != EOF) {
			printC(c, isLineFeed, lineNumber);
			isLineFeed = (char) (c == '\n');
			lineNumber += isLineFeed;
			c = fgetc(fp);
			if (isNonBlank && isLineFeed && (c == '\n')) {
				printf("%c", c);
				c = fgetc(fp);
				while (c == '\n') {
					if (!isSqueezeBlank) {
						printf("%c", c);
					}
					c = fgetc(fp);
				}
			} else if (isSqueezeBlank && isLineFeed && (c == '\n')) {
				printC(c, isLineFeed, lineNumber);
				++lineNumber;
				c = fgetc(fp);
				while (c == '\n') {
					c = fgetc(fp);
				}
			}
		}
		fclose(fp);
	} else {
		printf("Файл по пути %s не может быть найден.\n", path);
	}
}

int handlingArguments(int argc, char *argv[]) {	
	int argCount = 0;
	opterr = 0;
	char arguments[] = "nsEb";
	int res = getopt(argc, argv, arguments);
	while (res != -1) {
		switch (res) {
			case 'n':
				isNumber = 1;
				++argCount;
				break;
			case 's':
				isSqueezeBlank = 1;
				++argCount;
				break;
			case 'E':
				isShowEnds = 1;
				++argCount;
				break;
			case 'b':
				isNonBlank = 1;
				isNumber = 1;
				++argCount;
				break;
			default:
				printf("У утилиты есть только следующие параметры: \"%s\".\n", arguments);
				exit(1);
		}
		res = getopt(argc, argv, arguments);
	}
	return argCount;
}

int main(int argc, char **argv) {
	for (int i = consoleArgumentsStart + handlingArguments(argc, argv); i < argc; ++i) {
		cat(argv[i]);
	}
	return 0;
}
