from math import ceil
def U(n):
    print "U(" +str(n)+")\t\t called"
    if(n<=65):
        answer = 15*n
        print "U(" +str(n)+")\t\t returns " +str(answer)
        return answer
    else:
        x = float(9)/10
        x=x*n
        answer = n + U(x)
        print "U(" +str(n)+")\t returns " +str(answer)
        return answer
    
a = U(99)

def Z(n):
    print "Z(" +str(n)+")\t called"
    if(n<=65):
        answer = 15*n
        print "Z(" +str(n)+")\t returns " +str(answer)
        return answer
    else:
        x = float(9)/10
        x=x*n
        answer = n + Z(ceil(x))
        print "Z(" +str(n)+")\t returns " +str(answer)
        return answer
    
b = Z(99)

print a, b