#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = filename of numpy-array file of the images
#argv[2] = filename of numpy-array file of the labels
#argv[3] = filename of model to evaluate

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
score = cnn.evaluate(images, labels, batch_size=batch_size, verbose=False)
print("Loss: %.4lf\nAccuracy: %.4lf" % (score[0], score[1]))