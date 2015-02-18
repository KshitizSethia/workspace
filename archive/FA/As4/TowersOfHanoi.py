"Problem 3.30"
class disk:
    def __init__(self, num):
        self.num = num
        self.color = "r"
    
    def __repr__(self):
        return "(" +str(self.num) +"," +self.color +")"
            
    def __str__(self):
        return "(" +str(self.num) +"," +self.color +")"    

def move(source_tower, destination_tower):
    disk_to_be_moved = source_tower.pop()
    
    "See if the move is legal"
    if( len(destination_tower)==0 or 
        disk_to_be_moved.num<destination_tower[-1].num):
        destination_tower.append(disk_to_be_moved)
    elif(disk_to_be_moved.color==destination_tower[-1].color):
        raise Exception("invalid move")

def toh(number_of_disks, source_tower, aux_tower, destination_tower):
    if(number_of_disks==1):
        move(source_tower, destination_tower)
    else:
        toh((number_of_disks-1), source_tower, destination_tower, aux_tower)
        move(source_tower, destination_tower)
        toh((number_of_disks-1), aux_tower, source_tower, destination_tower)

def push_disks(number_of_disks, tower):
    for num in range(number_of_disks, 0, -1):
        new_disk = disk(num)
        new_disk.color = "g"
        tower.append(new_disk)
                 
def pop_disks(number_of_disks, tower):
    for num in range(number_of_disks, 0, -1):
        tower.pop()
        
def magic_two_col_toh(number_of_disks, source_tower, aux_tower, destination_tower):
    if(number_of_disks==1):
        move(source_tower, destination_tower)
    else:
        magic_two_col_toh((number_of_disks-1), source_tower, destination_tower, aux_tower)
        if(source_tower[-1].color == "r"):
            # replace nth red disk with n-1 green disks in ascending order
            disk_storer = source_tower.pop()
            push_disks(number_of_disks-1, source_tower)
            #transport n-1 disks
            magic_two_col_toh(number_of_disks-1, source_tower, aux_tower, destination_tower)
            # replace n-1 green disks on destination_tower by nth red disk.
            pop_disks(number_of_disks-1, destination_tower)
            destination_tower.append(disk_storer)  
        else:
            move(source_tower, destination_tower)
        magic_two_col_toh((number_of_disks-1), aux_tower, source_tower, destination_tower)
        
a, b, c=[], [], []
for num in range(3, 0, -1):
    a.append(disk(num))
print a    
magic_two_col_toh(len(a), a, b, c)
print a

"""print "Problem 1.1 Example\n"
a=[5,4,3,2,1]
b,c = [], []
print a,b,c
toh(5,a,b,c)
print "Towers after toh"
print a,b,c
"""

"Problem 1.2"
"""
def is_it_there(disk_number, tower):
    return disk_number in tower

def disordered_toh(number_of_disks, source_tower, aux_tower, destination_tower):
    if(number_of_disks==0):
        return
    else:
        if(is_it_there(number_of_disks, source_tower)):
            disordered_toh(number_of_disks-1, source_tower, destination_tower, aux_tower)
            move(source_tower, destination_tower)
            if number_of_disks>1 : toh(number_of_disks-1, aux_tower, source_tower, destination_tower)  
        elif(is_it_there(number_of_disks, aux_tower)):
            disordered_toh(number_of_disks-1, destination_tower, aux_tower, source_tower)
            move(aux_tower, destination_tower)
            if number_of_disks>1 : toh(number_of_disks-1, source_tower, aux_tower, destination_tower)
        else: 
            "is_it_there(number_of_disks, destination_tower)"
            disordered_toh(number_of_disks-1, source_tower, aux_tower, destination_tower)


"""
"""print "Problem 1.2 example"
a=[5,3]
b=[4,1]
c=[2]
print a,b,c
disordered_toh(5, a, b, c)
print "Towers after disordered_toh"
print a,b,c
"""

