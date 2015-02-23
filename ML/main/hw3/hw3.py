import cPickle
import time
import winsound
import Queue
from collections import Counter
import numpy as np

import pre_processor
import settings
import sparse_gradient_descent
import thread_runner
import matplotlib.pyplot as plt
import timeit
import copy

def calculate_best_weights_parallel(words_train, words_test, labels_train, labels_test):
    lambdas = [0.000001,0.00001,0.0001,0.001,0.01,0.1,1.0,10.0]
    
    sharedData = thread_runner.ProcessSharedData()
    
    for reg in lambdas:
        sharedData.workQueue.put(reg)
    
    threads = []
    threadCreationTime = 0
    start = timeit.default_timer()
    for index in range(3):
        threads.append(thread_runner.Runner(sharedData, words_train, words_test, labels_train, labels_test))
        threads[index].start()
    
    for thread in threads:
        thread.join()
    threadCreationTime += timeit.default_timer() - start
        
    print "avg time taken: " +str((threadCreationTime)/len(lambdas))
    
    for index in range(3):
        time.sleep(0.2)
        winsound.Beep(440, 400)
    
    print "best loss: " +str(sharedData.ns.best_result)\
            +" with lambda: " +str(sharedData.ns.best_lambda)
    cPickle.dump((sharedData.ns.best_weights)\
                 , open(settings.path_best_weights,"wb"), cPickle.HIGHEST_PROTOCOL)
    

def calculate_best_weights(words_train, words_test, labels_train, labels_test):
    
    best_result = float("inf")
    best_weights = Counter()
    best_lambda = 0
    time_taken = 0
    lambdas = [0.000001,0.00001,0.0001,0.001,0.01,0.1,1.0,10.0]
    
    for reg in lambdas:
        strout = "regularization: " + str(reg)
        params = sparse_gradient_descent.params(lambda_reg=reg, max_epochs=10, checkGradient=False)

        start = timeit.default_timer()
        weights = sparse_gradient_descent.run(words_train, labels_train, params)
        time_taken +=timeit.default_timer()-start
        
        strout += "\n" +str(sparse_gradient_descent.\
                            loss_01(words_train, labels_train, weights))
        test_loss = sparse_gradient_descent.loss_01(words_test, labels_test, weights)
        strout += "\n" +str(test_loss)
        print strout
        
        winsound.Beep(440, 400)
        
        if(test_loss<best_result):
            best_result = test_loss
            best_weights = copy.deepcopy(weights)
            best_lambda = reg
    print "avg time taken: " +str(time_taken/len(lambdas))
    
    print "best loss: " +str(best_result)\
            +" with lambda: " +str(best_lambda)
    cPickle.dump(best_weights\
                 , open(settings.path_best_weights,"wb"), cPickle.HIGHEST_PROTOCOL)
    
    for index in range(3):
        time.sleep(0.2)
        winsound.Beep(440, 400)
    
class reviewDetails:
    def __init__(self, words, label, score):
        self.words = words
        self.label = label
        self.score = score
    def __cmp__(self, other):
        return cmp(self.score, other.score)

def evaluateWorstMisclassifications(weights, labels, words_test, labels_test):
    class wordScores:
        def __init__(self, word, score):
            self.word = word
            self.score = score
        def __cmp__(self, other):
            return cmp(self.score, other.score)
        
    qu = Queue.PriorityQueue()
    for index in range(len(words_test)):
        score = sparse_gradient_descent.bag_dot(weights, words_test[index])\
                *labels_test[index]
        qu.put(reviewDetails(words_test[index], labels_test[index], score))
    
    #analysis of worst elements
    for index in range(5):
        element = qu.get()
        qu2 = Queue.PriorityQueue() 
        for word in list(element.words):
            contribution = element.words[word]*weights[word]*element.label
            #inverse_contribution = abs(1/contribution)\
            #                        if contribution!=0 else float("inf")
            qu2.put(wordScores(word, contribution))
        
        strout = ""
        for count in range(5):
            strout+=qu2.get().word +", "
        print strout
        
