import numpy as np
from hw1 import batch_gradient_descent

class params:
    def __init__(self, initial_bias
                    ,alpha
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
        return "Regularized Gradient Descent, params:"\
                +" initial_bias: " +str(self.initial_bias)\
                +" alpha: " +str(self.alpha)\
                +" lambda_reg: " +str(self.lambda_reg)\
                +" num_iter: " +str(self.num_iter)\
                +" checking gradient?: " +str(self.use_grad_checker)
                
###################################################
###Q2.5b: Batch Gradient Descent with regularization term
def run(X, y, params):
    """
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        params - parameters for the algo
    Returns:
        theta_hist - the history of parameter vector, 2D numpy array of size (num_iter+1, num_features) 
        loss_hist - the history of regularized loss value, 1D numpy array
    """
    
    (num_instances, num_features) = X.shape
    theta = np.ones(num_features) #Initialize theta
    theta_hist = np.zeros((params.num_iter+1, num_features))  #Initialize theta_hist
    loss_hist = np.zeros(params.num_iter+1) #Initialize loss_hist
    
    theta_hist[0] = theta
    
    iteration=0
    while(True):
        if(not params.use_grad_checker or\
           generic_regularized_gradient_checker(X, y, theta, params.lambda_reg, compute_loss, compute_loss_gradient)):
            #above line introduces inefficiency as it is already computes
            #loss and gradient for theta, which is again computed below
            loss_hist[iteration] = compute_loss(X, y, theta, params.lambda_reg)
            theta = theta - params.alpha\
                            *compute_loss_gradient(X, y, theta, params.lambda_reg)                        
            theta_hist[iteration+1] = theta
            iteration+=1
            if(iteration>=params.num_iter):
                break
        else:
            raise("Gradient checking failed!")
    loss_hist[iteration] = compute_loss(X, y, theta, params.lambda_reg)
    """    
    else:
        iteration=0
        while(True):
            #print "no_checking_iteration: " +str(iteration)
            
            loss_hist[iteration] = compute_loss(X, y, theta, params.lambda_reg)
            theta = theta - params.alpha*compute_loss_gradient(X, y, theta, params.lambda_reg)   
            theta_hist[iteration+1] = theta
            iteration+=1
            if(iteration>=params.num_iter):
                break
        loss_hist[iteration] = compute_loss(X, y, theta, params.lambda_reg)
       """ 
    return (theta_hist, loss_hist)

def generic_regularized_gradient_checker(X, y, theta, lambda_reg, objective_func, gradient_func, epsilon=0.01, tolerance=1e-4):
    """
    The functions takes objective_func and gradient_func as parameters. And check whether gradient_func(X, y, theta) returned
    the true gradient for objective_func(X, y, theta).
    Eg: In LSR, the objective_func = compute_square_loss, and gradient_func = compute_square_loss_gradient
    """
    true_gradient = gradient_func(X, y, theta, lambda_reg) #the true gradient
    num_features = theta.shape[0]
    approx_grad = np.zeros(num_features)
    for index in range(num_features):
        step = np.zeros(num_features)
        step[index] = epsilon
        approx_grad[index] = (objective_func(X, y, theta+step, lambda_reg)\
                              -objective_func(X, y, theta-step, lambda_reg))/(2*epsilon)
    temp = true_gradient-approx_grad

    if(np.linalg.norm(temp)>tolerance):
        #print "difference in gradient: " +str(temp)
        print "difference scalar: " +str(np.linalg.norm(temp))
        return False
    else:
        return True

def regularized_gradient_checker(X, y, theta, lambda_reg, epsilon=0.01, tolerance=1e-4):
    return generic_regularized_gradient_checker(X, y, theta, lambda_reg, compute_loss, compute_loss_gradient, epsilon, tolerance)
    
###################################################
###Q2.5a: Compute the gradient of Regularized Batch Gradient Descent
def compute_loss_gradient(X, y, theta, lambda_reg):
    """
    Compute the gradient of L2-regularized square loss function given X, y and theta
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D numpy array of size (num_features)
        lambda_reg - the regularization coefficient
    
    Returns:
        grad - gradient vector, 1D numpy array of size (num_features)
    """
    grad = batch_gradient_descent.compute_loss_gradient(X, y, theta)+2*lambda_reg*theta
    return grad

def compute_loss(X, y, theta, lambda_reg):
    """
    Given a set of X, y, theta, compute the square loss for predicting y with X*theta
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D array of size (num_features)
        lambda_reg - the regularization coefficient
    
    Returns:
        loss - the square loss, scalar
    """
    loss = batch_gradient_descent.compute_loss(X, y, theta) + lambda_reg*np.dot(theta, theta)
    return loss
