import numpy as np
from hw1 import batch_gradient_descent
from matplotlib import pyplot as plt
import timeit

def doConjugateDescentLasso(data):
    log_lambdas = np.arange(-2,2,0.5)
    lambdas = np.power(10, log_lambdas)
    
    X = data.train_x
    y = data.train_y
    
    losses = []
    best_loss = np.Inf
    best_lambda = 0.0
    best_weights = np.zeros(X.shape[1])
    time_taken = 0
    plt.subplot(131)
    for lambda_reg in lambdas:
        term = np.linalg.inv(np.dot(np.transpose(X), X))
        term = term + lambda_reg*np.identity(term.shape[0])
        term2 = np.dot(np.transpose(X), y)
        initial_weights = np.dot(term, term2)
        #print "lambda: " +str(lambda_reg) + "weights: " +str(initial_weights)
        
        start = timeit.default_timer()
        weights, loss_hist = minimize(X, y, lambda_reg, initial_weights)
        time_taken += timeit.default_timer() - start
        
        loss_hinge = batch_gradient_descent.compute_loss(data.validation_x, data.validation_y, weights)
        losses.append(loss_hinge)
        if(loss_hinge<best_loss):
            best_loss = loss_hinge
            best_weights = weights
            best_lambda = lambda_reg
        plt.plot(loss_hist, label="lambda: " +str(lambda_reg))
    plt.legend()
    
    print "best loss_hinge: " +str(best_loss) + " at lambda: " +str(best_lambda)
    print "number of weights in vicinity(10^-2) of zero: " +str((best_weights[np.abs(best_weights)<(10**-2)]).size)
    print "avg time taken: " +str(time_taken/lambdas.size)
    
    plt.subplot(132)
    plt.plot(np.log(losses), log_lambdas)
    plt.yticks(log_lambdas)
    plt.subplot(133)
    plt.plot(best_weights)
    plt.show()
    
def minimize(X, y, lambda_reg, weights):
    dimensions = weights.size
    #max_iter=200
    iter=0
    gain = 10
    losses = []
    while(not gain<10**-4):
        weights_old = np.copy(weights)
        for dim in range(dimensions):
            a_dim = 2*(np.linalg.norm(X[:,dim])**2)
            term = y - np.dot(X,weights) + weights[dim]*X[:,dim]
            c_dim = 2*(np.dot(X[:,dim], term ))
            weights[dim] = soft(c_dim/a_dim, lambda_reg/a_dim)
            
            loss_hinge = batch_gradient_descent.compute_loss(X,y,weights)
            losses.append(loss_hinge)
            iter+=1
        gain = np.abs(np.linalg.norm(weights-weights_old))
        #print "gain: " +str(gain)
        
    print "lambda: " +str(lambda_reg) +", iterations: " +str(iter)
    return weights, losses

def minimize_slow(X, y, lambda_reg, weights):
    samples, dimensions = X.shape
    #max_iter=200
    iter=0
    gain = 10
    losses = []
    while(not gain<10**-3):
        weights_old = np.copy(weights)
        for dim in range(dimensions):
            loss_prev = batch_gradient_descent.compute_loss(X, y, weights)
            
            a_dim = 0#2*(np.linalg.norm(X[:,dim])**2)
            c_dim = 0
            for sample in range(samples):
                a_dim += np.square(X[sample, dim])
                term = y[sample] - np.dot(X[sample,:],weights) + weights[dim]*X[sample,dim]
                c_dim += X[sample,dim]*term
            
            a_dim *=2
            c_dim *=2
            
            weights[dim] = soft(c_dim/a_dim, lambda_reg/a_dim)
            
            loss_hinge = batch_gradient_descent.compute_loss(X,y,weights)
            losses.append(loss_hinge)
            iter+=1
        gain = np.abs(np.linalg.norm(weights-weights_old))
        
    #print "lambda: " +str(lambda_reg) +", iterations: " +str(iter)
    return weights, losses
            
def soft(value, threshold):
    term = np.abs(value)-threshold
    if term<0: term=0
    return np.sign(value)*term