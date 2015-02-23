import multiprocessing

import Queue

class ThreadSharedData():
    def __init__(self):
        self.qu = multiprocessing.Queue()
        mgr = multiprocessing.Manager()
        self.ns = mgr.Namespace()
        self.ns.highestResult = 0.0

class runner(multiprocessing.Process):
    
    def __init__(self, threadNum, sharedData):
        multiprocessing.Process.__init__(self)
        self.threadNum = threadNum
        self.sharedData = sharedData
        
    def run(self):
        while(not self.sharedData.qu.empty()):
            print "pid: " +str(self.pid)
            
            val = self.sharedData.qu.get()

            result = 1
            while (val>0):
                result*=val
                val-=1
            print "result: " +str(result)
            print "highest yet: " +str(self.sharedData.ns.highestResult)
            if(self.sharedData.ns.highestResult<result):
                self.sharedData.ns.highestResult = result
