import unicodecsv
import sys
import datetime as datetime

def read_csv(filename):
    with open(filename, 'rb') as f:
        try:
            reader = unicodecsv.DictReader(f)
            return list(reader)
        except Exception as e:
            print e

def gen_hash_item(field, feat): 
    return '{0}:{1}:1'.format(field,hash(feat) % (10000000))

def gen_hash_row(row, cols):
    result = []
    i = 0
    size = len(cols)
    while (i < size):
        if (cols[i] == 'clicked'):
            i += 1
            continue
        if row[cols[i]] != 'NA':
                result.append(gen_hash_item(i, cols[i] + "-" + row[cols[i]]))
        i += 1
    return str(row['clicked']) + ' ' + ' '.join(result)+'\n'

input_filepath = sys.argv[1] 
output_filepath = './input-format-ffm.ffm' 
file_list = read_csv(input_filepath)
num_rows = len(file_list)

cols = list(file_list[0].keys())

f = open(output_filepath, 'w')

i = 0
for row in file_list:
    if (i % 200000 == 0):
        print 'Percent completed:', ( i / num_rows) * 100, 'Time:', datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    result = gen_hash_row(row, cols)
    f.write(result)
    i += 1

f.close()
