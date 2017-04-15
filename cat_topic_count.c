#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CATEGORY_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/documents_categories.csv"
#define TOPIC_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/documents_topics.csv"
#define PAGE_VIEWS_CSV "/media/D4DE7E1CDE7DF758/B.Tech_Project/Dataset_zips/page_views_sample.csv"
#define CATEGORY_OP "./category_count.csv"
#define TOPIC_OP "./topic_count.csv"
#define TABLE_SIZE 2999335
typedef struct node{
	int category;
	double confidence;
	struct node* next;
}node;
node* createnode(int category, double confidence){
	node* newnode;
	newnode = (node *)malloc(sizeof(node));
	if(newnode == NULL){
		fprintf(stderr, "MEMORY ISSUE");
		exit(-1);
	}
	newnode->category = category;
	newnode->confidence = confidence;
	return newnode;
}
double calculate_correlation(node *first, node *second){
	double result = 0;
	while(first){
		node *p = second;
		while(p){
			if(p->category == first->category){
				result += p->confidence * first->confidence;
			}
			p = p->next;
		}
		first = first->next;
	}
	return result;
}
int main(){
	int document, i;
	int document1, document2;
	char c[64], user[33];
	double confidence;
	int category, count;
	node* newnode;
	FILE *fp;

	node** cat_table;
	node** topic_table;

	double topic_count[300], cat_count[2101];
	cat_table = (node **)calloc(TABLE_SIZE, sizeof(node *));
	if(cat_table == NULL){
		fprintf(stderr, "MEMORY ISSUE");
	}
	topic_table = (node **)calloc(TABLE_SIZE, sizeof(node *));
	if(topic_table == NULL){
		fprintf(stderr, "MEMORY ISSUE");
	}
	
	for(i = 0; i < 300; i++){
		topic_count[i] = 0.0;
	}
	for(i = 0; i < 2101; i++){
		cat_count[i] = 0.0;
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

		fscanf(fp, ",%d,%lf\n", &category, &confidence);
		newnode = createnode(category, confidence);

		newnode->next = cat_table[document];
		cat_table[document] = newnode;
	}
	fclose(fp);
	//HASH TABLE created
	
	printf("CATEGORY CREATED\n");

	//opening file documents_categories.csv
	fp = fopen(TOPIC_CSV, "r");
	if(fp == NULL){
		perror("TOPIC_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	while(fscanf(fp, "%d", &document) != -1){
		//Following line parses a csv

		fscanf(fp, ",%d,%lf\n", &category, &confidence);
		newnode = createnode(category, confidence);
		newnode->next = topic_table[document];
		topic_table[document] = newnode;
	}
	fclose(fp);
	//HASH TABLE created
	
	printf("TOPIC CREATED\n");

	fp = fopen(PAGE_VIEWS_CSV, "r");
	if(fp == NULL){
		perror("PAGE_VIEWS_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	while(fscanf(fp, "%[^,],%d,%s\n", user, &document, c) != -1){
		count++;
		newnode = cat_table[document];
		while(newnode){
			cat_count[newnode->category] += newnode->confidence;
			newnode = newnode->next;
		}
		newnode = topic_table[document];
		while(newnode){
			topic_count[newnode->category] += newnode->confidence;
			newnode = newnode->next;
		}
		if(count%100000 == 0){
			fprintf(stderr, "ROW %d\n", count);
		}
	}
	fclose(fp);
	fp = fopen(CATEGORY_OP, "w+");
	if(fp  == NULL){
		perror("CATEGORY_OP FILE FAILED TO OPEN");
		exit(-1);
	}
	fprintf(fp, "category,popularity\n");
	for(i = 1; i < 2101; i++){
		fprintf(fp, "%d,%.12lf\n", i, cat_count[i]);
	}
	fclose(fp);
	fp = fopen(TOPIC_OP, "w+");
	if(fp  == NULL){
		perror("TOPIC_OP FILE FAILED TO OPEN");
		exit(-1);
	}
	fprintf(fp, "topic,popularity\n");
	for(i = 1; i < 300; i++){
		fprintf(fp, "%d,%.12lf\n", i, topic_count[i]);
	}
	fclose(fp);
	//Following part takes 2 document_id as input and calculate their correlation
}
