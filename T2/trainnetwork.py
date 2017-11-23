#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = number of epochs to train
#argv[2] = filename of model to train
#argv[3] = filename of images array
#argv[4] = filename of labels array
#argv[5] = filename to save the trained network

from keras import backend, models, metrics
import numpy as np
import sys
import time

#Carregar imagens, labels e modelos
cnn = models.load_model(sys.argv[2])
images = np.load(sys.argv[3])
labels = np.load(sys.argv[4])
cnn.summary()

#Treinar pelo numero de epochs necessário
batch_size = 64
init_time = time.time()
train_score = cnn.fit(images, labels, epochs=int(sys.argv[1]), batch_size=batch_size, verbose=False)
print("Train Loss: %.4lf\nTrain Accuracy: %.4lf" % (train_score.history['loss'][-1], train_score.history['acc'][-1]))
print("Time: %ds\n" % (time.time()-init_time))

cnn.save(sys.argv[5])
backend.clear_session()