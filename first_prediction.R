library(plyr)
library(xgboost)
library(readr)
library(stringr)
library(caret)
library(car)


load("~/B.Tech Project/AllDataImported.RData")

documents_topics_unique = ddply(documents_topics, ~document_id, function(x){x[which.max(x$confidence_level),]})
documents_topics_unique$confidence_level = NULL

documents_entities_unique = ddply(documents_entities, ~document_id, function(x){x[which.max(x$confidence_level),]})
documents_entities_unique$confidence_level = NULL

documents_categories_unique = ddply(documents_categories, ~document_id, function(x){x[which.max(x$confidence_level),]})
documents_categories_unique$confidence_level = NULL

documents_topics_entities = merge(documents_topics_unique, documents_entities_unique, all = TRUE)
documents_topics_entities_categories = merge(documents_topics_entities, documents_categories_unique, all = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

documents_complete = merge(documents_meta, documents_topics_entities_categories, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

documents_complete$publish_time = NULL

promoted_complete = merge(promoted_content, documents_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

clicks_train_complete = merge(clicks_train, promoted_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

rename(clicks_train_complete, c("topic_id" = "ad_topic_id", "entity_id" = "ad_entity_id", "category_id" = "ad_category_id"))

events$timestamp = NULL

events_complete = merge(events, documents_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

rename(events_complete, c("topic_id" = "read_topic_id", "entity_id" = "read_entity_id", "category_id" = "read_category_id"))

save.image("~/B.Tech Project/RWorkSpace.RData")

train_final = merge(clicks_train_complete, events_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")



#Preparing the test data frame
clicks_test_complete = merge(clicks_test, promoted_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")

rename(clicks_test_complete, c("topic_id" = "ad_topic_id", "entity_id" = "ad_entity_id", "category_id" = "ad_category_id"))

test_final = merge(clicks_test_complete, events_complete, all.x = TRUE)

save.image("~/B.Tech Project/RWorkSpace.RData")




#Start with the prediction now
# load data after we make the final 2 files
df_train = train_final
df_test = test_final

# Loading labels of train data

labels = df_train['clicked']
df_train = df_train[-grep('clicked', colnames(df_train))]

# combine train and test data
df_all = rbind(df_train,df_test)

# one-hot-encoding categorical features
ohe_feats = c('read_document_category', 'read_document_entity', 'read_document_topic', 'ad_document_category', 'ad_document_entity', 'ad_document_topic', 'campaign_id', 'advertiser_id', 'platform', 'geo_location', 'traffic_source')
dummies <- dummyVars(~ read_document_category+read_document_entity+read_document_topic+ad_document_category+ad_document_entity+ad_document_topic+campaign_id+advertiser_id+platform+geo_location+traffic_source, data = df_all)
df_all_ohe <- as.data.frame(predict(dummies, newdata = df_all))
df_all_combined <- cbind(df_all[,-c(which(colnames(df_all) %in% ohe_feats))],df_all_ohe)

# split train and test
X_train = df_all_combined[df_all_combined$uuid %in% df_train$uuid,]
y <- labels$clicked
X_test = df_all_combined[df_all_combined$uuid %in% df_test$uuid,]

#Tune and Run the model
y = train_final$clicked
library(xgboost)
xgb <- xgboost(data = data.matrix(train_final[,-1]),
               label = y, 
               eta = 0.1,
               max_depth = 15, 
               nround=15, 
               objective = "binary:logistic",
               nthread = 8,
               missing = NaN
)

names <- dimnames(data.matrix(train_final[,-1]))[[2]]
importance_matrix <- xgb.importance(names, model = xgb)
xgb.plot.importance(importance_matrix[1:6,])

# predict values in test set
# y_pred <- predict(xgb, data.matrix(test_final[,-1]))

for(i in 1:nrow(user_analysis_frame)) {
  print(i)
  user_id = user_analysis_frame[i,  'user_id']
  display_ids = events$display_id[events$uuid == user_id] 
  display_ids_in_train = display_ids[display_ids <= 16874593]
  display_ids_in_test = display_ids[display_ids > 16874593]
  user_analysis_frame[i, 'display_ids_in_train'] = paste(display_ids_in_train, collapse = " ")
  user_analysis_frame[i, 'display_ids_in_test'] = paste(display_ids_in_test, collapse = " ")
}

names(platform_analysis)[names(platform_analysis) == 'clicked'] = 'platform_freq'
names(geo_loc_analysis)[names(geo_loc_analysis) == 'clicked'] = 'geo_loc_freq'
names(campaign_analysis)[names(campaign_analysis) == 'clicked'] = 'campaign_freq'
names(advertiser_analysis)[names(advertiser_analysis) == 'clicked'] = 'advertiser_freq'

test_final = merge(test_final_25dec, platform_analysis, all.x = TRUE, sort = FALSE, by = c('platform', 'ad_id'))
test_final = merge(test_final, campaign_analysis, all.x = TRUE, sort = FALSE, by = c('campaign_id', 'ad_id'))
test_final = merge(test_final, advertiser_analysis, all.x = TRUE, sort = FALSE, by = c('advertiser_id', 'ad_id'))
test_final = merge(test_final, geo_loc_analysis, all.x = TRUE, sort = FALSE, by = c('geo_location', 'ad_id'))
