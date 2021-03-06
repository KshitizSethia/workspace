import math;
import operator;

# algo parameters
ratio = 0.3
ratio_complement = 1.0-ratio

comparer = operator.gt#for maximization
            # operator.lt;# for minimization 
precision = 0.00001

limits = [0.0,22.0/7]

def calculate(x):
    return math.exp(x)*pow(math.sin(x),2);
def move_ahead():
    return limits[0]*ratio_complement + limits[1]*ratio;
def move_back():
    return limits[0]*ratio + limits[1]*ratio_complement

x1 = limits[0]
x2 = limits[1]

while (x2-x1>precision):
    fx1 = calculate(x1);
    fx2 = calculate(x2);
    
    if(comparer(fx1, fx2)):
        limits[1] = x2;
        x2 = move_back()
        if(x2<x1):
            temp = x1
            x1=x2
            x2=temp
    else:
        limits[0] = x1;
        x1 = move_ahead()
        if(x1>x2): 
            temp = x1
            x1=x2
            x2=temp
        
print (x1+x2)/2.0;