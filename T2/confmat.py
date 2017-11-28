#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = filename of model to evaluate
#argv[2] = filename of numpy-array file of the images
#argv[3] = filename of numpy-array file of the labels

from keras import models
import numpy as np
import sys

#Argumentos
cnn = models.load_model(sys.argv[1])
images = np.load(sys.argv[2])
labels = np.load(sys.argv[3])
#cnn.summary()

#Avaliar rede
batch_size = 64
pred = cnn.predict(images, batch_size=batch_size, verbose=False)

conf = np.zeros((15, 15))
conf_count = np.zeros(15)
for i in range(labels.shape[0]):
	clas = np.argmax(labels[i], axis=0)
	pred_clas = np.argmax(pred[i], axis=0)
	conf[clas][pred_clas] = conf[clas][pred_clas] + 1
	conf_count[clas] = conf_count[clas] + 1

for i in range(15):
	conf[i] = conf[i]/conf_count[i]
	print(np.array_str(conf[i]).replace('\n', ''))