"Problem 1.3"
"""
def move_to_small_tower(source_tower, small_tower):
    if(len(small_tower)<1):
        move(source_tower, small_tower)
    else:
        raise Exception("Limit reached on small tower")

def move_two_disks(source_tower, destination_tower, small_tower):
    move_to_small_tower(source_tower, small_tower)
    move(source_tower, destination_tower)
    move(small_tower, destination_tower)
        

def four_towers_of_hanoi(number_of_disks, source_tower, aux_tower, destination_tower, small_tower):
    if(number_of_disks==1):
        move(source_tower, destination_tower)
    elif(number_of_disks==0):
        return
    else:
        four_towers_of_hanoi((number_of_disks-2), source_tower, destination_tower, aux_tower, small_tower)
        move_two_disks(source_tower, destination_tower, small_tower)
        four_towers_of_hanoi((number_of_disks-2), aux_tower, source_tower, destination_tower, small_tower)

"""
"""print "Problem 1.3 example"
a=[5,4,3,2,1]
b=[]
c=[]
d=[]
print a,b,c,d
four_towers_of_hanoi(5, a, b, c, d)
print "4 Towers after toh"
print a,b,c,d
"""


"Problem 1.4"
"""
def flip_color(disk_number, colors_of_disks):
    colors_of_disks[disk_number-1] = not colors_of_disks[disk_number-1] 
"1.4a"
def colored_toh(number_of_disks, colors_of_disks, source_tower, aux_tower, destination_tower):
    if(number_of_disks==1):
        flip_color(source_tower[-1], colors_of_disks)
        move(source_tower, destination_tower)
    else:
        colored_toh((number_of_disks-1),colors_of_disks, source_tower, destination_tower, aux_tower)
        flip_color(source_tower[-1], colors_of_disks)
        move(source_tower, destination_tower)
        colored_toh((number_of_disks-1), colors_of_disks, aux_tower, source_tower, destination_tower)
"1.4b"
def colored_toh_no_color_change(number_of_disks, colors_of_disks, source_tower, aux_tower, destination_tower):
    colored_toh(number_of_disks, colors_of_disks, source_tower, destination_tower, aux_tower)
    colored_toh(number_of_disks, colors_of_disks, aux_tower, source_tower, destination_tower)

"1.4c"
def color_flipped_toh(number_of_disks, colors_of_disks, source_tower, aux_tower, destination_tower):
    if(number_of_disks==1):
        flip_color(source_tower[-1], colors_of_disks)
        move(source_tower, destination_tower)
    else:
        color_flipped_toh((number_of_disks-1),colors_of_disks, source_tower, aux_tower, destination_tower)
        color_flipped_toh((number_of_disks-1),colors_of_disks, destination_tower, source_tower, aux_tower)
        flip_color(source_tower[-1], colors_of_disks)
        move(source_tower, destination_tower)
        color_flipped_toh((number_of_disks-1), colors_of_disks, aux_tower, source_tower, destination_tower)
    
"""
"""print "Problem 1.4 example"
a=[5,4,3,2,1]
b=[]
c=[]
is_color_white=[False,False,False,False,False]
print a,b,c, is_color_white
"colored_toh(5, is_color_white, a, b, c)"
"colored_toh_no_color_change(5, is_color_white, a, b, c)"
color_flipped_toh(5, is_color_white, a, b, c)
print "towers after toh, and are disks white?"
print a,b,c,is_color_white"""


"Problem 1.5"
"""
def increment_tower(tower_num, increment):
    return (tower_num+increment)%3

def circular_move(towers, source, destination):
    if(destination == increment_tower(source,1)):
        "moving one tower to right"
        move(towers[source], towers[destination])
    elif(destination == increment_tower(source,2)):
        "moving two towers to the right"
        move(towers[source], towers[increment_tower(source,1)])
        move(towers[increment_tower(source,1)], towers[destination])
    else:
        raise Exception("Didn't expect this circular move!")

     
def circular_toh(number_of_disks, towers, source_tower, destination_tower):
    if(number_of_disks==1):
        circular_move(towers, source_tower, destination_tower)
    else:
        aux_tower = [0,1,2]
        aux_tower.remove(source_tower)
        aux_tower.remove(destination_tower)

        circular_toh((number_of_disks-1), towers, source_tower, aux_tower[-1])
        
        if(is_it_there(number_of_disks-1, towers[increment_tower(source_tower,1)])):
            circular_toh(number_of_disks-1, towers, 
                         increment_tower(source_tower,1), increment_tower(source_tower,2))
        circular_move(towers, source_tower, increment_tower(source_tower,1))
        circular_toh(number_of_disks-1, towers,
                     increment_tower(source_tower,2), source_tower)
        if(destination_tower == increment_tower(source_tower,2)):
            circular_move(towers, increment_tower(source_tower, 1), destination_tower)
        circular_toh(number_of_disks-1, towers, source_tower, destination_tower)
        

sample_towers=[[5,4,3,2,1],[],[]]
circular_toh(len(sample_towers[0]), sample_towers, 0, 1)
print sample_towers
"""