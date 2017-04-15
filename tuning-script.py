import sys
import os

def analyze_output(filename):
	"""
	Return values:
	-1,value: overfitting occured & highest index at which it did not occur
	1,value: overfitting did not occur and value minimum va_logloss
	"""
	f = open(filename, 'r')
	line = f.readline()
	prev_val_logloss = 1
	i = 1
	while (line):
		curr_val_loss = float(line.split(' ')[2])
		if (curr_val_loss > pre_val_loss):
			f.close()
			return -1,i - 1
		prev_val_loss = curr_val_loss
		line = f.readline()
		i += 1
	f.close()
	return 1,curr_val_loss
		

train_file = sys.argv[1]
validation_file = sys.argv[2]

regularization = 0.000002
regularization_step_size = 0.00001

learning_rate = 0.05
learning_rate_step_size = 0.05

iterations = 20
iterations_step_size = 20

fo = open('tuning-analysis.txt', 'w')
fo.write('regularization\tlearning_rate\titerations\tvalidation_loss\n')

prev_loss = 2
curr_loss = 1

while (True):
	
	if (curr_loss > prev_loss):
		break
	prev_loss = curr_loss

	while (True):
		if (curr_loss > prev_loss):
			break
		prev_loss = curr_loss
		quit_flag = False

		while (True):
			ret = os.system('./ffm-train' + ' -l ' + str(regularization) + ' -r '+ str(learning_rate) + ' -t ' + str(iterations) + ' -p ' + validation_file + ' ' + train_file + ' > log.txt')	
			if (ret != 0):
				print 'Error executing command'
				sys.exit(-1)
			flag,value = analyze_output('log.txt')
			if (quit_flag):
				fo.write(str(regularization) + '\t' + str(learning_rate) + '\t' + str(iterations) + '\t' + str(value) + '\n')
				curr_loss = value
				break
			if (flag == -1):
				quit_flag = True
				iterations = value
			else:
				iterations += iteration_step_size


		learning_rate += learning_rate_step_size

	regularization += regularization_step_size

fo.close()
