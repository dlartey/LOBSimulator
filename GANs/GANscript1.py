import tensorflow as tf
from tensorflow.keras import layers, models
import pandas as pd
import numpy as np
from sklearn.preprocessing import MinMaxScaler

# Function for loading and preprocessing data
def load_preprocess_data(file_path):
    data = pd.read_csv(file_path)  # Load data from CSV file into DataFrame
    scaler = MinMaxScaler(feature_range=(-1, 1))  # Initialise MinMaxScaler to scale data between -1 and 1
    scaled_data = scaler.fit_transform(data.values)  # Scale the data
    return scaled_data, scaler  # Return scaled data and the scaler for inverse transformation later

# Load and preprocess the dataset
file_path = 'GANs/Datasets/AMZN_2012-06-21_34200000_57600000_orderbook_1.csv'  # File path for the dataset (AMZN SNAPSHOT ORDERBOOK EXAMPLE LOCATED IN 'GANs/Datasets')
real_data, scaler = load_preprocess_data(file_path) 

# Ensure the number of features in the data matches the Generator and Discriminator architecture
# .shape[1] accesses number of columns (features) in the dataset
num_features = real_data.shape[1]

# Generator Network
# Responsible for creating the fake data
def build_generator():
    model = models.Sequential([
        # First layer with 128 neurons, ReLU activation
        # 'relu' activation means it only passes values that are +ve and ignores -ve ones 
        # the layer takes in 100 numbers as input (random numbers to start generatig data)
        layers.Dense(128, activation='relu', input_dim=100),  

        # Batch Normalisation layer 
        # Its purpose is to stabilise the learning process and to reduce the number of training epochs required
        # Normalises the outputs of the previous layer
        layers.BatchNormalization(),  

        # Second dense layer with 256 neurons, also using ReLU activation.
        # This layer continues the pattern recognition process started by the first layer,
        # learning more complex patterns as it has more neurons.
        # The increase in neurons allows the network to learn more complex and varied aspects of the data.
        layers.Dense(256, activation='relu'), 

        layers.BatchNormalization(),  # Normalises the outputs of the previous layer

        # Final output layer with 'tanh' activation function
        # This function outputs values between -1 and 1
        # Useful since our data has been scaled in this range
        # The number of neurons here is equal to the number of features in the dataset (num_features)
        # This layer's job is to reshape the learned patterns into the shape of our actual data  
        layers.Dense(num_features, activation='tanh')  
    ])
    return model

# Discriminator Network
# Responsible for detecting what data is fake or real
def build_discriminator():
    model = models.Sequential([
        # First layer with 256 neurons
        # This layer uses ReLU activation, which means these neurons only react to positive signals    
        layers.Dense(256, activation='relu', input_dim=num_features),

        # Layer to prevent overfitting.
        # This layer randomly ignores some neurons during training  
        layers.Dropout(0.3), 
        
        layers.Dense(128, activation='relu'), 

        layers.Dropout(0.3), 

        # Final output layer with just 1 neuron, using sigmoid activation
        # This function is like a scale, ranging from 0 to 1
        # If the output is close to 1, the network thinks the data is real 
        # Close to 0 -> it's fake.     
        layers.Dense(1, activation='sigmoid')  
    ])
    return model

# Instantiating the Generator and Discriminator networks
generator = build_generator()
discriminator = build_discriminator()

# Loss function for both networks
cross_entropy = tf.keras.losses.BinaryCrossentropy()

# Loss function for Discriminator
# Calculates how well the discriminator is able to distinguish real data from the fake data
def discriminator_loss(real_output, fake_output):

    # 'real_loss' measures the error when the discriminator looks at real data
    # We want the discriminator to output 1 for real data, so we compare its output to 1s.    
    real_loss = cross_entropy(tf.ones_like(real_output), real_output)  

    # 'fake_loss' measures the error when the discriminator looks at fake data.
    # We want the discriminator to output 0 for fake data, so we compare its output to 0s.
    fake_loss = cross_entropy(tf.zeros_like(fake_output), fake_output) 

    # Total loss is the sum of real_loss and fake_loss.
    # A lower total loss means the discriminator is better at telling real from fake.     
    return real_loss + fake_loss 

