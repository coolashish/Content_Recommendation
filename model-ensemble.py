import sys

"""
    Combines the submission of various models into one submission
    Enter the files as command line arguments in order of decreasing weightage
    Assumes all files contain same number of rows
"""

def get_majority(lst):
    """
        Uses Boyer-Moore algorithm to find
        majority element
    candidate = lst[0]

    count = 0
    for elem in lst:
        if (count == 0):
            candidate = elem 
        if (elem == candidate):
            count += 1
        else:
            count -= 1
    count = 0
    for elem in lst:
        if (elem == candidate):
            count += 1
    """
    a = {}
    candidate = lst[0]
    for elem in lst:
	if elem not in a:
	    a[elem] = 0
	else:
	    a[elem] += 1
    for elem in lst:
    	if (a[elem] >= len(lst) / 3):
            candidate = elem
    return candidate

def ensemble(result, fo):
    """
        Use majority to get a final submission
        result is a list of file lines
    """
    size = len(result)
    lst = []
    for line in result:
        values = line.split(',')
        lst.append(values[1].split()[0])
    majority_elem = get_majority(lst)
    fo.write(values[0] + ',' + majority_elem + ' ')
    ads = result[0].split(',')[1].split()
    for ad in ads:
        if (ad != majority_elem):
            fo.write(ad + ' ')
    fo.write('\n')


        

size = len(sys.argv)
print size
print sys.argv
#size -= 1
print sys.argv

fo = open('model-ensemble-submission-19jan.csv', 'w')
fo.write('display_id,ad_id\n')

f = []
i = 1
while (i < size): 
    print 'Filename', sys.argv[i]
    f.append(open(sys.argv[i], 'r'))
    i += 1
    
result = []

#Read the header
for elem in f:
    elem.readline()

for elem in f:
    result.append(elem.readline())

flag = True
while (len(result) != 0):

    ensemble(result, fo)
    result[:] = [] 
    for elem in f:
        line = elem.readline()
        if (line):
            result.append(line)
        else:
            flag = False
            break
    if (flag == False):
        break

for elem in f:
    elem.close()
fo.close()
