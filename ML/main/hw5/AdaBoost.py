from __future__ import division
import pandas as pd
from sklearn import tree
import math
from matplotlib import pyplot as plt
import numpy as np
import sklearn

def getWeightedError(predictions, actuals, weights):
    error = 0.0
    for index in range(len(predictions)):
        if predictions[index]!=actuals[index]:
            error = error + weights[index]
    error = error/sum(weights)
    return error

def getMisclassificationError(predictions, actuals):
    misclassification_idx = np.array(actuals) != np.array(predictions)
    misclassification_error = len(filter(lambda x: x==True, misclassification_idx))\
                                    /len(actuals)
    return misclassification_error

def revise_weights(predictions, actuals, weights, weight_multiplier):
    for index in range(len(predictions)):
        if predictions[index]!=actuals[index]:
            weights[index] = weights[index]*weight_multiplier
    return weights


def get_final_prediction(classifiers, weights_of_classifiers, inputs):

    def get_prediction(classifiers, weights_of_classifiers, input):
        prediction = 0.0
        for index in range(len(weights_of_classifiers)):
            temp = classifiers[index].predict(input)[0]
            prediction = prediction + temp*weights_of_classifiers[index]
        return (1,-1)[prediction<0]
    
    
    return list(map(lambda input: get_prediction(classifiers,\
                      weights_of_classifiers, input), inputs))

def plotDecisionSurface(predictor, X_train, Y_train, weights, title):
    
    #scale the weights
    scaled_weights = weights
    scaled_weights = np.square(scaled_weights)
    scaled_weights = map((lambda weight:\
                          (0.0+((weight-min(scaled_weights))/(max(scaled_weights)-min(scaled_weights))))*500.0)\
                         ,scaled_weights)
    # make a mesh in 2d plane and predict values
    # for whole mesh to plot the classification surface
    # copied from: http://scikit-learn.org/stable/auto_examples/tree/plot_iris.html#example-tree-plot-iris-py
    mesh_step = 0.02
    x_min, x_max = X_train[:, 0].min() - 1, X_train[:, 0].max() + 1
    y_min, y_max = X_train[:, 1].min() - 1, X_train[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, mesh_step), \
                         np.arange(y_min, y_max, mesh_step))

    mesh_predictions = predictor(np.c_[xx.ravel(), yy.ravel()])
    mesh_predictions = np.array(mesh_predictions).reshape(xx.shape)
    plt.contourf(xx, yy, mesh_predictions, \
                         cmap=plt.get_cmap("Paired"))
    
    # plot training points
    for idx in range(len(Y_train)):
        color = "g" if Y_train[idx] == 1 else "r"
        plt.scatter(X_train[idx, 0], X_train[idx, 1], c=color, label=str(Y_train[idx]),\
                    cmap=plt.get_cmap("Paired"), alpha=0.2, marker="o", s=scaled_weights[idx])
    
    plt.axis("tight")
    plt.title(title)
    plt.savefig("plots/"+title+".png")
    plt.clf()
    

def main():
    # load data
    dataFrame = pd.read_csv("data/banana_train.csv", delimiter=",").as_matrix()
    X_train = dataFrame[:, 1:]
    Y_train = dataFrame[:, 0]
    dataFrame = pd.read_csv("data/banana_test.csv", delimiter=",").as_matrix()
    X_test = dataFrame[:, 1:]
    Y_test = dataFrame[:, 0]
    
    max_depth = 3
    max_rounds = 10
    
    #initialize AdaBoost params
    num_train_examples = len(Y_train)
    weights = [1/num_train_examples]*num_train_examples
    classifiers = []
    weights_of_classifiers = []
    
    #record error
    hist_train_error =[]
    hist_test_error = []
    
    for round in range(1,max_rounds+1):
        classifier = tree.DecisionTreeClassifier(max_depth=max_depth)
        classifier = classifier.fit(X_train, Y_train, sample_weight=weights)
        
        predictions = classifier.predict(X_train)
        error = getWeightedError(predictions, Y_train, weights)
        weight_multiplier = (1-error)/error
        weight_of_classifier = math.log(weight_multiplier)
        
        weights = revise_weights(predictions, Y_train, weights, weight_multiplier)
        
        classifiers.append(classifier)
        weights_of_classifiers.append(weight_of_classifier)
        
        #training error till now
        predictions_till_now = get_final_prediction(classifiers, weights_of_classifiers, X_train)
        hist_train_error.append(getMisclassificationError(predictions_till_now, Y_train))
        
        #test error till now
        predictions_till_now_test = get_final_prediction(classifiers, weights_of_classifiers, X_test)
        hist_test_error.append(getMisclassificationError(predictions_till_now_test, Y_test))
        
        plotDecisionSurface((lambda inputs: get_final_prediction(classifiers, weights_of_classifiers, inputs))\
                            , X_train, Y_train,weights, title="AdaBoost round " +str(round))
    
    final_predictions = get_final_prediction(classifiers, weights_of_classifiers, X_train)
    plt.plot(hist_train_error, label="Training Error")
    plt.plot(hist_test_error, label="Test Error")
    plt.title("Misclassification error vs number of AdaBoost Rounds")
    plt.legend()
    plt.savefig("plots/AdaBoost_roundsProgress.png")
    plt.clf()
    
if __name__ == "__main__":
    main()