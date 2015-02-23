import copy
import winsound
import multiprocessing 
from collections import Counter

import sparse_gradient_descent
import timeit

class ProcessSharedData:
    def __init__(self):
        self.workQueue = multiprocessing.Queue()

        mgr = multiprocessing.Manager()
        self.ns = mgr.Namespace()
        self.ns.best_result = float("inf")
        self.ns.best_weights = Counter()
        self.ns.best_lambda = 0
        self.ns.time_taken = 0

class Runner(multiprocessing.Process):
    def __init__(self\
                 , sharedData
                 , words_train\
                 , words_test\
                 , labels_train\
                 , labels_test):
        multiprocessing.Process.__init__(self)
        self.shared = sharedData
        self.words_train  = copy.deepcopy(words_train) 
        self.words_test   = copy.deepcopy(words_test)
        self.labels_train = copy.deepcopy(labels_train)
        self.labels_test  = copy.deepcopy(labels_test)
        
    def run(self):
        while(not self.shared.workQueue.empty()):
            reg = self.shared.workQueue.get()
            self.execute(reg)
    
    def execute(self, reg):
        strout = "regularization: " + str(reg)
        params = sparse_gradient_descent.params(lambda_reg=reg, max_epochs=10, checkGradient=False)
        
        start = timeit.default_timer()
        weights = sparse_gradient_descent.run(self.words_train, self.labels_train, params)
        taken = timeit.default_timer()-start
        self.shared.ns.time_taken = self.shared.ns.time_taken + taken

        strout += "\n" +str(sparse_gradient_descent.loss_01(self.words_train, self.labels_train, weights))
        test_loss = sparse_gradient_descent.loss_01(self.words_test, self.labels_test, weights)
        strout += "\n" +str(test_loss)
        print strout
        
        if(test_loss<self.shared.ns.best_result):
            self.shared.ns.best_result = test_loss
            self.shared.ns.best_weights = copy.deepcopy(weights)
            self.shared.ns.best_lambda = reg
        
        winsound.Beep(440, 400)
