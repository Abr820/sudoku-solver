#libraries
import numpy as np
from keras import layers
from keras.layers import Input, Add, Dense, Activation, ZeroPadding2D, BatchNormalization
from keras.layers import Flatten, Conv2D, AveragePooling2D, MaxPooling2D, Dropout
from keras.models import Model, load_model
from keras.initializers import glorot_uniform
import tensorflow as tf
import random


#variables
IMAGE_WIDTH=28
IMAGE_HEIGHT=28
IMAGE_SIZE=(IMAGE_WIDTH, IMAGE_HEIGHT)
IMAGE_CHANNELS=3
batch_size=32

def identity_block(X, f, filters, stage, block):

	# defining name basis
	conv_name_base = 'res' + str(stage) + block + '_branch'
	bn_name_base = 'bn' + str(stage) + block + '_branch'

	# Retrieve Filters
	F1, F2, F3 = filters

	# Save the input value. You'll need this later to add back to the main path. 
	X_shortcut = X

	# First component of main path
	X = Conv2D(filters = F1, kernel_size = (1, 1), strides = (1,1), padding = 'valid', name = conv_name_base + '2a', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2a')(X)
	X = Activation('relu')(X)

	# Second component of main path (≈3 lines)
	X = Conv2D(filters = F2, kernel_size = (f, f), strides = (1,1), padding = 'same', name = conv_name_base + '2b', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2b')(X)
	X = Activation('relu')(X)

	# Third component of main path (≈2 lines)
	X = Conv2D(filters = F3, kernel_size = (1, 1), strides = (1,1), padding = 'valid', name = conv_name_base + '2c', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2c')(X)

	# Final step: Add shortcut value to main path, and pass it through a RELU activation (≈2 lines)
	X = Add()([X,X_shortcut])
	X = Activation('relu')(X)

	return X
    
    
def convolutional_block(X, f, filters, stage, block, s = 2):

	# defining name basis
	conv_name_base = 'res' + str(stage) + block + '_branch'
	bn_name_base = 'bn' + str(stage) + block + '_branch'

	# Retrieve Filters
	F1, F2, F3 = filters

	# Save the input value
	X_shortcut = X

	# First component of main path 
	X = Conv2D(F1, (1, 1), strides = (s,s), name = conv_name_base + '2a', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2a')(X)
	X = Activation('relu')(X)

	# Second component of main path (≈3 lines)
	X = Conv2D(filters = F2, kernel_size = (f, f), strides = (1,1), padding = 'same', name = conv_name_base + '2b', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2b')(X)
	X = Activation('relu')(X)

	# Third component of main path (≈2 lines)
	X = Conv2D(filters = F3, kernel_size = (1, 1), strides = (1,1), padding = 'valid', name = conv_name_base + '2c', kernel_initializer = glorot_uniform(seed=0))(X)
	X = BatchNormalization(axis = 3, name = bn_name_base + '2c')(X)

	##### SHORTCUT PATH #### (≈2 lines)
	X_shortcut = Conv2D(filters = F3, kernel_size = (1, 1), strides = (s,s), padding = 'valid', name = conv_name_base + '1', kernel_initializer = glorot_uniform(seed=0))(X_shortcut)
	X_shortcut = BatchNormalization(axis = 3, name = bn_name_base + '1')(X_shortcut)

	# Final step: Add shortcut value to main path, and pass it through a RELU activation (≈2 lines)
	X = Add()([X,X_shortcut])
	X = Activation('relu')(X)

	return X
    
    
def createModel():

	# Define the input as a tensor with shape input_shape
	input_shape=(28,28,3)
	X_input = Input(input_shape)
	X = convolutional_block(X_input, f = 3, filters = [8, 16, 32], stage = 1, block='a', s = 1)
	X = identity_block(X, 3, [8, 16, 32], stage=1, block='b')
	X = identity_block(X, 3, [8, 16, 32], stage=1, block='c')
	# AVGPOOL (≈1 line). Use "X = AveragePooling2D(...)(X)"
	X = AveragePooling2D((2, 2), strides=(2, 2), name='avg_pool')(X)

	X = convolutional_block(X, f = 3, filters = [8, 16, 32], stage = 2, block='a', s = 1)
	X = identity_block(X, 3, [8, 16, 32], stage=2, block='b')
	X = identity_block(X, 3, [8, 16, 32], stage=2, block='c')

	# output layer
	X = Flatten()(X)
	X = Dropout(0.7)(X)
	X = Dense(10, activation='softmax', name='fc' + str(10), kernel_initializer = glorot_uniform(seed=0))(X)

	# Create model
	model = Model(inputs = X_input, outputs = X, name='ResNet20')
	model.summary() 
	return model  




