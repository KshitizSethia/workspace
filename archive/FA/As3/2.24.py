#fix size of n
n=5

#initialize global result array
result = [0]*n
for index in range(0,n):
    result[index]=index+1
#debugging vars
count=0
print "initial: " +str(result)

def swap(a,b):
    temp = result[a]
    result[a] = result[b]
    result[b] = temp

def permute(n):
    global count
    if(n==0):
        print result;
        count+=1
    else:
        for index in range(n-1,-1,-1):
            swap(n-1, index)
            permute(n-1)
            swap(n-1, index)
permute(n)
print count