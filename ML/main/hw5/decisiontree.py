from __future__ import division
import pandas as pd
from sklearn import tree
import numpy as np
from matplotlib import pyplot as plt
import os
import shutil

def makeModel(max_depth):    
    # classify and predict
    classifier = tree.DecisionTreeClassifier(max_depth=max_depth)
    classifier = classifier.fit(X_train, Y_train)
        
    mesh_predictions = classifier.predict(np.c_[xx.ravel(), yy.ravel()])
    mesh_predictions = mesh_predictions.reshape(xx.shape)
    plt.contourf(xx, yy, mesh_predictions, \
                         cmap=plt.get_cmap("Paired"))
    
    # plot training points
    for idx in range(len(Y_train)):
        color = "g" if Y_train[idx] == 1 else "r"
        plt.scatter(X_train[idx, 0], X_train[idx, 1], c=color, label=str(Y_train[idx]), \
                    cmap=plt.get_cmap("Paired"), alpha=0.2)
    
    plt.axis("tight")
    plt.title("Tree of max depth " +str(max_depth))
    plt.savefig("plots/"+str(max_depth)+".png")
    plt.clf()
    
    #calculate train and test misclassification
    predicted_Y_train = classifier.predict(X_train)
    misclassification_idx_train = np.array(Y_train) != np.array(predicted_Y_train)
    misclassification_error_train = len(filter(lambda x: x==True, misclassification_idx_train))\
                                    /len(Y_train)
    
    predicted_Y_test = classifier.predict(X_test)
    misclassification_idx_test = np.array(Y_test) != np.array(predicted_Y_test)
    misclassification_error_test = len(filter(lambda x: x==True, misclassification_idx_test))\
                                    /len(Y_test)
    
    return misclassification_error_train, misclassification_error_test

def makeModel_leaves(max_leaves):    
    # classify and predict
    classifier = tree.DecisionTreeClassifier(max_leaf_nodes=max_leaves)
    classifier = classifier.fit(X_train, Y_train)
        
    mesh_predictions = classifier.predict(np.c_[xx.ravel(), yy.ravel()])
    mesh_predictions = mesh_predictions.reshape(xx.shape)
    plt.contourf(xx, yy, mesh_predictions, \
                         cmap=plt.get_cmap("Paired"))
    
    # plot training points
    for idx in range(len(Y_train)):
        color = "g" if Y_train[idx] == 1 else "r"
        plt.scatter(X_train[idx, 0], X_train[idx, 1], c=color, label=str(Y_train[idx]), \
                    cmap=plt.get_cmap("Paired"), alpha=0.2)
    
    plt.axis("tight")
    plt.title("Tree of max leaves " +str(max_leaves))
    plt.savefig("plots/max_leaves_"+str(max_leaves)+".png")
    plt.clf()
    
    #calculate train and test misclassification
    predicted_Y_train = classifier.predict(X_train)
    misclassification_idx_train = np.array(Y_train) != np.array(predicted_Y_train)
    misclassification_error_train = len(filter(lambda x: x==True, misclassification_idx_train))\
                                    /len(Y_train)
    
    predicted_Y_test = classifier.predict(X_test)
    misclassification_idx_test = np.array(Y_test) != np.array(predicted_Y_test)
    misclassification_error_test = len(filter(lambda x: x==True, misclassification_idx_test))\
                                    /len(Y_test)
    
    return misclassification_error_train, misclassification_error_test

def main():
    shutil.rmtree("plots", ignore_errors=True)
    os.makedirs("plots")
    
    test_errors=[]
    train_errors=[]
    
#    tree_depths = range(1, 21)
#    for depth in tree_depths:
#        #plt.subplots(int("25"+str(depth)))
#        train_error, test_error = makeModel(depth)
#        test_errors.append(test_error)
#        train_errors.append(train_error)    
#        print "done with depth " +str(depth)
#    
    maxLeaves = range(15, 80)
    for leaves in maxLeaves:
        train_error, test_error = makeModel_leaves(leaves)
        test_errors.append(test_error)
        train_errors.append(train_error)    
        print "done with leaves " +str(leaves)
    
    
    #plot test and train error with tree depth
    plt.plot(train_errors, label="training error")
    plt.plot(test_errors, label="test error")
    plt.xticks(range(len(maxLeaves)),maxLeaves)
    plt.grid()
    plt.legend()
    plt.savefig("plots/error_analysis.png")
    plt.clf()

# load data
dataFrame = pd.read_csv("data/banana_train.csv", delimiter=",").as_matrix()
X_train = dataFrame[:, 1:]
Y_train = dataFrame[:, 0]
dataFrame = pd.read_csv("data/banana_test.csv", delimiter=",").as_matrix()
X_test = dataFrame[:, 1:]
Y_test = dataFrame[:, 0]

# shuffle data
np.random.seed(13)
indexes = np.random.shuffle(np.arange(len(Y_train)))
X_train = (X_train[indexes])[0, :, :]
Y_train = (Y_train[indexes])[0, :]

# make a mesh in 2d plane and predict values
# for whole mesh to plot the classification surface
# copied from: http://scikit-learn.org/stable/auto_examples/tree/plot_iris.html#example-tree-plot-iris-py
mesh_step = 0.02
x_min, x_max = X_train[:, 0].min() - 1, X_train[:, 0].max() + 1
y_min, y_max = X_train[:, 1].min() - 1, X_train[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, mesh_step), \
                     np.arange(y_min, y_max, mesh_step))

if __name__ == "__main__":
    main()