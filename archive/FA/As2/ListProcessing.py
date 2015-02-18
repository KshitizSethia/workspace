"Problem 1.7"
class ll_node:
    def __init__(self, data, nxt=None):
        self.data=data
        self.nxt=nxt

    def __repr__(self):
        if(self.nxt!=None):
            return str(self.data) +"-> " +str(self.nxt)
        else:
            return str(self.data) +"-> None"
            
    def __str__(self):
        if(self.nxt!=None):
            return str(self.data) +"->" +str(self.nxt)
        else:
            return str(self.data) +"->None"
        
def Clean(node):
    if(node==None):
        return node

    if(node.data==0):
        node=node.nxt
    
    if(node!=None):
        if(node.data!=0):
            node.nxt = Clean(node.nxt)
        else:
            return Clean(node)
    return node

"""def Clean(node):
    if(node==None or node.nxt==None):
        return node
    if(node.nxt.data==0):
        node.nxt=node.nxt.nxt
    Clean(node.nxt)
"""
        
#linked_list = ll_node(1,ll_node(2,ll_node(0,ll_node(0,ll_node(3,ll_node(0,ll_node(4,ll_node(0,None))))))))
linked_list = ll_node(1,ll_node(2,ll_node(0,ll_node(0,ll_node(0,ll_node(0,ll_node(3,ll_node(0,ll_node(4,ll_node(0,None))))))))))
print linked_list
Clean(linked_list)
print linked_list
