import pandas as pd
import numpy as np
import sys
import ctypes

map_col = lambda dat,col: col+"-"+dat.map(str)

#gen_hash_item = lambda field, feat: '{0}:{1}:1'.format(field,hashstr(feat))
#Replace {0} by field_id
#Replace {1} by hash value of feature (ad_id-42337) 
#gen_hash_item = lambda field, feat: '{0}:{1}:1'.format(field,ctypes.c_size_t(hash(feat) % 1000000000000000).value) )
gen_hash_item = lambda field, feat: '{0}:{1}:1'.format(field, hash(feat) % (5 * pow(10, 7)))

def gen_hash_row(feats,label):
    result = []
    for idx, item in enumerate(feats):
        #idx is index in array
        #idx serves as feature_id
        #item is e.g. ad_id-42337
        #val is 42337
        val = item.split('-')[-1]
        if val != 'nan':
                result.append(gen_hash_item(idx,item))
    #lbl = 1
    #if label == 0:
    #    lbl = -1
    return str(label) + ' ' + ' '.join(result)+'\n'

#input file name
filename = sys.argv[1]
merge_dat = pd.read_csv(filename)
output_name = 'ffm-input.ffm'

#Extract clicked column from data frame
targets = merge_dat.ix[:,'clicked']
targets = list(targets)

#Remove clicked and display id columns from df
merge_dat_val = merge_dat.drop(['clicked'],axis=1)

#cols is list of column names
cols = merge_dat_val.columns

features = []

for col in merge_dat_val.columns:
    #col is name of a column
    #merge_dat_val[col] contains all values of that column
    #map_col converts it to form 'columnName-value'
    features.append(map_col(merge_dat_val[col],col))

#Create rows from all columns again
features = np.array(features).T

with open(output_name,'w') as f_tr:
    i = 0;
    for item,label in zip(features,targets):
        row = gen_hash_row(item,label)
        f_tr.write(row)
        i+=1
