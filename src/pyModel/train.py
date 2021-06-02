#libraries
import numpy as np
import pandas as pd 
from keras.preprocessing.image import ImageDataGenerator, load_img, img_to_array
from keras.utils import to_categorical
import matplotlib.pyplot as plt
from keras import layers
from keras.layers import Input, Add, Dense, Activation, ZeroPadding2D, BatchNormalization
from keras.layers import Flatten, Conv2D, AveragePooling2D, MaxPooling2D, Dropout
from keras.models import Model, load_model
from keras.initializers import glorot_uniform
import tensorflow as tf
import random
import os
import model

#variables
IMAGE_WIDTH=28
IMAGE_HEIGHT=28
IMAGE_SIZE=(IMAGE_WIDTH, IMAGE_HEIGHT)
IMAGE_CHANNELS=3
batch_size=32


from keras.preprocessing.image import ImageDataGenerator

train_datagen = ImageDataGenerator(rescale=1./255)

val_datagen = ImageDataGenerator(rescale = 1./255)

main_dir="/home/abrar/My_Project/SudokuSolver/"

train_datagen = ImageDataGenerator(rescale=1./255)
train_generator = train_datagen.flow_from_directory(
        (main_dir + 'data/train/datasets'),
        target_size=(28, 28),
        batch_size=batch_size,
        class_mode='categorical',
        shuffle = True)

validation_datagen = ImageDataGenerator(rescale=1./255)
validation_generator = validation_datagen.flow_from_directory(
         (main_dir + 'data/train/datasets'),
        target_size=(28, 28),
        batch_size=batch_size,
        class_mode='categorical')
        
        
#define and call model
model = model.createModel()
#model = load_model(main_dir + 'data/models/ResNet20.h5')
#model.summary()
# Compiling the CNN
model.compile(optimizer = 'adam', loss = 'binary_crossentropy', metrics = ['acc'])

from keras.callbacks import ModelCheckpoint,ReduceLROnPlateau,EarlyStopping
checkpoint = ModelCheckpoint((main_dir + 'data/models/ResNet20.h5'), monitor='val_acc', verbose=1,
    save_best_only=True, mode='auto', period=1)
learning_rate_reduction = ReduceLROnPlateau(monitor='val_acc', 
                                            patience=1, 
                                            verbose=1, 
                                            factor=0.4, 
                                            min_lr=0.000001)
earlystop = EarlyStopping(patience=10)
callbacks = [checkpoint,earlystop,learning_rate_reduction]

epochs=50
history = model.fit_generator(
    train_generator, 
    epochs=epochs,
    validation_data=validation_generator,
    validation_steps=2000//batch_size,
    steps_per_epoch=10000//batch_size,
    callbacks=callbacks
)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 12))
ax1.plot(history.history['loss'], color='b', label="Training loss")
ax1.plot(history.history['val_loss'], color='r', label="validation loss")
ax1.set_xticks(np.arange(1, epochs, 1))
ax1.set_yticks(np.arange(0, 1, 0.1))

ax2.plot(history.history['acc'], color='b', label="Training accuracy")
ax2.plot(history.history['val_acc'], color='r',label="Validation accuracy")
ax2.set_xticks(np.arange(1, epochs, 1))

legend = plt.legend(loc='best', shadow=True)
plt.tight_layout()
plt.show()
