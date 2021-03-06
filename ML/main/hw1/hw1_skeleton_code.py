import pandas as pd
import logging
import numpy as np
import sys
import matplotlib.pyplot as plt
from sklearn.cross_validation import train_test_split
from hw1 import batch_gradient_descent
from hw1 import regularized_gradient_descent
import timeit
from hw1 import stochastic_gradient_descent
from numpy import Inf
import helpers

### Assignment Owner: Hao Xu
### Edited By: Kshitiz Sethia

#######################################
####Q2.1: Normalization
def feature_normalization(train, test):
    """Rescale the data so that each feature in the training set is in
    the interval [0,1], and apply the same transformations to the test
    set, using the statistics computed on the training set.
    
    Args:
        train - training set, a 2D numpy array of size (num_instances, num_features)
        test  - test set, a 2D numpy array of size (num_instances, num_features)
    Returns:
        train_normalized - training set after normalization
        test_normalized  - test set after normalization

    """
    mins_of_features = np.amin(train, axis=0)
    maxs_of_features = np.amax(train, axis=0)
    range_of_features = maxs_of_features-mins_of_features
    range_of_features[range_of_features==0] = 1
    
    train_normalized = (train - mins_of_features)/range_of_features
    test_normalized = (test - mins_of_features)/range_of_features
    
    return (train_normalized, test_normalized)
    
####################################
###Q2.4b: Implement backtracking line search in batch_gradient_descent
###Check http://en.wikipedia.org/wiki/Backtracking_line_search for details
#TODO
    
#2.4.2 Plotting loss vs iterations for various values of step sizes
def batch_analysis(X_train, y_train, X_test, y_test):
    iterations = 1000
    alphas = [0.0001,0.001,0.01,0.02,0.03,0.04,0.1]
    test_losses = np.zeros(len(alphas))
    
    #plot training losses
    plt.subplot(121)
    for index in range(len(alphas)):
        batch_params = batch_gradient_descent.params(alphas[index], iterations, False)
        theta_hist, loss_hist = batch_gradient_descent.run(X_train, y_train, batch_params)
        test_losses[index] = batch_gradient_descent.compute_loss(X_test, y_test, theta_hist[-1,:])
        plt.plot(np.log(loss_hist), label=str(alphas[index]))
    
    plt.xlabel("iteration")
    plt.ylabel("log(training loss)")
    plt.title("Analysis of Batch gradient descent for various values of alpha (in legend)")
    
    plt.legend()
    
    plt.subplot(122)
    plt.plot(test_losses, np.log10(alphas))
    plt.yticks(np.log10(alphas))
    plt.title("X axis: log10(alpha)")
    plt.xlabel("log10(alpha)")
    plt.xlabel("test loss")
    
    plt.show()
    
#############################################
##Q2.5c: Visualization of Regularized Batch Gradient Descent
##X-axis: log(lambda_reg)
##Y-axis: square_loss
def regularized_analysis(X_train, y_train, X_test, y_test):
    
    iterations = 1000
    lambdas = [0.005,0.01, 0.1, 1, 10]
    initial_biases = [0.001,0.01,0.1,1,5,10,20,30,150,1000]
    #alphas = [0.0001,0.001,0.01,0.02,0.03,0.04,0.1]
    
    test_losses_lambda = np.zeros(len(lambdas))
    test_losses_bias = np.zeros(len(initial_biases))
    
    initial_bias_for_lambda_sweep=1
    best_lambda_index = 0
    best_bias_index = 0
    best_test_loss=Inf
    
    #plot training losses
    plt.subplot(131)
    for index_lambda in range(len(lambdas)):
        #for index_bias in range(len(initial_biases)):
        reg_params = hw1.regularized_gradient_descent.params(initial_bias_for_lambda_sweep, 0.01, lambdas[index_lambda], iterations, False)
        theta_hist, loss_hist = hw1.regularized_gradient_descent.run(X_train, y_train, reg_params)
        test_loss = hw1.batch_gradient_descent.compute_loss(X_test, y_test, theta_hist[-1,:])
        if(test_loss<best_test_loss):
            best_lambda_index = index_lambda
            best_test_loss = test_loss
        test_losses_lambda[index_lambda] = test_loss
        plt.plot(np.log(loss_hist), label="bias= " +str(reg_params.initial_bias) +", lambda= " +str(reg_params.lambda_reg))
    
    for index_bias in range(len(initial_biases)):
        reg_params = hw1.regularized_gradient_descent.params(initial_biases[index_bias], 0.01, lambdas[best_lambda_index], iterations, False)
        theta_hist, loss_hist = hw1.regularized_gradient_descent.run(X_train, y_train, reg_params)
        test_loss = hw1.batch_gradient_descent.compute_loss(X_test, y_test, theta_hist[-1,:])
        if(test_loss<best_test_loss):
            best_bias_index = index_bias
            best_test_loss = test_loss
        test_losses_bias[index_bias] = test_loss
        plt.plot(np.log(loss_hist), label="bias= " +str(reg_params.initial_bias) +", lambda= " +str(reg_params.lambda_reg))
    
    reg_params = hw1.regularized_gradient_descent.params(initial_biases[best_bias_index],\
                                                      0.01, lambdas[best_lambda_index], iterations, False)
    theta_hist,loss_hist = hw1.regularized_gradient_descent.run(X_train,y_train, reg_params)
    print "best test loss: " +str(hw1.regularized_gradient_descent.compute_loss(X_test, y_test, theta_hist[-1,:], reg_params.lambda_reg))
    
    
    plt.xlabel("iteration")
    plt.ylabel("log(training loss)")
    plt.title("Analysis of Regularized gradient descent: sweeping bias and lambda")
    plt.legend()
    
    plt.subplot(132)
    plt.title("Lambda sweep with bias= " +str(initial_bias_for_lambda_sweep)\
              +" | X: test_loss, Y: log10(lambdas)")
    plt.plot(test_losses_lambda, np.log10(lambdas))
    plt.yticks(np.log10(lambdas))
    
    plt.subplot(133)
    plt.title("Bias sweep with lambda= " +str(lambdas[best_lambda_index])\
              +" | X: test_loss, Y: log10(initial_bias)")
    plt.plot(test_losses_bias, np.log10(initial_biases))
    plt.yticks(np.log10(initial_biases))
    
    
    plt.show()


