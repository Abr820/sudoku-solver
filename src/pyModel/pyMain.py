#libraries
import numpy as np
import pandas as pd 
from keras.preprocessing.image import ImageDataGenerator, load_img, img_to_array
from keras.utils import to_categorical
import matplotlib.pyplot as plt
from keras.models import Model, load_model
import tensorflow as tf
import random
import os
import model

main_dir = "/home/abrar/My_Project/SudokuSolver/"

model = load_model(main_dir+"data/models/ResNet20.h5")
        
def predict(img):
    x = np.array(img)
    x = np.expand_dims(x, axis=0)
    y = model.predict(x)
    print("y shape is:",y.shape)
    y = np.argmax(y,axis = 1)
    y = np.squeeze(y)
    return y
