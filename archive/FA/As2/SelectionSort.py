#skeleton code copied from http://www.geekviewpoint.com/python/sorting/selectionsort to save time

def selectionsort_iter(n,aList):
    for index_top in range(n-1,0,-1):
        max_ptr = 0
        for test_dex in range( 1 , index_top+1):
            if aList[max_ptr] < aList[test_dex]:
                max_ptr = test_dex
     
        swap( aList, index_top, max_ptr )

"2.23.a"
def selectionsort_recur(n,aList):
    if(n==1): return
    max_ptr = 0
    for test_dex in range( 1 , n):
        if aList[max_ptr] < aList[test_dex]:
            max_ptr = test_dex
 
    swap( aList, n-1, max_ptr )
    selectionsort_recur(n-1, aList)
 
"2.23.b"
def selectionsort_recur_undo(n,aList):
    if(n==1):
        print "in last level of recur: "
        print aList
        return
    max_ptr = 0
    for test_dex in range( 1 , n):
        if aList[max_ptr] < aList[test_dex]:
            max_ptr = test_dex
 
    swap( aList, n-1, max_ptr )
    selectionsort_recur_undo(n-1, aList)
    swap(aList, n-1, max_ptr)

 
def swap( A, x, y ):
    tmp = A[x]
    A[x] = A[y]
    A[y] = tmp
  
ip=[18,5,3,1,19,6,0,7,4,2,5]
print ip
selectionsort_recur_undo(len(ip),ip)
print ip