def stochastic_analysis(X_train, y_train, X_test, y_test):
    
    iterations = 10
    alpha = 0.1
    lambdas = [0.01,0.1,1,10,100]
    initial_biases = [0.1,1,10]
    #alphas = [0.0001,0.001,0.01,0.02,0.03,0.04,0.1]
    
    test_losses_lambda = np.zeros(len(lambdas))
    test_losses_bias = np.zeros(len(initial_biases))
    
    initial_bias_for_lambda_sweep=1
    best_lambda_index = 0
    best_bias_index = 0
    best_test_loss=Inf
    
    #plot training losses
    plt.subplot(131)
    for index_lambda in range(len(lambdas)):
        #for index_bias in range(len(initial_biases)):
        reg_params = hw1.stochastic_gradient_descent.params(initial_bias_for_lambda_sweep, alpha, lambdas[index_lambda], iterations, False)
        theta_hist, loss_hist, best_theta = hw1.stochastic_gradient_descent.run(X_train, y_train, reg_params)
        test_loss = hw1.batch_gradient_descent.compute_loss(X_test, y_test, best_theta)
        if(test_loss<best_test_loss):
            best_lambda_index = index_lambda
            best_test_loss = test_loss
        test_losses_lambda[index_lambda] = test_loss
        plt.plot(np.log(np.ravel(loss_hist)), label="bias= " +str(reg_params.initial_bias) +", lambda= " +str(reg_params.lambda_reg))
    
    for index_bias in range(len(initial_biases)):
        reg_params = hw1.stochastic_gradient_descent.params(initial_biases[index_bias], alpha, lambdas[best_lambda_index], iterations, False)
        theta_hist, loss_hist, best_theta = hw1.stochastic_gradient_descent.run(X_train, y_train, reg_params)
        test_loss = hw1.batch_gradient_descent.compute_loss(X_test, y_test, best_theta)
        if(test_loss<best_test_loss):
            best_bias_index = index_bias
            best_test_loss = test_loss
        test_losses_bias[index_bias] = test_loss
        plt.plot(np.log(np.ravel(loss_hist)), label="bias= " +str(reg_params.initial_bias) +", lambda= " +str(reg_params.lambda_reg))
    #
    reg_params = hw1.stochastic_gradient_descent.params(initial_biases[best_bias_index],\
                                                      alpha,lambdas[best_lambda_index], iterations, False)
    theta_hist,loss_hist, best_theta = hw1.stochastic_gradient_descent.run(X_train,y_train, reg_params)
    print "best test loss: " +str(hw1.batch_gradient_descent.compute_loss(X_test, y_test, best_theta))
    
    
    plt.xlabel("iteration")
    plt.ylabel("log(training loss)")
    plt.title("Analysis of stochastic gradient descent: sweeping bias and lambda")
    plt.legend()
    
    plt.subplot(132)
    plt.title("Lambda sweep with bias= " +str(initial_bias_for_lambda_sweep)\
              +" | X: test_loss, Y: log10(lambdas)")
    plt.plot(np.log(test_losses_lambda), np.log10(lambdas))
    plt.yticks(np.log10(lambdas))
    
    plt.subplot(133)
    plt.title("Bias sweep with lambda= " +str(lambdas[best_lambda_index])\
              +" | X: test_loss, Y: log10(initial_bias)")
    plt.plot(test_losses_bias, np.log10(initial_biases))
    plt.yticks(np.log10(initial_biases))
    
    
    plt.show()



