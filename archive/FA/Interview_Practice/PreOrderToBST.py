# https://onedrive.live.com/edit.aspx/Documents/Interview%20Questions?cid=7e955140f7174ff1&id=documents?&wd=target%28Practice.one%7c77D26279-ED93-4598-BF51-7AB3F7EEA1D3%2fPreorder%20to%20unique%20BST%7cAC43A266-E45E-4E33-87E2-BB735F520486%2f%29
length_of_problem = 11
PreOrder = [12,5,2,1,3,7,6,8,18,14,21,0]
index = 0

class Tree:
    def __init__(self, data):
        self.left = None
        self.right = None
        self.data = data
    def __repr__(self, level=0):
        ret = "\t"*level+repr(self.data)+"\n"
        if(self.left!=None): ret += self.left.__repr__(level+1)
        if(self.right!=None): ret += self.right.__repr__(level+1)
        return ret

def PrintInOrder(tree):
    if(tree.left != None): PrintInOrder(tree.left)
    print tree.data
    if(tree.right != None): PrintInOrder(tree.right)

# https://onedrive.live.com/edit.aspx/Documents/Interview%20Questions?cid=7e955140f7174ff1&id=documents?&wd=target%28Practice.one%7c77D26279-ED93-4598-BF51-7AB3F7EEA1D3%2fPreorder%20to%20unique%20BST%7cAC43A266-E45E-4E33-87E2-BB735F520486%2f%29
def MakeBSTFromPreOrder(tree):
    global index
    #if(PreOrder[index+1]<PreOrder[index]):
    if(index>=length_of_problem-1): 
        return
    index+=1
    tree.left = Tree(PreOrder[index])
    if(PreOrder[index+1]<PreOrder[index]): 
        MakeBSTFromPreOrder(tree.left)
    index+=1
    tree.right = Tree(PreOrder[index])
    if(PreOrder[index+1]<PreOrder[index]): 
        MakeBSTFromPreOrder(tree.right)

# https://onedrive.live.com/edit.aspx/Documents/Interview%20Questions?cid=7e955140f7174ff1&id=documents?&wd=target%28Practice.one%7c77D26279-ED93-4598-BF51-7AB3F7EEA1D3%2fPreorder%20to%20PostOrder%7c615800AB-3CB2-4E40-AD6C-115B35A73055%2f%29
def MakePostFromPreOrder(preListing, start, end):
    if(start>end): return
    
    left_subtree_end = start
    if(start<end):
        if(preListing[start+1] < preListing[start]):
            # left subtree present
            left_subtree_end+=1
            while(left_subtree_end<end 
                  and preListing[left_subtree_end+1]<preListing[start]):
                left_subtree_end+=1
            MakePostFromPreOrder(preListing, start+1, left_subtree_end)
        if(preListing[left_subtree_end+1]>preListing[start]):
            #right subtree present
            MakePostFromPreOrder(preListing, left_subtree_end+1, end)
    print(preListing[start])


myTree = Tree(PreOrder[index])
MakeBSTFromPreOrder(myTree)
PrintInOrder(myTree)
NewPreOrder = [12,5,2,1,3,7,6,8,18,14,21];
MakePostFromPreOrder(NewPreOrder, 0, len(NewPreOrder)-1)