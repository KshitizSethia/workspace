Standard ML of New Jersey v110.74 [built: Tue Jan 31 16:19:10 2012]
fun less_than [] comparer search_val = []
 |  less_than (l::ls) comparer search_val = if (comparer (l,search_val)) then l::less_than ls comparer search_val else
less_than ls comparer search_val;
fun not_less_than [] comparer search_val = []
 |  not_less_than (l::ls) comparer search_val = if (comparer (l,search_val))
then not_less_than ls comparer search_val
else l::not_less_than ls comparer search_val;

fun partitionSort comparer [] = []
 |  partitionSort comparer (l::ls) = partitionSort comparer (less_than ls comparer l) @
l::partitionSort comparer (not_less_than ls comparer l);

partitionSort (fn(a,b) => length a < length b) [[1, 9, 3, 6], [1], [2,4,6], [5,5]];
val less_than = fn : 'a list -> ('a * 'b -> bool) -> 'b -> 'a list
val not_less_than = fn : 'a list -> ('a * 'b -> bool) -> 'b -> 'a list
val partitionSort = fn : ('a * 'a -> bool) -> 'a list -> 'a list
val it = [[1],[5,5],[2,4,6],[1,9,3,6]] : int list list
- datatype tree = 
	leaf of 'a list
	| node of leaf list;
stdIn:16.12-16.16 Error: unbound type constructor: leaf
stdIn:14.1-16.21 Error: unbound type variable in type declaration: 'a
- datatype tree = 
	leaf of 'a::[]
	| node of leaf::[];
stdIn:17.14-17.16 Error: syntax error: deleting  LBRACKET RBRACKET
- datatype tree = 
	leaf of 'a list
	| node of tree list;
stdIn:1.2-20.21 Error: unbound type variable in type declaration: 'a
- datatype tree = 
	leaf of list
	| node of tree list;
stdIn:21.10-21.14 Error: type constructor list given 0 arguments, wants 1
- datatype 'a' tree = 
	leaf of 'a list
	| node of tree list;
stdIn:24.12-24.16 Error: type constructor tree given 0 arguments, wants 1
stdIn:1.2-24.21 Error: unbound type variable in type declaration: 'a
- datatype 'a' tree = 
	leaf of 'a
	| node of tree list;
stdIn:26.12-26.16 Error: type constructor tree given 0 arguments, wants 1
stdIn:1.2-26.21 Error: unbound type variable in type declaration: 'a
- datatype 'a' tree = 
	leaf of 'a
	| node of 'a tree list;
stdIn:1.2-28.24 Error: unbound type variable in type declaration: 'a
- datatype 'a tree = 
	leaf of 'a
	| node of 'a tree list;
datatype 'a tree = leaf of 'a | node of 'a tree list
- datatype 'a tree = 
	leaf of 'a list
	| node of 'a tree list;
datatype 'a tree = leaf of 'a list | node of 'a tree list
- val myTree = node [node [node [leaf [4,2,14],leaf [9,83,32],leaf [96,123,4]],
node [leaf [47,71,82]],node [leaf [19,27,10],
leaf [111,77,22,66]],
leaf [120,42,16]],
leaf [83,13]]
;
val myTree = node [node [node #,node #,node #,leaf #],leaf [83,13]] : int tree
- fun sortTree comparer (leaf x) = partitionSort comparer x
=  |  sortTree comparer (node x::xs) = (sortTree comparer x) @ (sortTree comparer xs);
stdIn:40.5-41.84 Error: types of rules don't agree [tycon mismatch]
  earlier rule(s): ('Z * 'Z -> bool) * 'Z tree -> 'Z list
  this rule: ('Z * 'Z -> bool) * 'Y tree list -> 'X list
  in rule:
    (comparer,:: (node <pat>,xs)) =>
      (sortTree comparer) x @ (sortTree comparer) xs
stdIn:40.5-41.84 Error: right-hand-side of clause doesn't agree with function result type [tycon mismatch]
  expression:  'Z tree -> 'Z list
  result type:  'Y tree list -> 'X list
  in declaration:
    sortTree = (fn arg => (fn <pat> => <exp>))
- fun sortTree comparer (leaf x) = partitionSort comparer x
=  |  sortTree comparer (node (x::xs)) = (sortTree comparer x) @ (sortTree comparer xs);
stdIn:42.65-42.85 Error: operator and operand don't agree [tycon mismatch]
  operator domain: 'Z tree
  operand:         'Z tree list
  in expression:
    (sortTree comparer) xs
- fun sortTree comparer (leaf x) = leaf (partitionSort comparer x)
=  |  sortTree comparer (node (x::xs)) = node ((sortTree comparer x) @ (sortTree comparer xs));
stdIn:43.71-43.91 Error: operator and operand don't agree [tycon mismatch]
  operator domain: 'Z tree
  operand:         'Z tree list
  in expression:
    (sortTree comparer) xs
stdIn:1.6-43.93 Error: right-hand-side of clause doesn't agree with function result type [tycon mismatch]
  expression:  _ tree -> _ tree
  result type:  _ tree -> _ list
  in declaration:
    sortTree = (fn arg => (fn <pat> => <exp>))
- fun sortTree comparer (leaf x) = leaf (partitionSort comparer x);
stdIn:1.6-42.7 Warning: match nonexhaustive
          (comparer,leaf x) => ...
  
val sortTree = fn : ('a * 'a -> bool) -> 'a tree -> 'a tree
- fun sortTree comparer (leaf x) = leaf (partitionSort comparer x)