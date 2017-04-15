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


#file_list = read_csv(input_filepath)
#num_rows = len(file_list)

print 'Before input file open'
fin = open(input_filepath, 'r')
print 'After output file open'
reader = unicodecsv.DictReader(fin)
print 'After DictReader'

cols = list(next(reader))
print 'First line', cols

f = open(output_filepath, 'w')

i = 0
#for row in file_list:
row = next(reader)
while (row):
    if (i % 200000 == 0):
#print 'Percent completed:', ( i / num_rows) * 100, 'Time:', datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print 'No. of rows completed:', i, 'Time:', datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    #result = gen_hash_row(row, cols)
    #f.write(result)
    i += 1
    print 'row', row
    row = next(reader)
    #row = reader.next()


f.close()
fin.close()
