#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = path to load input images
#argv[2] = path to save output arrays

from keras.preprocessing.image import ImageDataGenerator
import numpy as np
import sys
import os.path
from PIL import ImageFile
from math import ceil

#Argumentos
in_path = os.path.join("..", "Data", "Datasets", "separated", "train")
out_path = os.path.join("..", "Data", "Datasets", "keras", "train")
if (len(sys.argv) >= 2):
	in_path = sys.argv[1]
if (len(sys.argv) >= 3):
	out_path = sys.argv[2]

ImageFile.LOAD_TRUNCATED_IMAGES = True
#Criar um objeto que cria um iterador que percorre e classifica as imagens
#baseado na estrutura de pastas
batch_size = 256
img_gen = ImageDataGenerator()
img_flow = img_gen.flow_from_directory(in_path, target_size=(224, 224), class_mode='categorical', batch_size=batch_size, shuffle=False)

#Gerar vetores finais com todas as imagens e labels
x, y = img_flow.next()
for i in range(ceil((img_flow.samples-batch_size)/batch_size)):
	aux_x, aux_y = img_flow.next()
	x = np.concatenate((x, aux_x))
	y = np.concatenate((y, aux_y))

print("Images array: " + str(x.shape))
print("Labels array: " + str(y.shape))
np.save(os.path.join(out_path, "Images"), x)
np.save(os.path.join(out_path, "Labels"), y)