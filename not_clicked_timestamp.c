#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define CATEGORY_CSV "./Dataset_zips/documents_meta.csv"
#define TABLE_SIZE 2999334
int parse(char *c, struct tm *publish_time){
	int count = 0;
	while(count < 3){
		if(*c == ','){
			count++;
		}
		c++;
	}
	if(*c){
		sscanf(c, "%d-%d-%d %d:%d:%d",  
				&publish_time->tm_year, 
				&publish_time->tm_mon,
				&publish_time->tm_mday,
				&publish_time->tm_hour,
				&publish_time->tm_min,
				&publish_time->tm_sec);
		publish_time->tm_year -= 1900;
		return 0;
	}
	else{
		return -1;
	}
}
int main(){
	FILE *fp;
	time_t *table;
	int document_id = 0, source_id, publish_id;
	int ad_document_id, read_document_id;
	char c[1024];
	int clicked;
	char display_id[64], ad_id[64], campaign_id[64], advertiser_id[64],
	     ad_source_id[64], ad_publisher_id[64], ad_topic_id[64], ad_entity_id[64],
	     ad_category_id[64], uuid[64], platform[64], geo_location[64], read_source_id[64], 
	     read_publisher_id[64], read_topic_id[64], read_entity_id[64], read_category_id[64];
	double topic_correlation, entity_correlation, category_correlation;
	
	struct tm publish_time;
	publish_time.tm_sec = 0;
	publish_time.tm_min = 0;
	publish_time.tm_hour = 0;
	publish_time.tm_mday = 1;     /* 1st */
	publish_time.tm_mon = 6;      /* July */
	publish_time.tm_year = 2038 - 1900; /* 2038 in years since 1900 */
	publish_time.tm_isdst = 0;          /* Daylight Saving not in affect (UTC) */
#ifdef _BSD_SOURCE
	publish_time.tm_zone = "UTC";
#endif

	table = (time_t *)calloc(TABLE_SIZE, sizeof(time_t));
	if(table == NULL){
		fprintf(stderr, "Memory issues");
		exit(-1);
	}
	//opening file documents_categories.csv
	fp = fopen(CATEGORY_CSV, "r");
	if(fp == NULL){
		perror("CATEGORY_CSV file failed to open");
		exit(-1);
	}
	//fscanf(fp, "%s\n", c);
	fscanf(fp, "%s\n", c);
	while( fscanf(fp, "%d", &document_id)!= -1){
		//scanf("%s\n", c);
		fgets(c, 1024, fp);
		if(parse(c, &publish_time)){
			continue;
		}
		if(document_id < 0 || table[document_id] != 0){
			continue;	
		}
		table[document_id] = mktime(&publish_time);
	}
	fclose(fp);
	fprintf(stderr, "TABLE CREATED");
	printf("difference,absolute_difference\n");
	scanf("%s\n", c);
	while(scanf("%[^,],%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%lf,%lf\n",
				display_id,
				ad_id,
				&clicked,
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
				&entity_correlation) != -1){
		if(clicked == 1 || table[ad_document_id] == 0 || table[read_document_id] == 0){
			continue;
		}
		printf("%d,%d\n", 
				table[ad_document_id] - table[read_document_id],
				abs(table[ad_document_id] - table[read_document_id]));
	}
	
}
