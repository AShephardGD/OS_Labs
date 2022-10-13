#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>

const int consoleArgumentsStart = 1;

int isAll = 0;
int isLong = 0;

void resetColor() {
	printf("\033[0m");
}

void blue() {
	printf("\033[0;34m");
}

void green() {
	printf("\033[0;32m");
}

void cyan() {
	printf("\033[0;36m");
}

void colorName(char* path) {
	struct stat pathStat;
	stat(path, &pathStat);
	mode_t m = pathStat.st_mode;
	if (S_ISDIR(m)) blue();
	else if (S_ISLNK(m)) cyan();
	else if ((m & S_IFREG) && (m & (S_IXUSR | S_IXGRP | S_IXOTH))) green();
	printf("%s\n", basename(path));
	resetColor();
}

char* getStrFromTime(time_t time) {
	int len = 21;
	char* buff = (char*) malloc(len * sizeof(char));
	strftime(buff, len, "%Y %b %d %H:%M:%S", localtime(&time));
	return buff;
}

char* getUsernameById(uid_t id) {
	struct passwd* u = getpwuid(id);
	return u ? u->pw_name : NULL;
}

char* getGroupById(gid_t id) {
	struct group* g = getgrgid(id);
	return g ? g->gr_name : NULL;
}

void longPrint(char* path) {
	struct stat pathStat;
	stat(path, &pathStat);
	mode_t m = pathStat.st_mode;

	if      (S_ISREG(m))  printf("-");
	else if (S_ISDIR(m))  printf("d");
	else if (S_ISBLK(m))  printf("b");
	else if (S_ISCHR(m))  printf("c");
	else if (S_ISLNK(m))  printf("l");
	else if (S_ISFIFO(m)) printf("p");
	else                  printf("s");	
	
	char fileAccess[] = "---------";
	fileAccess[0] = (S_IRUSR & m) ? 'r' : '-';
	fileAccess[1] = (S_IWUSR & m) ? 'w' : '-';
	fileAccess[2] = (S_IXUSR & m) ? 'x' : '-';
	fileAccess[3] = (S_IRGRP & m) ? 'r' : '-';
	fileAccess[4] = (S_IWGRP & m) ? 'w' : '-';
	fileAccess[5] = (S_IXGRP & m) ? 'x' : '-';
	fileAccess[6] = (S_IROTH & m) ? 'r' : '-';
	fileAccess[7] = (S_IWOTH & m) ? 'w' : '-';
	fileAccess[8] = (S_IXOTH & m) ? 'x' : '-';
	printf("%s", fileAccess);

	printf(" ");
	printf("%ld", pathStat.st_nlink);
	printf(" ");

	char* u = getUsernameById(pathStat.st_uid);
	if (u) {
		printf("%s", u);
	}
	printf(" ");

	char* g = getGroupById(pathStat.st_gid);
	if (g) {
		printf("%s", g);
	}
	printf(" ");

	printf("%ld", pathStat.st_size);
	printf(" ");

	char* time = getStrFromTime(pathStat.st_mtime);
	printf("%s", time);
	printf(" ");
	free(time);
}

void lsFile(char* path) {
	if ((!isAll && (basename(path)[0] != '.')) || isAll) {
		if (isLong) {
			longPrint(path);
		}
		colorName(path);
	}
}

void ls(char* path) {
	DIR* d;
	d = opendir(path);
	printf("<========================= %s =========================>\n", path);
	if (d) {
		struct dirent* dir = readdir(d);
		while(dir) {
			char* filename = dir->d_name;
			int pLen = strlen(path) + 1 + 1 + strlen(filename) + 1;
			char* p = calloc(pLen, sizeof(char));
			strcat(p, path);
			strcat(p, "/");
			strcat(p, filename);
			lsFile(p);
			free(p);
			dir = readdir(d);
		}
		closedir(d);
	} else {
		lsFile(path);
	}
}

void handlingArguments(int argc, char **argv) {
	opterr = 0;
	char arguments[] = "la";
	int res = getopt(argc, argv, arguments);
	while (res != -1) {
		switch (res) {
			case 'l':
				isLong = 1;
				break;
			case 'a':
				isAll = 1;
				break;
			default:
				printf("У утилиты есть только следующие параметры: \"%s\".\n", arguments);
				exit(1);
		}
		res = getopt(argc, argv, arguments);
	}
}

int main(int argc, char **argv) {
	handlingArguments(argc, argv);
	int curDir = 1;
	for (int i = consoleArgumentsStart; i < argc; ++i) {
		if (argv[i][0] != '-') {
			ls(argv[i]);
			curDir = 0;
		}
	}
	if (curDir) {
		ls("./");
	}
	return 0;
}
