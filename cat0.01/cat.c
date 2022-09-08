#include <stdio.h>

const int consoleArgumentsStart = 1;

int main(int argc, char *argv[]) {
	if (argc <= consoleArgumentsStart) {
		printf("Вы не ввели название файла\n");
		return 1;
	}
	for (int i = consoleArgumentsStart; i < argc; ++i) {
		FILE *fp;
		fp = fopen(argv[i], "r");
		if (fp) {
			char c = fgetc(fp);
			printf("<================================== ");
			printf(argv[i]);
			printf(" ==================================>\n");
			while (c != EOF) {
				printf("%c", c);
				c = fgetc(fp);
			}
			fclose(fp);
		} else {
			printf("Файл по пути %s не может быть найден", argv[i]);
		}
		printf("\n");
	}
	return 0;
}
