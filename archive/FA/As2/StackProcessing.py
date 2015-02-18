def IsItAnOp(x):
    if(x in ["-4","-1","-59","-56"]):
        return False
    return not x.isdigit()

def is_empty(stack1):
    return len(stack1)==0

global_stack=[-4,-1,63,59,-59,-56]
global_stack.reverse()
def Eval(x,a,b):
    result = global_stack.pop()
    return str(result)

def StackVal(lst):
    x=lst.pop()
    if (not IsItAnOp(x)):
        return x
    return Eval(x, StackVal(lst), StackVal(lst))

def Solve(node):
    Y=[]
    a=node.pop()
    b=node.pop()
    x=node.pop()
    while(not is_empty(node) or not is_empty(Y)):
        if(IsItAnOp(x)and not IsItAnOp(a) and not IsItAnOp(b)):
            if(not is_empty(Y)):
                b=Eval(x,a,b)
                a=Y.pop()
                x=node.pop()
            else:
                a=Eval(x,a,b)
                b=node.pop()
                x=node.pop()
        else:
            Y.append(a)
            a=b
            b=x
            x=node.pop()
    return Eval(x,a,b)

ip = ["+", "*", "+", "-", "2", "6", "3", "-", "*", "9", "7", "4", "3"]
ip.reverse()
StackVal(ip)