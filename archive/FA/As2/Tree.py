#from collections import namedtuple

#n_node = namedtuple("n_node", "data,child,sibling")

class n_node:
    def __init__(self,data,child,sibling):
        self.data=data
        self.child=child
        self.sibling=sibling
    
    def __str__(self):
        return str(self.data)
    
    def __repr__(self):
        return str(self.data)

n_ary_tree= n_node(
                 1,
                 n_node(
                        2,
                        n_node(5,None,None),
                        n_node(
                               3,
                               None,
                               n_node(
                                      4,
                                      n_node(
                                             6,
                                             None,
                                             n_node(
                                                    7,
                                                    n_node(8,None,None),
                                                    None
                                                    )
                                         ),
                                      None
                                      )
                               )
                        ),
                 None
                )

"2.19.c2"
def ParentPre(v, pv):
    op="("
    op+=str(v.data) 
    op+=", " 
    op+="None" if (pv==None) else str(pv.data) 
    op+=")"
    print op
    child = v.child
    while(child!=None):
        ParentPre(child,v)
        child=child.sibling
        
#ParentPre(n_ary_tree, None)


"2.19.d2"
def ParentPost(v, pv):
    child = v.child
    while(child!=None):
        ParentPost(child,v)
        child=child.sibling
    op="("
    op+=str(v.data) 
    op+=", " 
    op+="None" if (pv==None) else str(pv.data) 
    op+=")"
    print op
    
ParentPost(n_ary_tree, None)

print "next"

def PrintPost(node):
    child1=node.child
    while(child1!=None):
        PrintPost(child1)

        op="("
        op+=str(child1.data) 
        op+=", " 
        op+=str(node.data) 
        op+=")"
        print op
        
        child1=child1.sibling
        
PrintPost(n_ary_tree)