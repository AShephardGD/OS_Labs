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

void longPrint(char* dir, char* filename) {
	int pathLen = strlen(dir) + 1 + strlen(filename);
	char* path = (char*) calloc(pathLen, sizeof(char));
	strcat(path, dir);
	strcat(path, "/");
	strcat(path, filename);
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

void lsFile(char* path, char* filename) {
	if ((!isAll && (filename[0] != '.')) || isAll) {
		if (isLong) {
			longPrint(path, filename);
		}
		printf("%s\n", filename);
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
			lsFile(path, filename);
			dir = readdir(d);
		}
		closedir(d);
	} else {
		lsFile(dirname(path), basename(path));
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
	int i = consoleArgumentsStart + isAll + isLong;
	if (i != argc) {
		for (; i < argc; ++i) {
			ls(argv[i]);
		}
	} else {
		ls("./");
	}
	return 0;
}
