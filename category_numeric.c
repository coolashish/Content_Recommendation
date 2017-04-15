#include <stdio.h>

int main(int rgc, char **argv){
        int document;
        int read_document_id, ad_document_id;
        char c[25][128];
        double confidence;
        char category[64];
        FILE *fp;
        int i;
        FILE *fp1, *fp2;
        int display_id;
        char user[16];
        int count = 0;
        char ad_id[64], clicked[64], campaign_id[64], advertiser_id[64],
             ad_source_id[64], ad_publisher_id[64], ad_topic_id[64], ad_entity_id[64],
             ad_category_id[64], uuid[64], platform[64], geo_location[64], read_source_id[64],
             read_publisher_id[64], read_topic_id[64], read_entity_id[64], read_category_id[64];
        double topic_correlation, entity_correlation, category_correlation;
        double user_ka_category_correlation, user_ka_topic_correlation, user_ka_entity_correlation;
        char popularity[5][64];
        char *char_ptr;
        double temp_double;


        fp1= fopen(argv[1], "r");
        if(fp1 == NULL){
                fprintf(stderr, "Error in %s", argv[1]);
        }
        fp2 = fopen(argv[2], "r");
        if(fp2 == NULL){
                fprintf(stderr, "Error in %s", argv[2]);
        }

        while(fscanf(fp2, "%d,%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf,%lf,%lf,%[^,],%[^,],%[^,],%[^,],%s\n",
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
                                &user_ka_entity_correlation,
                                popularity[0],
                                popularity[1],
                                popularity[2],
                                popularity[3],
                                popularity[4]) != -1){

                fscanf(fp1, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",
                        c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10],
                        c[11], c[12], c[13], c[14], c[15], c[16], c[17], c[18], c[19], c[20],
                        c[21], c[22], c[23], c[24]);
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s ",
                        c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10],
                        c[11], c[12], c[13]);
                printf("13:%d 14:%d 15:%d 16:%d 17:%d 18:%d ",
                        (int)(category_correlation * 10000),
                        (int)(topic_correlation * 10000),
                        (int)(entity_correlation * 10000),
                        (int)(user_ka_category_correlation * 10000),
                        (int)(user_ka_topic_correlation * 10000),
                        (int)(user_ka_entity_correlation * 10000)
                        );
                if(strcmp(popularity[0], "NA") == 0){
                        printf("%s ", c[20]);
                }
                else {
                        sscanf(popularity[0], "%lf", &temp_double);
                        printf("19:%d ", (int)(temp_double * 10000));
                }

                if(strcmp(popularity[1], "NA") == 0){
                        printf("%s ", c[21]);
                }
                else {
                        sscanf(popularity[1], "%lf", &temp_double);
                        printf("20:%d ", (int)(temp_double * 10000));
                }

                if(strcmp(popularity[2], "NA") == 0){
                        printf("%s ", c[22]);
                }
                else {
                        sscanf(popularity[2], "%lf", &temp_double);
                        printf("21:%d ", (int)(temp_double * 10000));
                }

                if(strcmp(popularity[3], "NA") == 0){
                        printf("%s ", c[23]);
                }
                else {
                        sscanf(popularity[2], "%lf", &temp_double);
                        printf("22:%d ", (int)(temp_double * 10000));
                }

                if(strcmp(popularity[4], "NA") == 0){
                        printf("%s\n", c[24]);
                }
                else {
                        sscanf(popularity[4], "%lf", &temp_double);
                        printf("23:%d\n", (int)(temp_double * 10000));
                }
        }
}
       
