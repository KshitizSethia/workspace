import numpy as np
from hw1 import batch_gradient_descent
from conjugate_grad_desent_lasso import minimize
from matplotlib import pyplot as plt
import timeit

def doConjugateDescentLassoWithHomotopy(data):
    
    X = data.train_x
    y = data.train_y
    
    lambda_max = 2*np.linalg.norm(np.dot(np.transpose(X),(y-np.mean(y))), ord=np.Inf)  
    log_lambdas = np.arange(np.log10(lambda_max),-2,-0.1)
    lambdas = np.power(10, log_lambdas)
    
    losses = []
    best_loss = np.Inf
    best_lambda = 0.0
    best_weights = np.zeros(X.shape[1])
    time_taken = 0
    
    #homotopy
    term = np.linalg.inv(np.dot(np.transpose(X), X))
    term = term + lambda_max*np.identity(term.shape[0])
    term2 = np.dot(np.transpose(X), y)
    weights_for_warm_starting = np.dot(term, term2)
    
    plt.subplot(131)
    for lambda_reg in lambdas:
        
        start = timeit.default_timer()
        weights_for_warm_starting, loss_hist = minimize(X, y, lambda_reg, weights_for_warm_starting)
        time_taken += timeit.default_timer() - start
        
        loss_hinge = batch_gradient_descent.compute_loss(data.validation_x, data.validation_y, weights_for_warm_starting)
        losses.append(loss_hinge)
        if(loss_hinge<best_loss):
            best_loss = loss_hinge
            best_weights = weights_for_warm_starting
            best_lambda = lambda_reg
        plt.plot(loss_hist, label="lambda: " +str(lambda_reg))
    plt.legend()
    
    print "best loss_hinge: " +str(best_loss) + " at lambda: " +str(best_lambda)
    print "number of weights in vicinity of zero: " +str((best_weights[np.abs(best_weights)<(10**-2)]).size)
    print "avg time taken: " +str(time_taken/lambdas.size)
    
    plt.subplot(132)
    plt.plot(np.log(losses), log_lambdas)
    plt.yticks(log_lambdas)
    plt.subplot(133)
    plt.plot(best_weights)
    plt.show()