def visualizeConfidenceScore(weights, labels, words_test, labels_test):
    qu = Queue.PriorityQueue()
    for index in range(len(words_test)):
        score = abs(sparse_gradient_descent.bag_dot(weights, words_test[index]))
        qu.put(reviewDetails(words_test[index], labels_test[index], score))
    
    batch_size = 10
    numBatches = len(labels_test) / batch_size
    batch_losses = [0] * (numBatches)
    batch_scores = [0] * (numBatches)
    for batch_num in range(numBatches):
        batch = [Counter()] * batch_size
        labels = [0.0] * batch_size
        avg_score = 0
        for index in range(batch_size):
            temp = qu.get()
            batch[index] = temp.words
            labels[index] = temp.label
            avg_score += temp.score
        
        avg_score /= batch_size
        batch_losses[batch_num] = sparse_gradient_descent.loss_01(batch, labels, weights)
        batch_scores[batch_num] = avg_score
    
    plt.plot(batch_scores, batch_losses)
    plt.xlabel("Absolute value of confidence score")
    plt.ylabel("01 loss percentage")
    plt.show()


def weights_histogram(weights):
    plt.hist(weights.values(), bins=1000)
    plt.show()

def getWorstMisclassification(weights, labels, words_test, labels_test):
    qu = Queue.PriorityQueue()
    for index in range(len(words_test)):
        score = sparse_gradient_descent.bag_dot(weights, words_test[index])\
                *labels_test[index]
        qu.put(reviewDetails(words_test[index], labels_test[index], score))
    element = qu.get()
    return  element.words

def getBestClassification(weights, labels, words_test, labels_test):
    qu = Queue.PriorityQueue()
    for index in range(len(words_test)):
        score = sparse_gradient_descent.bag_dot(weights, words_test[index])\
                *labels_test[index]*-1.0
        qu.put(reviewDetails(words_test[index], labels_test[index], score))
    element = qu.get()
    return  element.words

def visualizeClassificationExtremes(weights, labels, words_test, labels_test):
    misclassified = getWorstMisclassification(weights, labels, words_test, labels_test)
    well_classified = getBestClassification(weights, labels, words_test, labels_test)
    
    common_words = set(misclassified.keys())\
                    & set(well_classified.keys())\
                    & set(weights.keys())
    good = []
    bad = []
    measure = []
    for word in list(common_words):
        good.append(well_classified[word])
        bad.append(misclassified[word])
        measure.append(weights[word])
        
    plt.plot(np.log(measure), label="log(computed weights)")
    plt.plot(good, label="tfidf for best prediction", color="green")
    plt.plot(bad, label="tfidf for worst prediction", color="red")
    plt.legend()
    plt.show()


def classification_histogram(words_test, labels_test, weights):
    classifications = []
    for index in range(len(words_test)):
        classifications.append(sparse_gradient_descent.bag_dot(weights, words_test[index]) * labels_test[index])
    
    plt.hist(classifications, 1000)
    plt.title("Histogram of Classification (y*w*x) for each test sample. centered around: " + str(sum(classifications) / len(classifications)))
    plt.xlabel("Value of classification, above zero is correct classification, below is wrong")
    plt.show()

def main():
    #use next stmt first time to generate data dump
    #do not do pre-processing when there's no:
    #new data, or change in pre-processing
    #pre_processor.PreProcessData()
    
    words = cPickle.load(open(settings.path_words, "rb"))
    labels = cPickle.load(open(settings.path_labels, "rb"))
    
    #2.1
    words_train, words_test = words[0:1500], words[1500:]
    labels_train, labels_test = labels[0:1500], labels[1500:]
    
    #--------Processing the data, generating a model--------
    calculate_best_weights_parallel(words_train, words_test, labels_train, labels_test)
    
    #--------post-processing, evaluating the model--------
    weights = cPickle.load(open(settings.path_best_weights,"rb"))
    #weights_histogram(weights)
    #visualizeConfidenceScore(weights, labels, words_test, labels_test)
    evaluateWorstMisclassifications(weights, labels, words_test, labels_test)
    #classification_histogram(words_test, labels_test, weights)
    #visualizeClassificationExtremes(weights, labels, words_test, labels_test)
    
if __name__ == "__main__":
    main()
