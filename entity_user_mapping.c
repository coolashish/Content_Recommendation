#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CATEGORY_CSV "./Datasets/documents_entities.csv"
#define EVENTS_CSV "./Datasets/events.csv"
#define PAGE_VIEWS_CSV "/home/hadoop/page_views.csv"
#define TABLE_SIZE 2999334
typedef struct node{
	char category[32];
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
	strncpy(newnode->category, category, 32);
	newnode->confidence = confidence;
	newnode->next = NULL;
	return newnode;
}
int insert_node(node *p, char *category, double confidence){
	if(strncmp(p->category, category, 32) == 0){
		p->confidence += confidence;
		return 0;
	}
	else if(p->next){
		return insert_node(p->next, category, confidence);
	}
	else{
		p->next = createnode(category, confidence); 
		return 0;
	}
}
void print_node(node *p){
	while(p){
		printf("%.5s\t%lf\n", p->category, p->confidence);
		p = p->next;
	}
}
typedef struct dict{
	node *data;
	struct dict* next[16];
}dict;
void init_dictionary(dict *p){
	int i;
	for(i = 0; i < 16; i++){
		p->next[i] = NULL;
	}
	p->data = NULL;
}
int insert_dictionary(dict *p, char *key, char *category, double confidence, int level){
	if(level < 14){
		if(p->next[*key] == NULL){
			p->next[*key] = calloc(1, sizeof(dict));
			if(p->next[*key] == NULL){
				fprintf(stderr, "MEMORY ISSUE\n");
			}
		}
		insert_dictionary(p->next[*key], key + 1, category, confidence, level + 1);
	}
	else{
		if(p->data == NULL){
			p->data = createnode(category, confidence); 
		}
		else{
			return insert_node(p->data, category, confidence);
		}
	}
}
double calculate_correlation(node *first, node *second){
	double result = 0;
	while(first){
		node *p = second; 
		while(p){
			if(strncmp(p->category, first->category, 32) == 0){
				result += p->confidence * first->confidence;
			}
			p = p->next;
		}
		first = first->next;
	}
	return result;
}
node *get_dictionary(dict *p, char *key, int level){
	if(p == NULL){
		return NULL;
	}
	if(level == 14){
		return p->data;
	}
	return get_dictionary(p->next[*key], key + 1, level + 1);

}
void print_dictionary(dict *d){
	if(d == NULL){
		return;
	}
	static int level = 0;
	int i;
	if(level == 14){
		print_node(d->data);
		return;
	}
	level++;
	for(i = 0; i < 16; i++){
		print_dictionary(d->next[i]);
	}
	level--;
}
int main(){
	int document;
	int read_document_id, ad_document_id;
	char c[256];
	double confidence;
	char category[64];
	node* newnode;
	FILE *fp;
	dict user_mapping;
	init_dictionary(&user_mapping);
	int i;
	node* index;

	int display_id;
	char user[16];
	int count = 0;
	char ad_id[64], clicked[64], campaign_id[64], advertiser_id[64],
	     ad_source_id[64], ad_publisher_id[64], ad_topic_id[64], ad_entity_id[64],
	     ad_category_id[64], uuid[64], platform[64], geo_location[64], read_source_id[64], 
	     read_publisher_id[64], read_topic_id[64], read_entity_id[64], read_category_id[64];
	double topic_correlation, entity_correlation, category_correlation;
	double user_ka_category_correlation, user_ka_topic_correlation, user_ka_entity_correlation;

	node** table;
	table = (node **)calloc(TABLE_SIZE, sizeof(node *));
	if(table == NULL){
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
		newnode->next = table[document];
		table[document] = newnode;
	}
	fclose(fp);
	//HASH TABLE created
	
	fprintf(stderr, "TABLE CREATED\n");

	fp = fopen(EVENTS_CSV, "r");
	if(fp == NULL){
		perror("CATEGORY_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	while(fscanf(fp, "%d", &display_id) != -1){
		count++;
		fscanf(fp, ",%[^,],%d,%s\n", user, &document, c);
		if(!(user[0] >= '0' && user[0] <= '8')){
			continue;
		}
		newnode = table[document];
		for(i = 0; user[i]; i++){
			if(user[i] >= '0' && user[i] <= '9'){
				user[i] -= '0';
			}
			else {
				user[i] += 10 - 'a';
			}
		}
		while(newnode){
			insert_dictionary(&user_mapping, user, newnode->category, newnode->confidence, 0);
			newnode = newnode->next;
		}
		if(count%100000 == 0){
			fprintf(stderr, "ROW %d\n", count);
		}
	}
	fclose(fp);
	fprintf(stderr, "EVENTS READ\n");


	fp = fopen(PAGE_VIEWS_CSV, "r");
	if(fp == NULL){
		perror("PAGE_VIEWS_CSV file failed to open");
		exit(-1);
	}
	fscanf(fp, "%s\n", c);
	while(fscanf(fp, "%[^,],%d,%s\n", user, &document, c) != -1){
		count++;
		newnode = table[document];
		for(i = 0; user[i]; i++){
			if(user[i] >= '0' && user[i] <= '9'){
				user[i] -= '0';
			}
			else {
				user[i] += 10 - 'a';
			}
		}
		index = get_dictionary(&user_mapping, user, 0);
		if(index == NULL){
			continue;
		}
		while(newnode){
			//insert_dictionary(&user_mapping, user, newnode->category, newnode->confidence, 0);
			insert_node(index, newnode->category, newnode->confidence);
			newnode = newnode->next;
		}
		if(count%100000 == 0){
			fprintf(stderr, "ROW %d\n", count);
		}
	}
	fclose(fp);

//	while(scanf("%s", user) != -1){
//		for(i = 0; user[i]; i++){
//			if(user[i] >= '0' && user[i] <= '9'){
//				user[i] -= '0';
//			}
//			else {
//				user[i] += 10 - 'a';
//			}
//		}
//		if((newnode = get_dictionary(&user_mapping, user)) == NULL){
//			fprintf(stderr, "NO ENTRY\n");	
//		}
//		else{
//			print_node(newnode);
//		}
//	}
	printf("display_id,ad_id,clicked,ad_document_id,campaign_id,advertiser_id,ad_source_id,ad_publisher_id,read_document_id,uuid,platform,geo_location,read_source_id,read_publisher_id,category_correlation,topic_correlationi,entity_correlation,user_ka_category_correlation,user_ka_topic_correlation,user_ka_entity_correlation\n");
	scanf("%s\n", c);
	count = 0;
	while(scanf("%d,%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf,%lf,%lf\n",
				&display_id,
				ad_id,
				clicked,
				&ad_document_id,
				campaign_id,
				advertiser_id,
				ad_source_id,
				ad_publisher_id,
				&read_document_id,
				uuid,
				platform,
				geo_location,
				read_source_id,
				read_publisher_id,
				&category_correlation,
				&topic_correlation,
				&entity_correlation,
				&user_ka_category_correlation,
				&user_ka_topic_correlation,
				&user_ka_entity_correlation) != -1){
		printf("%d,%s,%s,%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%.12lf,%.12lf,%.12lf,%.12lf,%.12lf,",
				display_id,
				ad_id,
				clicked,
				ad_document_id,
				campaign_id,
				advertiser_id,
				ad_source_id,
				ad_publisher_id,
				read_document_id,
				uuid,
				platform,
				geo_location,
				read_source_id,
				read_publisher_id,
				category_correlation,
				topic_correlation,
				entity_correlation,
				user_ka_category_correlation,
				user_ka_topic_correlation);
		if(!(uuid[0] >= '0' && uuid[0] <= '8')){
			printf("%.12lf\n", user_ka_entity_correlation);
			continue;
		}
		for(i = 0; uuid[i]; i++){
			if(uuid[i] >= '0' && uuid[i] <= '9'){
				uuid[i] -= '0';
			}
			else {
				uuid[i] += 10 - 'a';
			}
		}
		printf("%.12lf\n", calculate_correlation(table[ad_document_id], get_dictionary(&user_mapping, uuid, 0)));
		
	}

	//Following part takes 2 document_id as input and calculate their correlation
	//	while(scanf("%d %d", &document1, &document2) != -1){
	//		//printf("%.12lf\n", calculate_correlation(table[document1], table[document2]));
	//		//calculate_correlation(table[document1], table[document2]);
	//	}
}
