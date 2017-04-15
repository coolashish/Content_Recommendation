#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CATEGORY_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/documents_entities.csv"
#define TOPIC_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/documents_topics.csv"
#define PAGE_VIEWS_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/page_views_sample.csv"
#define CATEGORY_OP "./category_count.csv"
#define TOPIC_OP "./topic_count.csv"
#define TABLE_SIZE 2999335
typedef struct node{
	char category[64];
	double confidence;
	struct node* next;
}node;
node* createnode(char *category, double confidence){
	node* newnode;
	newnode = (node *)malloc(sizeof(node));
	if(newnode == NULL){
		fprintf(stderr, "MEMORY ISSUE");
		exit(-1);
	}
	strcpy(newnode->category, category);
	newnode->confidence = confidence;
	return newnode;
}
int main(){
	int document, i;
	int document1, document2;
	char c[64], user[33];
	double confidence;
	int count;
	char category[64];
	node* newnode;
	FILE *fp;

	node** cat_table;
	node** topic_table;

	cat_table = (node **)calloc(TABLE_SIZE, sizeof(node *));
	if(cat_table == NULL){
		fprintf(stderr, "MEMORY ISSUE");
	}
	//opening file documents_categories.csv
	fp = fopen(CATEGORY_CSV, "r");
	if(fp == NULL){
		perror("CATEGORY_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	while(fscanf(fp, "%d", &document) != -1){
		//Following line parses a csv

		fscanf(fp, ",%[^,],%lf\n", category, &confidence);
		newnode = createnode(category, confidence);
		newnode->next = cat_table[document];
		cat_table[document] = newnode;
	}
	fclose(fp);
	//HASH TABLE created
	
	printf("CATEGORY CREATED\n");

	fp = fopen(PAGE_VIEWS_CSV, "r");
	if(fp == NULL){
		perror("PAGE_VIEWS_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	printf("entity,confidence\n");
	while(fscanf(fp, "%[^,],%d,%s\n", user, &document, c) != -1){
		count++;
		newnode = cat_table[document];
		while(newnode){
			printf("%.32s,%.12lf\n",newnode->category, newnode->confidence);
			newnode = newnode->next;
		}
		if(count%100000 == 0){
			fprintf(stderr, "ROW %d\n", count);
		}
	}
	fclose(fp);
}
