import sys

try:
    input_file = sys.argv[1] 
except Exception as e:
    print 'Enter input file name as command line argument'
    sys.exit(0)

print 'Output filename: entity-score.csv'
fi = open(input_file, 'r')

entity_score = {}

line = fi.readline()
while (line):
    lst = line.split(',')
    if (lst[0] not in entity_score):
        entity_score[lst[0]] = float(lst[1].replace('\n', ''))
    else:
        entity_score[lst[0]] += float(lst[1].replace('\n', ''))
    line = fi.readline()
fi.close()

fo = open('entity-score.csv', 'w')
fo.write('entity_id,score\n')
for key in entity_score:
    fo.write(key + ',' + str(entity_score[key]) + '\n')
fo.close()
print 'Output filename: entity-score.csv'
