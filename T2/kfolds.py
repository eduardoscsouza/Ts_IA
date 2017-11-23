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

images_name = str(os.path.split(sys.argv[2])[-1])
labels_name = str(os.path.split(sys.argv[3])[-1])
out_path = sys.argv[4]

folds = int(sys.argv[1])
fold_size = int(n_images/folds)
for i in range(folds):
	train_images = np.concatenate((images[0:i*fold_size], images[(i+1)*fold_size:n_images]))
	train_labels = np.concatenate((labels[0:i*fold_size], labels[(i+1)*fold_size:n_images]))
	test_images = images[i*fold_size:(i+1)*fold_size]
	test_labels = labels[i*fold_size:(i+1)*fold_size]

	train_path = "fold_" + str(i) + "_train_"
	test_path = "fold_" + str(i) + "_test_"
	np.save(os.path.join(out_path, train_path + images_name), train_images)
	np.save(os.path.join(out_path, train_path + labels_name), train_labels)
	np.save(os.path.join(out_path, test_path + images_name), test_images)
	np.save(os.path.join(out_path, test_path + labels_name), test_labels)

	print("Fold " + str(i))
	print("Train: [0, " + str(i*fold_size) + ") U [" + str((i+1)*fold_size) + ", " + str(n_images) + "); Images Shape: " + str(train_images.shape))
	print("Test: [" + str(i*fold_size) + ", " + str((i+1)*fold_size) + "); Images Size: " + str(test_images.shape))