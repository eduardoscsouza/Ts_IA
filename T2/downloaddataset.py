#Author: Eduardo Santos Carlos de Souza

#Usage:
#argv[1] = path to csv file with urls
#argv[2] = directory to store the images
#argv[3] = number of images to fetch. if -1, fetch all, if -2 fetch all of given styles, -3 fetch specific quantity of given styles

from urllib.request import urlretrieve
from urllib.error import URLError
import numpy as np
import os
import sys
import csv

in_path = "wikipaintings_oct2013.csv"
out_path = "Dataset"
n_imgs = -1
verbose = True
name_idx = 0
style_idx = 5
url_idx = 10
if (len(sys.argv) >= 2):
	in_path = sys.argv[1]
if (len(sys.argv) >= 3):
	out_path = sys.argv[2]
if (len(sys.argv) >= 4):
	n_imgs = int(sys.argv[3])
if (len(sys.argv) >= 5):
	verbose = (str.lower(sys.argv[4]) in {"true", "1"})
if (len(sys.argv) >= 6):
	name_idx = int(sys.argv[5])
	style_idx = int(sys.argv[6])
	url_idx = int(sys.argv[7])

url_file = open(in_path)
reader = list(csv.reader(url_file))
idxs = np.arange(len(reader)-1) + 1

if (n_imgs >= -1):
	if (n_imgs > -1):
		idxs = np.random.permutation(idxs)[0:n_imgs]

	completion = 0
	for i in idxs:
		line = reader[i]
		if verbose:
			print("On ID " + str(i))

		img_dir = os.path.join(".", out_path, line[style_idx])
		if (not os.path.exists(img_dir)):
			os.makedirs(img_dir)
		
		try:
			urlretrieve(line[url_idx], filename=os.path.join(img_dir, line[name_idx]))
			if verbose:
				print("Found " + line[name_idx])
		except URLError:
			print("Error on URL " + line[url_idx])

		completion = completion + 1
		if verbose:
			print("Completion: " + str(completion/len(idxs)) + "%")
else:
	styles = set()
	n_styles = int(input())
	for i in range(n_styles):
		styles.add(input())

	for style in styles:
		img_dir = os.path.join(".", out_path, style)
		if (not os.path.exists(img_dir)):
			os.makedirs(img_dir)

	if (n_imgs==-3):
		per_style = int(input())
		style_count = {}
		for style in styles:
			style_count[style] = per_style
		idxs = np.random.permutation(idxs)
	
	for i in idxs:
		line = reader[i]
		if verbose:
			print("On ID " + str(i))

		if ((line[style_idx] in styles) and ((n_imgs==-2) or (style_count[line[style_idx]]>0))):
			try:
				urlretrieve(line[url_idx], filename=os.path.join(".", out_path, line[style_idx], line[name_idx]))
				if verbose:
					print("Found " + line[name_idx])
				if (n_imgs==-3):
					style_count[line[style_idx]] = style_count[line[style_idx]] - 1
			except URLError:
				print("Error on URL " + line[url_idx])