def plot_iterations_vs_loss(type, loss_hist):
    figure = plt.figure()
    rect = 0.1,0.1,0.8,0.8
    iterations = len(loss_hist)
    
    axes = figure.add_axes(rect)
    axes.plot(np.arange(iterations), loss_hist, 'r')
    axes.set_xlabel("iteration")
    axes.set_ylabel("loss")

    title=type
    
    axes.set_title(title)
    plt.show()

def do_batch_gradient_descent(X_train, y_train, X_test, y_test):
    batch_params = hw1.batch_gradient_descent.params(0.01, 1000, False)
    
    start = timeit.default_timer()
    theta_hist, loss_hist = hw1.batch_gradient_descent.run(X_train, y_train, batch_params)
    elapsed = timeit.default_timer() - start
    print_summary(X_train, y_train, X_test, y_test, elapsed, theta_hist[-1], batch_params)
    #plot_iterations_vs_loss("batch", loss_hist)

def do_regularized_gradient_descent(X_train, y_train, X_test, y_test):
    reg_params = hw1.regularized_gradient_descent.params(10, 0.01, 0.1, 1000, False)
    X_train, X_test = helpers.modify_bias_term(X_train, X_test, reg_params.initial_bias)
    start = timeit.default_timer()
    theta_hist, loss_hist = hw1.regularized_gradient_descent.run(X_train, y_train, reg_params)
    elapsed = timeit.default_timer() - start
    print_summary(X_train, y_train, X_test, y_test, elapsed, theta_hist[-1], reg_params)
    #plot_iterations_vs_loss("regularized", loss_hist)
    
def do_stochastic_gradient_descent(X_train, y_train, X_test, y_test):
    stoc_params = hw1.stochastic_gradient_descent.params(0.1, 0.1, 0.1, 10, False)
    X_train, X_test = helpers.modify_bias_term(X_train, X_test, stoc_params.initial_bias)
    
    start = timeit.default_timer()
    theta_hist, loss_hist, best_theta = hw1.stochastic_gradient_descent.run(X_train, y_train, stoc_params)
    elapsed = timeit.default_timer() - start
    print_summary(X_train, y_train, X_test, y_test, elapsed, best_theta, stoc_params)
    #plot_iterations_vs_loss("stochastic", np.ravel(loss_hist))
    
def print_summary(X_train, y_train, X_test, y_test, time_taken, theta, params):
    test_loss = hw1.batch_gradient_descent.compute_loss(X_test, y_test, theta)
    train_loss = hw1.batch_gradient_descent.compute_loss(X_train, y_train, theta)
    #plot_iterations_vs_loss(iterations, loss_hist, alpha, test_loss, lambda_reg)
    print str(params)\
            +"\n  training loss: " +str(train_loss)\
            +" test_loss: " +str(test_loss)\
            +" time taken: " +str(time_taken)\

################################################
###Q2.6b Visualization that compares the convergence speed of batch
###and stochastic gradient descent for various approaches to step_size
##X-axis: Step number (for gradient descent) or Epoch (for SGD)
##Y-axis: log(objective_function_value)


def main():
    #use this to log to file, use when printing lots of stuff
    #sys.stdout = open("log.txt", "w")
    
    #Loading the dataset
    print('loading the dataset')
    
    #df = pd.read_csv('hw1_my_data.csv', delimiter=',')
    df = pd.read_csv('hw1-data.csv', delimiter=',')
    X = df.values[:,:-1]
    y = df.values[:,-1]
    print "range of y: " +str(y.max()-y.min())
    
    print('Split into Train and Test')
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size =0.4, random_state=10)
    
    print("Scaling all to [0, 1]")
    X_train, X_test = feature_normalization(X_train, X_test)
    X_train = np.hstack((X_train, np.ones((X_train.shape[0], 1)))) #Add bias term
    X_test = np.hstack((X_test, np.ones((X_test.shape[0], 1)))) #Add bias term
    
    #run these for checking output
    #do_batch_gradient_descent(X_train, y_train, X_test, y_test)
    #do_regularized_gradient_descent(X_train, y_train, X_test, y_test)
    do_stochastic_gradient_descent(X_train, y_train, X_test, y_test)

    #batch_analysis(X_train, y_train, X_test, y_test)
    #regularized_analysis(X_train, y_train, X_test, y_test)
    #stochastic_analysis(X_train, y_train, X_test, y_test)
    
    
if __name__ == "__main__":
    main()
