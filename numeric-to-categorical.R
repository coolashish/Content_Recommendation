test_lesser_columns_12jan$user_ka_category_correlation[test_lesser_columns_12jan$user_ka_category_correlation > 0] = 1
gc()
test_lesser_columns_12jan$user_ka_topic_correlation[test_lesser_columns_12jan$user_ka_topic_correlation > 0] = 1
gc()
test_lesser_columns_12jan$user_ka_entity_correlation[test_lesser_columns_12jan$user_ka_entity_correlation > 0] = 1
gc()

test_lesser_columns_12jan = test_lesser_columns_12jan %>%  group_by(display_id) %>%  mutate(ad_source_popularity_rank = order(ad_source_popularity, decreasing=TRUE))
test_lesser_columns_12jan = test_lesser_columns_12jan %>%  group_by(display_id) %>%  mutate(ad_topic_popularity_rank = order(ad_topic_popularity, decreasing=TRUE))
test_lesser_columns_12jan = test_lesser_columns_12jan %>%  group_by(display_id) %>%  mutate(ad_categories_popularity_rank = order(ad_categories_popularity, decreasing=TRUE))


test_lesser_columns_12jan$ad_source_popularity = NULL
test_lesser_columns_12jan$ad_categories_popularity = NULL
test_lesser_columns_12jan$ad_topic_popularity = NULL