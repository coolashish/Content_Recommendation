#echo 'Starting encoding for ffm'
/home/hadoop/B.Tech-Project/libfm-1.42.src/scripts/csv_to_libffm.pl -target 2 -in /media/hadoop/A45C6942B33F249F/B.Tech-Project/train-numeric-to-categorical-14jan.csv,/media/hadoop/A45C6942B33F249F/B.Tech-Project/test_numeric-to-categoric-14jan.csv -separator ','
#echo 'Starting numerical encoding for train file'
#./categorical_to_numeric_ffm /home/hadoop/B.Tech-Project/train-lesser-columns-12jan.csv.libfm /home/hadoop/B.Tech-Project/train-lesser-columns-12jan.csv  > /media/hadoop/A45C6942B33F249F/B.Tech-Project/train-lesser-columns-14jan.ffm
#echo 'Starting numerical encoding for test file'
#./categorical_to_numeric_ffm /home/hadoop/B.Tech-Project/test_lesser-columns-12jan.csv.libfm /home/hadoop/B.Tech-Project/test_lesser-columns-12jan.csv  > /media/hadoop/A45C6942B33F249F/B.Tech-Project/test_lesser-columns-14jan.ffm
