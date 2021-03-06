import numpy as np
from hw1 import batch_gradient_descent
from scipy.optimize import minimize

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
        loss_hist - the history of regularized loss_hinge value, 1D numpy array
    """
    
    (num_instances, num_features) = X.shape
    theta = np.ones(num_features) #Initialize theta
    solution = minimize(objective(X,y, params.lambda_reg), theta)
    return solution.x
    
def objective(X, y, lambda_reg):
    def objective_lambda(theta):
        return compute_loss(X, y, theta, lambda_reg)
    return objective_lambda
    
def compute_loss(X, y, theta, lambda_reg):
    """
    Given a set of X, y, theta, compute the square loss_hinge for predicting y with X*theta
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D array of size (num_features)
        lambda_reg - the regularization coefficient
    
    Returns:
        loss_hinge - the square loss_hinge, scalar
    """
    loss_hinge = batch_gradient_descent.compute_loss(X, y, theta) + lambda_reg*np.dot(theta,theta)#(np.linalg.norm(theta)**2)
    return loss_hinge
