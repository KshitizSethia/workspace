(*
ML Assignment
Submitted by Kshitiz Sethia (N13773746, ks3839)
C:\\Users\\Kshitiz\\Documents\\Cloud\\Copy\\workspace\\PrgLng\\ML\\ML_Assignment_Kshitiz_Sethia.ml
*)
Control.Print.printDepth := 100;
Control.Print.printLength := 100;

(* 1.
Implement a partition sort function, intPartitionSort, for a list of integers. A partition sort (similar to Quicksort, but not in-place), given a list (x::xs), should partition the elements of xs into two sublists: one list with numbers less than x and the other list with numbers not less than x. Then, the partition sort is recursively called on each of the two sublists and the Final result is constructed from appending the resulting lists, along with the list containing x, in the appropriate order. You can write it all as one function or break it up into several functions.

A use of intPartitionSort (in the SML/NJ system) would be:
- intPartitionSort [3,5,1,8,4];
val it = [1,3,4,5,8] : int list
*)
fun intPartition [] search_val = ([],[])
 |  intPartition (l::ls) search_val = if l>search_val
									then let val (smaller, larger) = (intPartition ls search_val)
										in (smaller,l::larger)
										end
									else let val (smaller, larger) = intPartition ls search_val
										in (l::smaller, larger)
										end;
 
fun intPartitionSort [] = []
 |  intPartitionSort (l:: ls) = let val (smaller, larger) = (intPartition ls l)
								in (intPartitionSort smaller) @ l::(intPartitionSort larger)
								end;
 
intPartitionSort [4,7,2,4,1];

(* 2.
Implement a polymorphic partition sort function, partitionSort, that will sort a list of elements of any type. In order to do this, though, partitionSort must take an additional parameter, the < (less-than) operator, that operates on the element types of the list. For
example, two uses of partitionSort would be:

(* Using the built-in < for comparing integers. The compiler is smart enough to figure out which < to use *)
- partitionSort (op <) [1,9, 3, 6, 7];
val it = [1,3,6,7,9] : int list
(* sorting a list of lists, where the less-than operator compares the length of two lists *)
- partitionSort (fn(a,b) => length a < length b) [[1, 9, 3, 6], [1], [2,4,6], [5,5]];
val it = [[1],[5,5],[2,4,6],[1,9,3,6]] : int list list
*)
fun partition [] comparer search_val = ([],[])
 |  partition (l::ls) comparer search_val = if (comparer (l,search_val))
									then let val (smaller, larger) = (partition ls comparer search_val)
										in (l::smaller,larger)
										end
									else let val (smaller, larger) = partition ls comparer search_val
										in (smaller, l::larger)
										end;
 
fun partitionSort comparer [] = []
 |  partitionSort comparer (l:: ls) = let val (smaller, larger) = (partition ls comparer l)
								in (partitionSort comparer smaller) @ l::(partitionSort comparer larger)
								end;

partitionSort (fn(a,b) => length a < length b) [[1, 9, 3, 6], [1], [2,4,6], [5,5]];

(* 3.
Define a polymorphic tree datatype (i.e. datatype 'a tree = ...) such that a leaf is labeled with an 'a and an interior node has a list of children, each of type 'a tree. That is, each interior node can have an arbitrary number of children, rather than just two (as in a binary tree). For example, your datatype declaration should allow the following tree to be constructed:

val myTree = node [node [node [leaf [4,2,14],leaf [9,83,32],leaf [96,123,4]],
node [leaf [47,71,82]],node [leaf [19,27,10],
leaf [111,77,22,66]],
leaf [120,42,16]],
leaf [83,13]];
*)
datatype 'a tree = 
	leaf of 'a list
	| node of 'a tree list;

val myTree = node [node [node [leaf [4,2,14],leaf [9,83,32],leaf [96,123,4]],
node [leaf [47,71,82]],node [leaf [19,27,10],
leaf [111,77,22,66]],
leaf [120,42,16]],
leaf [83,13]];
(* 4.
Define a polymorphic sortTree function that, given an 'b list tree (for some type 'b, so that each leaf has a list of elements of type 'b) returns a new tree that is identical to the original tree, except that the list at each leaf is sorted. It must use the polymorphic partition sort function that you wrote (above), so therefore must also take the < (less-than) operator as a parameter.

A use of sortTree is:
(* Here the parameter is an int list tree *)
- sortTree (op <) (node [leaf [4,2,3,1], leaf [7,2,5,0]]);
val it = node [leaf [1,2,3,4],leaf [0,2,5,7]] : int list tree
*)
fun sortTree comparer (leaf x) = leaf (partitionSort comparer x)
 |  sortTree comparer (node (children)) = node (map (fn (element) => sortTree comparer element) children);

(* 5.
Define a polymorphic merge function that takes two sorted lists and returns a sorted list
containing the elements of both lists. Since it is polymorphic, it also needs to take the <
operator as a parameter. A use of merge would be:
(* Sorting in decreasing order *)
- merge (fn (a,b) => a > b) [8,6,4,2] [7,5,3,1];
val it = [8,7,6,5,4,3,2,1] : int list
*)
fun merge comparer [] L2 = L2
 |  merge comparer L1 [] = L1
 |  merge comparer (e1::L1s) (e2::L2s) = if (comparer (e1,e2))
										then e1::merge comparer L1s (e2::L2s)
										else e2::merge comparer (e1::L1s) L2s;

(* 6.
Finally, define a polymorphic function mergeTree that, given an 'c list tree (for some type 'c) returns a sorted list (of type 'c list) of all the elements found at all the leaves of the tree. It should build on code that you wrote for the previous parts of this assignment and use both the sortTree and merge functions. For example, given the tree myTree that I defined above,

(* Again, myTree is an int list tree, so sortTree will return an int list *)
mergeTree (op <) myTree;
val it =
[2,4,4,9,10,13,14,16,19,22,27,32,42,47,66,71,77,82,83,83,96,111,120,123]
: int list
*)
(* Answer: version without using sortTree *)
(*
fun mergeTree comparer (leaf x) = partitionSort comparer x
 |  mergeTree comparer (node children) = (foldr (fn (x1, x2) => merge comparer x1 x2) [] (map (mergeTree comparer) children));
*)
(*Answer: version using sortTree*)
fun mergeTree comparer tree = 
			let val sortedTree = sortTree comparer tree
				fun mergeSortedTree comparer (leaf x) = partitionSort comparer x
				 |  mergeSortedTree comparer (node children) = (foldr (fn (x1, x2) => merge comparer x1 x2) [] (map (mergeTree comparer) children));
			in
				mergeSortedTree comparer sortedTree
			end;

(*my functions for debugging*) 
fun printList (x) = (foldr (fn (s1,s2) => s1 ^ "," ^s2) "]" (["["]@(map Int.toString x)) );