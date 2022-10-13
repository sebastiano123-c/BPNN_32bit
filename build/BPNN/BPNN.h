/**
 * @file BPNN.h
 * @author Sebastiano Cocchi
 * @brief This file contains the subroutines to compute a Back Propagation (BP)
 * neural network (NN).
 * @version 0.1
 * @date 2022-05-09
 *
 * Disclaimer: this is not a class to guarantee more speed in execution.
 *
 * Refs:
 *  1) http://yann.lecun.com/exdb/publis/pdf/lecun-98b.pdf
 *  2) https://towardsdatascience.com/backpropagation-the-natural-proof-946c5abf63b1
 *
 *
 * Compile the lib:
 *     <path> C:\MinGW\bin\gcc -c BPNN.cpp -o BPNN.o
 *     <path> C:\MinGW\bin\ar rcs BPNN.a BPNN.o
 * 
 * 
 * @copyright Copyright (c) 2022
 *
 */
#include <cmath>
#include <cstring>
#include <vector>
#include <cstdlib>

#define DEFAULT_ACTIVATION_FUNCTION "sigmoid"

#ifndef BPNN_H
#define BPNN_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialize and randomize vectors and matrices (the bias vectors and
 * the weights matrices) before the calculations.
 *
 * @param structure vector containing the size of each layer, e.g. {n_in, n_h1,
 * n_h2, ..., n_out}
 * @param z 2D vector whose rows are the input state of each layer
 * @param a 2D vector whose rows are the output state of each layer
 * @param bias 2D vector whose rows are the bias state of each layer
 * @param deltaBias 2D vector whose rows are the increment for the biases
 * @param weights 3D vector whose 2D matrices are the weights of each m,m+1
 * layer
 * @param deltaWeights 3D vector whose 2D matrices are the weights increments
 * @param randomAmplitude (default 1.0f) random numbers maximum value
 * @param finesse (default 1000) digits after the dot of random numbers
 */
static void init(std::vector<int> structure, std::vector<std::vector<float> > &z,
          std::vector<std::vector<float> > &a,
          std::vector<std::vector<float> > &bias,
          std::vector<std::vector<float> > &deltaBias,
          std::vector<std::vector<std::vector<float> > > &weights,
          std::vector<std::vector<std::vector<float> > > &deltaWeights,
          float randomAmplitude = 1.0f, int finesse = 1000)
{
  // declare counters
  int ii, jj = -1, kk;

  // roll over the structure vector
  for (std::vector<int>::iterator itInt = structure.begin();
       itInt != structure.end(); itInt++, jj++)
  {
    a[jj + 1].resize(*(itInt)); // 0) resize the input and output layer vectors

    if (jj > -1)
    {
      z[jj].resize(*itInt); // 0) resize the input and output layer vectors

      // 1) randomize the bias vector
      std::vector<float> tempArr(*itInt);
      bias[jj].resize(*itInt); // resize the bias vector
      deltaBias[jj].resize(
          *itInt); // resize the Delta bias vector for the future

      for (ii = 0; ii < *itInt; ii++)
        tempArr[ii] =
            randomAmplitude * (float)(rand() % finesse) / (float)finesse;

      bias[jj] = (tempArr);

      // 2) randomize the weight vector
      std::vector<std::vector<float> > tempMat(*(itInt - 1),
                                              std::vector<float>(*itInt));
      weights[jj].resize(*(itInt - 1),
                         std::vector<float>(*itInt)); // resize the weights
      deltaWeights[jj].resize(
          *(itInt - 1),
          std::vector<float>(*itInt)); // resize the Delta weights

      for (kk = 0; kk < *(itInt - 1); kk++)
        for (ii = 0; ii < *itInt; ii++)
        {
          tempMat[kk][ii] =
              randomAmplitude * (float)(rand() % finesse) / (float)finesse;
        }

      weights[jj] = tempMat;
    }
  }
}

/**
 * @brief Forward propagation.
 *
 * @param structure vector containing the size of each layer, e.g. {n_in, n_h1,
 * n_h2, ..., n_out}
 * @param inputState ouput state of the initial layer, i.e. the initial state
 * @param z 2D vector whose rows are the input state of each layer
 * @param a 2D vector whose rows are the output state of each layer
 * @param bias 2D vector whose rows are the bias state of each layer
 * @param weights 3D vector whose 2D matrices are the weights of each m,m+1
 * layer
 * @param activationFunctionName (default DEFAULT_ACTIVATION_FUNCTION) vector
 * of chars containing the list of names of the activation functions for each
 * layer.
 */
void forwardPropagation(std::vector<int> structure,
                        std::vector<float> inputState,
                        std::vector<std::vector<float> > &z,
                        std::vector<std::vector<float> > &a,
                        std::vector<std::vector<float> > &bias,
                        std::vector<std::vector<std::vector<float> > > &weights,
                        std::vector<const char *> activationFunctionName);


/**
 * @brief Back propagates the errors and changes the weights matrices
 * accordingly to the gradient descendent method.
 *
 * @param structure vector containing the size of each layer, e.g. {n_in, n_h1,
 * n_h2, ..., n_out}
 * @param y 1D vector expected results
 * @param z 2D vector whose rows are the input state of each layer
 * @param a 2D vector whose rows are the output state of each layer
 * @param bias 2D vector whose rows are the bias state of each layer
 * @param deltaBias 2D vector whose rows are the increment for the biases
 * @param weights 3D vector whose 2D matrices are the weights of each m,m+1
 * layer
 * @param deltaWeights 3D vector whose 2D matrices are the weights increments
 * @param learningRate (float), learning rate eta
 * @param momentumFactor (float, default 1.0f), alpha s.t. w = alpha*w + eta *
 * delta * output
 * @param learningType (const char*; "online", "batch", default = ""), select
 * the type of the learning algorithm. If "online", it will update at every
 * iteration the weights matrix. If "", it will NOT update the weights. If
 * "batch", it will update the sum of all the pasts corrections. If you want to
 * use a mini batch learning type, you will select "" for the entire learning
 * set until the last one. At that point you will select "batch" to update the
 * weights will all the past corrections.
 * @param activationFunctionName (default DEFAULT_ACTIVATION_FUNCTION) vector
 * of chars containing the list of names of the activation functions for each
 * layer.
 *
 */
void backPropagation(
    std::vector<int> structure, std::vector<float> y,
    std::vector<std::vector<float> > &z, std::vector<std::vector<float> > &a,
    std::vector<std::vector<float> > &bias,
    std::vector<std::vector<float> > &deltaBias,
    std::vector<std::vector<std::vector<float> > > &weights,
    std::vector<std::vector<std::vector<float> > > &deltaWeights,
    float learningRate, float momentumFactor,
    const char *learningType,
    std::vector<const char *> activationFunctionName);

#ifdef __cplusplus
   }
#endif

#endif /* BPNN_H */