class dl_node:
    def __init__(self,data, prv,nxt):
        self.data=data
        self.prv=prv
        self.nxt=nxt
        
    def __repr__(self):
        str_prev = "None"
        str_next="None"
        
        if(self.prv!=None):
            str_prev = self.prv.data
        if(self.nxt!=None):
            str_next = self.nxt.data 
        
        return "(" +str_prev +"<" +str(self.data) +">" +str_next +")"
        
    def __str__(self):
        str_prev = "None"
        str_next="None"
        
        if(self.prv!=None):
            str_prev = str(self.prv)
        if(self.nxt!=None):
            str_next = str(self.nxt) 
        
        return str_prev +"<-" +str(self.data) +"->" +str_next

    def insert_before(self, node):
        node.nxt=self
        node.prv=self.prv
        self.prv=node
        
class dl_list:
    def __init__(self, node):
        self.first=node
        self.last=node
    
    def __repr__(self):
        result = "f:" +self.first.data
        result +=", l:" +self.last.data
        result +=" v:"
        
        node = self.first
        while(node!=None):
            result+=node.data +","
            node=node.nxt
        
        return result
    
    def insert_before(self, node):
        self.first.insert_before(node)
        self.first=node

def IsItAnOp(x):
    if(x in ["-4","-1","-59","-56"]):
        return False
    return not x.isdigit()

def is_empty(stack1):
    return len(stack1)==0

global_stack=[63,59,-4,-1,-59,56]
global_stack.reverse()
def Eval(x,a,b):
    result = global_stack.pop()
    return str(result)

def StackVal(lst):
    x=lst.pop()
    if (not IsItAnOp(x)):
        return x
    return Eval(x, StackVal(lst), StackVal(lst))


def replace(data, prev_node, next_node):
    new_node = dl_node(data, prev_node, next_node)
    prev_node.nxt = new_node
    next_node.prv = new_node
    return new_node

def Solve(dl_list):

    next_node = dl_list.first
    start_node = next_node
    
    a=next_node.data
    next_node=next_node.nxt
    b=next_node.data
    next_node=next_node.nxt
    x=next_node.data
    next_node=next_node.nxt
    
    while(next_node!=None):
        if(IsItAnOp(x)and not IsItAnOp(a) and not IsItAnOp(b)):
            if(start_node.prv!=None):
                b=Eval(x,a,b)
                start_node = start_node.prv
                replace(b, start_node, next_node)
                a=start_node.data
                x=next_node.data
                next_node=next_node.nxt
            else:
                a=Eval(x,a,b)
                replace(a,start_node.prv,next_node)
                b=next_node.data
                next_node=next_node.nxt
                x=next_node.data
                next_node=next_node.nxt
        else:
            start_node = start_node.nxt
            a=b
            b=x
            x=next_node.data
            next_node = next_node.nxt
    return Eval(x,a,b)

sample_list= dl_list(dl_node("+",None,None))
sample_list.insert_before(dl_node("*",None,None))
sample_list.insert_before(dl_node("+",None,None))
sample_list.insert_before(dl_node("-",None,None))
sample_list.insert_before(dl_node("2",None,None))
sample_list.insert_before(dl_node("6",None,None))
sample_list.insert_before(dl_node("3",None,None))
sample_list.insert_before(dl_node("-",None,None))
sample_list.insert_before(dl_node("*",None,None))
sample_list.insert_before(dl_node("9",None,None))
sample_list.insert_before(dl_node("7",None,None))
sample_list.insert_before(dl_node("4",None,None))
sample_list.insert_before(dl_node("3",None,None))

print Solve(sample_list)
print sample_list