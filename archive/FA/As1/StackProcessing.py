def is_empty(stack1):
    return len(stack1)==0

def IsItAnOp(x):
    if(x in ["-33","-0.303","5.697","-1","4.697"]):
        return False
    return not x.isdigit()

global_stack=[4.697,-1,5.697,-0.303,-33,35,7]
def Eval(x,a,b):
    result = global_stack.pop()
    return str(result)

def Solve(S, cost):
    Y=[]
    
    a=S.pop()
    cost[a]+=1
    b=S.pop()
    cost[b]+=1
    x=S.pop()
    cost[x]+=1
    
    while(not is_empty(S) or not is_empty(Y)):
        if(IsItAnOp(x)and not IsItAnOp(a) and not IsItAnOp(b)):
            if(not is_empty(Y)):
                b=Eval(x,a,b)
                cost[b]=1
                a=Y.pop()
                cost[a]+=1
                x=S.pop()
                cost[x]+=1
            else:
                a=Eval(x,a,b)
                cost[a]=1
                b=S.pop()
                cost[b]+=1
                x=S.pop()
                cost[x]+=1
        else:
             Y.append(a)
             cost[a]+=1
             a=b
             b=x
             x=S.pop()
             cost[x]+=1
             
    result = Eval(x,a,b)
    cost[result]=1
    return result
         
sample_stack = ["+","-", "8", "7",
                 "+", "6", "/",
                  "-", "*", "5",
                   "+", "4", "3",
                    "2", "1"]
print "stack length="  +str(len(sample_stack))
cost = {}
for entry in sample_stack:
    cost[entry]=0
    
print Solve(sample_stack, cost) 
print cost 
print "Cost=" +str(sum(cost.values()))
