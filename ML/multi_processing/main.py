import runner

def main():
    sharedData = runner.ThreadSharedData()
    for val in range(500, 550):
        sharedData.qu.put(val)
    
    threads = []
    for index in range(3):
        threads.append(runner.runner(index+1, sharedData))
        threads[index].start()
    
    for thread in threads:
        thread.join()
        
    print sharedData.ns.highestResult
    print "bye!"

if __name__ == "__main__":
    main()