# Loss function for Generator
# Calculates how well the generator is tricking the discriminator with it's fake outputs
def generator_loss(fake_output):

    # For the fake data it generates, we compare the discriminator's output to 1s this time
    # A lower loss means the generator is better at tricking the discriminator    
    return cross_entropy(tf.ones_like(fake_output), fake_output) 

# Optimisers for both networks
# 'Adam' is a type of optimiser that helps each network improve during training
# The number 1e-4 is the learning rate, which controls how big of steps to take when improving.
generator_optimizer = tf.keras.optimizers.Adam(1e-4)  
discriminator_optimizer = tf.keras.optimizers.Adam(1e-4)  

# Training step for the GAN
def train_step(real_data, batch_size):
    noise = tf.random.normal([batch_size, 100])  # Generate random noise as the starting point for the generator.

    with tf.GradientTape() as gen_tape, tf.GradientTape() as disc_tape:
        generated_data = generator(noise, training=True)  # Generator creates fake data from noise.

        real_output = discriminator(real_data, training=True)  # Discriminator's response to real data
        fake_output = discriminator(generated_data, training=True)  # Discriminator's response to fake data
        
        gen_loss = generator_loss(fake_output)  # Calculate generator loss (how well the generator is doing)
        disc_loss = discriminator_loss(real_output, fake_output)  # Calculate discriminator loss (how well the discriminator is doing)

    # Computing gradients of both networks
    gradients_of_generator = gen_tape.gradient(gen_loss, generator.trainable_variables)  # Gradients for generator
    gradients_of_discriminator = disc_tape.gradient(disc_loss, discriminator.trainable_variables)  # Gradients for discriminator

    # Apply the gradients to the optimisers to improve the generator and discriminator
    generator_optimizer.apply_gradients(zip(gradients_of_generator, generator.trainable_variables))  # Apply generator gradients
    discriminator_optimizer.apply_gradients(zip(gradients_of_discriminator, discriminator.trainable_variables))  # Apply discriminator gradients
    
    return gen_loss, disc_loss  # Return losses

# Function to train the GAN
def train(dataset, epochs, batch_size):
    for epoch in range(epochs):  # Loop through epochs
        gen_loss_epoch = 0  # Tracks generator loss per epoch.
        disc_loss_epoch = 0  # Tracks discriminator loss per epoch.
        steps = 0
        
        for data_batch in dataset:  # Iterate over each batch of real data
            gen_loss, disc_loss = train_step(data_batch, batch_size)  # Perform a training step and get the losses
            gen_loss_epoch += gen_loss.numpy()  # Add up generator loss
            disc_loss_epoch += disc_loss.numpy()  # Add up discriminator loss
            steps += 1
            
        gen_loss_epoch /= steps  # Average generator loss for this epoch
        disc_loss_epoch /= steps  # Average discriminator loss for this epoch

        # Print out the losses at the end of the epoch to see how the training is progressing as it happens
        print(f"Epoch {epoch + 1}, Generator Loss: {gen_loss_epoch}, Discriminator Loss: {disc_loss_epoch}")

# Prepare dataset for training
# This shuffling adds more randomness to the training process
batch_size = 32  # Number of samples in each batch of data.
buffer_size = 1000  # Buffer size for shuffling the data.
train_dataset = tf.data.Dataset.from_tensor_slices(real_data).shuffle(buffer_size).batch(batch_size)

# Start training the GAN
epochs = 50  # Adjust the number of epochs as needed
train(train_dataset, epochs, batch_size)

# Function to generate synthetic order book data
def generate_synthetic_data(generator, scaler, num_samples):
    noise = tf.random.normal([num_samples, 100])  # Generate random noise
    generated_data = generator(noise, training=False)  # Generate data using generator
    generated_data = generated_data.numpy()  # Convert to numpy array
    generated_data = scaler.inverse_transform(generated_data)  # Reverse the preprocessing scaling
    return generated_data  # Return the generated data

# Output generated synthetic data
# Note: For some reason the outputted samples may look like this, for example: '2.2111925e+06' instead of being '2211192'
num_samples_to_generate = 30  # Specify the number of synthetic sample rows you want to generate
synthetic_order_book_data = generate_synthetic_data(generator, scaler, num_samples_to_generate)
print(synthetic_order_book_data)



# # Save the entire generator model
# generator.save('generator_model')

# # Save only the weights of the generator
#generator.save_weights('generator_weights.h5')