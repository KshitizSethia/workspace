import matplotlib.pyplot as plt
import numpy as np
from sklearn.cross_validation import train_test_split
import helpers
from hw1 import regularized_gradient_descent
import math
from hw1 import batch_gradient_descent
from sklearn.linear_model.ridge import Ridge
import regularized_gradient_descent_optimize
import conjugate_grad_desent_lasso
import conjugate_grad_desent_lasso_homotopy

csvFileName = "hw2_autogen_data.csv"
    
#1.1 Dataset construction
def generateData():
    numExamples=150
    numFeatures=75
    
    samples = np.random.rand(numExamples, numFeatures)
    trueWeight = np.zeros(numFeatures)
    for index in range(10):
        trueWeight[index] = 10
    for index in range(10,20):
        trueWeight[index] = -10
    
    noise = np.random.randn(numExamples)
    trueOutput = np.dot(samples,trueWeight) + noise
    
    dataToStore = np.c_[samples, trueOutput];
    
    np.savetxt(csvFileName, dataToStore,delimiter=",")

def splitData(dataArray):
    X = dataArray[:,:-1]
    y = dataArray[:,-1]
    
    X_train, X_test, y_train, y_test = train_test_split(X, y, train_size=80, random_state=10)
    X_test, X_validate, y_test, y_validate = train_test_split(X_test, y_test, train_size=50, random_state=10)
    
    return helpers.data(X_train, X_validate, X_test, y_train, y_validate, y_test)

def doGridSearch(data, params):
    log_lambdas=np.arange(-7, 2, 0.5)
    lambdas = np.power(10, log_lambdas)
    
    #get best lambda
    losses =[]
    best_loss = np.Inf
    best_lambda = 0
    best_theta = np.zeros(data.train_x.shape[1])
    for lambda_reg in lambdas:
        params.lambda_reg = lambda_reg
        #old ridge: my code
        #theta_hist, loss_hist = regularized_gradient_descent.run(data.train_x, data.train_y, params)
        #loss_hinge = batch_gradient_descent.compute_loss(data.validation_x, data.validation_y, theta_hist[-1,:])
        
        # new ridge: using numpy optimize
        theta = regularized_gradient_descent_optimize.run(data.train_x, data.train_y, params)    
        loss_hinge = batch_gradient_descent.compute_loss(data.validation_x, data.validation_y, theta)
        
        #scikit's ridge
        #ridge_runner = Ridge(alpha=params.lambda_reg, max_iter=1000)
        #ridge_runner.fit(data.train_x, data.train_y)
        #loss_hinge = batch_gradient_descent.compute_loss(data.validation_x, data.validation_y, ridge_runner.coef_)
        
        losses.insert(-1, loss_hinge)
        #print "lambda: " +str(lambda_reg) +", loss_hinge: " +str(loss_hinge)
        if(loss_hinge<best_loss):
            best_loss = loss_hinge
            best_lambda = params.lambda_reg
            #best_theta = ridge_runner.coef_
            #best_theta = theta_hist[-1,:]
            best_theta = theta
    
    print "best loss_hinge: " +str(best_loss) +", for lambda: " +str(best_lambda)
    print "number of zeros in best theta: " +str((best_theta[best_theta==0]).size)
    print "number of weights in vicinity (10^-2) of zero: " +str((best_theta[np.abs(best_theta)<(10**-2)]).size)
    #print best_theta
    plt.subplot(122)
    plt.plot(best_theta)
    plt.subplot(121)
    plt.plot(np.asarray(losses),log_lambdas)
    plt.yticks(log_lambdas)
    plt.show()
        
# reading the data from csv
def main():
    #generateData()
    dataArray = np.loadtxt(csvFileName, delimiter=",")
    
    data = splitData(dataArray)
    print "range of y: " +str(data.train_y.max()-data.train_y.min())    
    
    #1.2
    #reg_params = regularized_gradient_descent.params(initial_bias=10, alpha=0.01, num_iter=1000, use_grad_checker=False)
    #data.train_x         = helpers.add_bias_term(data.train_x, reg_params.initial_bias)
    #data.test_x          = helpers.add_bias_term(data.test_x, reg_params.initial_bias)
    #data.validation_x    = helpers.add_bias_term(data.validation_x, reg_params.initial_bias)
    #doGridSearch(data, reg_params)
    
    #2.1, 2.2
    #print "no homotopy"
    #conjugate_grad_desent_lasso.doConjugateDescentLasso(data)
    
    #2.3, 
    print "with homotopy"
    conjugate_grad_desent_lasso_homotopy.doConjugateDescentLassoWithHomotopy(data)
    
if __name__ == "__main__":
    main()
