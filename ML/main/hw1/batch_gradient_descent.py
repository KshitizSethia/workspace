import numpy as np

class params:
    def __init__(self
                    #,initial_bias=1
                    ,alpha=0.1
                    #,lambda_reg=1
                    ,num_iter=1000
                    ,use_grad_checker=False
                    ):
        """Args:
            initial_bias - bias term, increase to minimize regularization on bias
            alpha - step size in gradient descent
            lambda_reg - the regularization coefficient
            numIter - number of iterations to run 
            grad_checker - a boolean value indicating whether checking the gradient when updating
        """
        #self.initial_bias     = initial_bias
        self.alpha            = alpha           
        #self.lambda_reg       = lambda_reg      
        self.num_iter         = num_iter        
        self.use_grad_checker = use_grad_checker
    
    def __str__(self):
        return "Batch Gradient Descent, params:"\
                +" alpha: " +str(self.alpha)\
                +" num_iter: " +str(self.num_iter)\
                +" checking gradient?: " +str(self.use_grad_checker)
                #+" initial_bias: " +str(self.initial_bias)\
                #+" lambda_reg: " +str(self.lambda_reg)\
                

####################################
####Q2.4a: Batch Gradient Descent
def run(X, y, params):
    """
    In this question you will implement batch gradient descent to
    minimize the square loss_hinge objective
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        params - parameters for the algo
    Returns:
        theta_hist - store the the history of parameter vector in iteration, 2D numpy array of size (num_iter+1, num_features) 
                    for instance, theta in iteration 0 should be theta_hist[0], theta in ieration (num_iter) is theta_hist[-1]
        loss_hist - the history of objective function vector, 1D numpy array of size (num_iter+1) 
    """
    (num_instances, num_features) = X.shape
    theta = np.ones(num_features) #initialize theta
    theta_hist = np.zeros((params.num_iter+1, num_features))  #Initialize theta_hist
    loss_hist = np.zeros(params.num_iter+1) #initialize loss_hist
    
    theta_hist[0] = theta
    
    iteration=0
    while(True):
        if(not params.use_grad_checker\
           or generic_gradient_checker(X, y, theta, compute_loss, compute_loss_gradient)):
            #above line introduces inefficiency as it is already computes
            #loss_hinge and gradient for theta, which is again computed below
            loss_hist[iteration] = compute_loss(X, y, theta)
            theta = theta - params.alpha*compute_loss_gradient(X, y, theta)   
            theta_hist[iteration+1] = theta
            iteration+=1
            if(iteration>=params.num_iter):
                break
        else:
            raise("Gradient checking failed!")
    
    loss_hist[iteration] = compute_loss(X, y, theta)
    
    return (theta_hist, loss_hist)

###########################################
###Q2.3a: Gradient Checker
#Getting the gradient calculation correct is often the trickiest part
#of any gradient-based optimization algorithm.  Fortunately, it's very
#easy to check that the gradient calculation is correct using the
#definition of gradient.
#See http://ufldl.stanford.edu/wiki/index.php/Gradient_checking_and_advanced_optimization
#def grad_checker(X, y, theta, epsilon=0.01, tolerance=1e-4): 
    """Implement Gradient Checker
    Check that the function compute_loss_gradient returns the
    correct gradient for the given X, y, and theta.

    Let d be the number of features. Here we numerically estimate the
    gradient by approximating the directional derivative in each of
    the d coordinate directions: 
    (e_1 = (1,0,0,...,0), e_2 = (0,1,0,...,0), ..., e_d = (0,...,0,1) 

    The approximation for the directional derivative of J at the point
    theta in the direction e_i is given by: 
    ( J(theta + epsilon * e_i) - J(theta - epsilon * e_i) ) / (2*epsilon).

    We then look at the Euclidean distance between the gradient
    computed using this approximation and the gradient computed by
    compute_loss_gradient(X, y, theta).  If the Euclidean
    distance exceeds tolerance, we say the gradient is incorrect.

    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D numpy array of size (num_features)
        epsilon - the epsilon used in approximation
        tolerance - the tolerance error
    
    Return:
        A boolean value indicate whether the gradient is correct or not

    """
    """
    true_gradient = compute_loss_gradient(X, y, theta) #the true gradient
    num_features = theta.shape[0]
    approx_grad = np.zeros(num_features)
    for index in range(num_features):
        step = np.zeros(num_features)
        step[index] = epsilon
        approx_grad[index] = (compute_loss(X, y, theta+step)-compute_loss(X, y, theta-step))/(2*epsilon)

    if(np.linalg.norm(true_gradient-approx_grad)>tolerance):
        return False
    else:
        return True
"""
#################################################
###Q2.3b: Generic Gradient Checker
def generic_gradient_checker(X, y, theta, objective_func, gradient_func, epsilon=0.01, tolerance=1e-4):
    """
    The functions takes objective_func and gradient_func as parameters. And check whether gradient_func(X, y, theta) returned
    the true gradient for objective_func(X, y, theta).
    Eg: In LSR, the objective_func = compute_loss, and gradient_func = compute_loss_gradient
    """
    true_gradient = gradient_func(X, y, theta) #the true gradient
    num_features = theta.shape[0]
    approx_grad = np.zeros(num_features)
    for index in range(num_features):
        step = np.zeros(num_features)
        step[index] = epsilon
        approx_grad[index] = (objective_func(X, y, theta+step)\
								-objective_func(X, y, theta-step))/(2*epsilon)

    if(np.linalg.norm(true_gradient-approx_grad)>tolerance):
        return False
    else:
        return True

########################################
###Q2.2b: compute the gradient of square loss_hinge function
def compute_loss_gradient(X, y, theta):
    """
    Compute gradient of the square loss_hinge (as defined in compute_loss), at the point theta.
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D numpy array of size (num_features)
    
    Returns:
        grad - gradient vector, 1D numpy array of size (num_features)
    """
    temp = np.dot(X, theta)
    temp = np.transpose(temp - y)
    grad = np.dot(temp, X)
    return grad/y.size

########################################
####Q2.2a: The square loss_hinge function

def compute_loss(X, y, theta):
    """
    Given a set of X, y, theta, compute the square loss_hinge for predicting y with X*theta
    
    Args:
        X - the feature vector, 2D numpy array of size (num_instances, num_features)
        y - the label vector, 1D numpy array of size (num_instances)
        theta - the parameter vector, 1D array of size (num_features)
    
    Returns:
        loss_hinge - the square loss_hinge, scalar
    """
    term = np.dot(X,theta)
    term = (term - y)
    #loss_hinge = (np.linalg.norm(term)**2)/(2*y.size)
    loss_hinge = np.dot(term, term)/(2*y.size)
    return loss_hinge
