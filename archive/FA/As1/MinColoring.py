"Problem 1.6"

def use_color_max_times(graph, color_assignments):
    "colored=set()"
    ready_to_color=set()
    try_next_time=set()
    
    "initialize all vertices in ready to color"
    counter=0
    for vertex in graph:
        ready_to_color.add(counter)
        counter+=1
    
    "initialize color counter"
    current_color=1
    
    "start coloring"
    while(True):
        
        "try coloring max vertices with this color"
        while(len(ready_to_color)>0):
            current_vertex=ready_to_color.pop()
            color_assignments[current_vertex]=current_color
            
            for connected_vertex in graph[current_vertex]:
                if(color_assignments[connected_vertex]==0):
                    ready_to_color.discard(connected_vertex)
                    try_next_time.add(connected_vertex)
                
        "done if all vertices are colored"
        if(len(try_next_time)==0):
            break
        
        "prepare for next color"
        current_color+=1
        for vertex in try_next_time:
            ready_to_color.add(vertex)
        try_next_time.clear()

def color_every_vertex(graph, colors):
    vertex_pointer=0
    
    for vertex_connections in graph:
        is_vertex_colored=False
        for color in colors:
            if(not is_vertex_colored
               and len(set(vertex_connections).intersection(color))==0):
                color.append(vertex_pointer)
                is_vertex_colored=True
        if(not is_vertex_colored):
            colors.append([vertex_pointer])
        vertex_pointer+=1
            
    
sample_graph=[[4,5,6,10],
              [5,6,7,8,10],
              [6,8,10,11],
              [],
              [0,7,8,9,10],
              [0,1,10],
              [0,1,2],
              [1,4,11],
              [1,2,4,10],
              [4],
              [0,1,2,4,5,8],
              [2,7],
              []]
"""sample_color_assignments = [0]*13
use_color_max_times(sample_graph, sample_color_assignments)
print sample_color_assignments"""

color_assignments=[[]]
color_every_vertex(sample_graph, color_assignments)
print color_assignments