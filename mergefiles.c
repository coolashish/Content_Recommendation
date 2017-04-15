#include <stdio.h>
int main(int argc, char *argv[]){
	char c[2048];
	FILE *fp1, *fp2, *fp3;
	fp1 = fopen(argv[1], "r");
	if(fp1 == NULL){
		fprintf(stderr, "fopen failed");
		return -1;
	}
	fp2 = fopen(argv[2], "r");
	if(fp2 == NULL){
		fprintf(stderr, "fopen failed");
		return -1;
	}
	fp3 = fopen(argv[3], "r");
	if(fp3 == NULL){
		fprintf(stderr, "fopen failed");
		return -1;
	}
	fscanf(fp1, "%s\n", c);
	printf("%s,", c);
	fscanf(fp2, "%s\n", c);
	printf("%s,", c);
	fscanf(fp3, "%[^,],%[^,],%[^,],%[^,],%[^,],", c, c, c, c, c);
		fscanf(fp3, "%s\n", c);
		printf("%s\n",c);
	while(fscanf(fp1, "%s\n", c) != -1){
		printf("%s,",c);
		fscanf(fp2, "%s\n", c);
		printf("%s,",c);
	fscanf(fp3, "%[^,],%[^,],%[^,],%[^,],%[^,],", c, c, c, c, c);
		fscanf(fp3, "%s\n", c);
		printf("%s\n",c);
	}
}
