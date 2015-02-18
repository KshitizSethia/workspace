"Problem 1.7"
class SinglyLLNode:
    def __init__(self, data):
        self.data=data
        self.next=None

    def __repr__(self):
        if(self.next!=None):
            return str(self.data) +"-> " +str(self.next)
        else:
            return str(self.data) +"-> None"
            
    def __str__(self):
        if(self.next!=None):
            return str(self.data) +"-> " +str(self.next)
        else:
            return str(self.data) +"-> None"
        
class SinglyLL:
    def __init__(self):
        self.first_node=None
        self.last_node=None

    def __repr__(self):
        return str(self.first_node)

    def __str__(self):
        return str(self.first_node)
        
    def appendleft(self, data):
        new_node= SinglyLLNode(data)
        
        if(self.first_node==None):
            self.first_node=new_node
            self.last_node=new_node
        else:
            new_node.next=self.first_node
            self.first_node=new_node
            
    def popleft(self):
        result=self.first_node
        self.first_node=result.next
        return result
    
    def transfer_left(self, destination):
        if(self.first_node!=None):
            node = self.popleft()
            destination.appendleft(node.data)
    
    def reverse(self):
        result = SinglyLL()
        
        while(self.first_node!=None):
            self.transfer_left(result)
        self.first_node = result.first_node
        self.last_node = result.last_node
        
    def clear(self):
        self.first_node=None
        self.last_node=None
"1.7.0"
def do_sum_preceding(SinglyLL):
    node_ref = SinglyLL.first_node
    sum_holder=0
    
    while(node_ref!=None):
        sum_holder+=node_ref.data
        node_ref.data=sum_holder
        node_ref= node_ref.next
        
L = SinglyLL()
L.appendleft(1)
L.appendleft(2)
L.appendleft(3)
"do_sum_preceding(L)"

"1.7.i"
    
R=SinglyLL()
R.appendleft(4)
R.appendleft(5)
R.appendleft(6)
"L.transfer_left(R)"
"print R"

"1.7.ii"
""""L.reverse()
print L"""

"1.7.iiia"
def do_sum_following(input):
    input.reverse()
    do_sum_preceding(input)
    input.reverse()
    
"""do_sum_following(L)
print L"""

"1.7.iiic"
def do_sum_following_2pass(input):
    "fist pass"
    do_sum_preceding(input)
    
    "variables"
    sum_of_list=input.last_node.data
    previous_value_of_last_node=0
    
    "second pass"
    node_ptr=input.first_node
    #previous_value_of_last_node=node_ptr.data
    #node_ptr.data = sum_of_list
    while(node_ptr!=None):
        backup = node_ptr.data
        node_ptr.data = sum_of_list-previous_value_of_last_node
        previous_value_of_last_node=backup
        node_ptr = node_ptr.next
        
list1 = SinglyLL()
list1.appendleft(7)
list1.appendleft(3)
list1.appendleft(1)
do_sum_following_2pass(list1)

"1.7.iiid"
def do_recursive_sum_preceding(sum_of_nodes, node_ref):
    if(node_ref!=None):
        sum_of_nodes+=node_ref.data
        node_ref.data=sum_of_nodes
        do_recursive_sum_preceding(sum_of_nodes, node_ref.next)
"""print L
do_recursive_sum_preceding(0, L.first_node)
print L"""

"1.7.iiie"
def do_recursive_sum_following(node_ref):
    sum_of_nodes=0
    if(node_ref!=None):
        sum_of_nodes = node_ref.data + do_recursive_sum_following(node_ref.next)
        node_ref.data=sum_of_nodes
    return sum_of_nodes

"""print L
do_recursive_sum_following(L.first_node)
print L"""        

"1.7.iv"
def get_loop_length(linked_list):
    runner = linked_list.first_node
    fast_runner = linked_list.first_node
    loop_length=0
            
    while(fast_runner!=None and fast_runner.next!=None):
        runner=runner.next
        fast_runner = fast_runner.next.next
        if(runner==fast_runner):
            loop_length+=1
            runner=runner.next
            while(runner!=fast_runner):
                loop_length+=1
                runner=runner.next
            return loop_length
    return loop_length

L.appendleft(4)
L.last_node.next = L.first_node.next
print get_loop_length(L)
    