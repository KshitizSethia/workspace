import numpy as np
from hw1 import regularized_gradient_descent
import math
from numpy import Inf

class params:
    def __init__(self, initial_bias=1
                    ,alpha=0.1
                    ,lambda_reg=1
                    ,num_iter=1000
                    ,use_grad_checker=False):
        """Args:
            initial_bias - bias term, increase to minimize regularization on bias
            alpha - step size in gradient descent
            lambda_reg - the regularization coefficient
            numIter - number of iterations to run 
            grad_checker - a boolean value indicating whether checking the gradient when updating
        """
        self.initial_bias     = initial_bias
        self.alpha            = alpha           
        self.lambda_reg       = lambda_reg      
        self.num_iter         = num_iter        
        self.use_grad_checker = use_grad_checker
    
    def __str__(self):
        return "Stochastic Gradient Descent, params:"\
                +" initial_bias: " +str(self.initial_bias)\
                +" alpha: " +str(self.alpha)\
                +" lambda_reg: " +str(self.lambda_reg)\
                +" num_iter: " +str(self.num_iter)\
                +" checking gradient?: " +str(self.use_grad_checker)

#############################################
###Q2.6a: Stochastic Gradient Descent
def run(X, y, params):
    """
    In this question you will implement stochastic gradient descent with a regularization term
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        alpha - string or float. step size in gradient descent
                NOTE: In SGD, it's not always a good idea to use a fixed step size. Usually it's set to 1/sqrt(t) or 1/t
                if alpha is a float, then the step size in every iteration is alpha.
                if alpha == "1/sqrt(t)", alpha = 1/sqrt(t)
                if alpha == "1/t", alpha = 1/t
        lambda_reg - the regularization coefficient
        num_iter - number of epochs (i.e number of times) to go through the whole training set
    
    Returns:
        theta_hist - the history of parameter vector, 3D numpy array of size (num_iter, num_instances, num_features) 
        loss hist - the history of regularized loss function vector, 2D numpy array of size(num_iter, num_instances)
    """
    num_instances, num_features = X.shape[0], X.shape[1]
    theta = np.ones(num_features) #Initialize theta
    #theta = np.zeros(num_features) #Initialize theta
    
    
    theta_hist = np.zeros((params.num_iter, num_instances, num_features))  #Initialize theta_hist
    loss_hist = np.zeros((params.num_iter, num_instances)) #Initialize loss_hist
    
    best_theta=theta
    least_loss = Inf
    #theta_hist[0,0,:] = theta
    iter = 0
    for epoch in range(params.num_iter):
        for instance in range(num_instances):
            if(not params.use_grad_checker\
               or hw1.regularized_gradient_descent.regularized_gradient_checker\
                                        (X[instance], y[instance], theta,\
                                        params.lambda_reg)):
                #step sizes
                step = params.alpha/math.sqrt(iter+1)
                #step = 1/math.sqrt(iter+1)
                #step = 1/(iter+1)
                gradient = hw1.regularized_gradient_descent.\
                            compute_loss_gradient(X[instance], y[instance],\
                                                  theta, params.lambda_reg)
                theta = theta - (gradient*step)
                theta_hist[epoch, instance, :] = theta
                loss = hw1.regularized_gradient_descent.compute_loss(X[instance], y[instance], theta, params.lambda_reg)
                loss_hist[epoch, instance] = loss
                if(loss<least_loss):
                    least_loss = loss
                    best_theta=theta
                iter+=1
            
    return theta_hist, loss_hist, best_theta
