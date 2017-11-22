#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = number of folds
#argv[2] = filename of images array
#argv[3] = filename of labels array
#argv[4] = folder to save the folds

import numpy as np
import sys
import os.path

#Carregar imagens, labels e modelos
images = np.load(sys.argv[2])
labels = np.load(sys.argv[3])
n_images = images.shape[0]
shuff = np.random.permutation(n_images)
images, labels = images[shuff], labels[shuff]

folds = int(sys.argv[1])
fold_size = int(n_images/folds)
print(images[n_images:n_images])
for i in range(folds):
	train_images = np.concatenate((images[0:i*fold_size], images[(i+1)*fold_size:n_images]))
	train_labels = np.concatenate((labels[0:i*fold_size], labels[(i+1)*fold_size:n_images]))
	test_images = images[i*fold_size:(i+1)*fold_size]
	test_labels = labels[i*fold_size:(i+1)*fold_size]

	np.save(os.path.join(sys.argv[4], "fold_" + str(i) + "_train_" + sys.argv[2]), train_images)
	np.save(os.path.join(sys.argv[4], "fold_" + str(i) + "_train_" + sys.argv[3]), train_labels)
	np.save(os.path.join(sys.argv[4], "fold_" + str(i) + "_test_" + sys.argv[2]), test_images)
	np.save(os.path.join(sys.argv[4], "fold_" + str(i) + "_test_" + sys.argv[3]), test_labels)

	print("Fold " + str(i))
	print("Train: [0, " + str(i*fold_size) + ") U [" + str((i+1)*fold_size) + ", " + str(n_images) + "); Size: " + str(train_images.shape[0]))
	print("Test: [" + str(i*fold_size) + ", " + str((i+1)*fold_size) + "); Size: " + str(test_images.shape[0]))