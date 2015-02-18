datatype 'a tree = 
leaf of 'a list
| node of 'a tree list;

fun sortTree comparer (leaf x) = leaf (partitionSort comparer x)
 |  sortTree comparer (node (children)) = node (map (fn (element) => sortTree comparer element) children);