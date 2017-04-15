#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
	char str[] = "0 0:1:1 1:2:1";
	char *p;
	p = strtok(str, " \t");
	if (!p)
		printf("p is null\n");
	printf("%s\n", p);
	p = strtok(NULL, " \t");
	printf("%s\n", p);
	p = strtok(NULL, " \t");
	printf("%s\n", p);
	return 0;
}
