(*fun less_than [] comparer search_val = []
 |  less_than (l::ls) comparer search_val = if (comparer (l,search_val)) then l::less_than ls comparer search_val else
less_than ls comparer search_val;
fun not_less_than [] comparer search_val = []
 |  not_less_than (l::ls) comparer search_val = if (comparer (l,search_val))
then not_less_than ls comparer search_val
else l::not_less_than ls comparer search_val;

fun partitionSort comparer [] = []
 |  partitionSort comparer (l::ls) = partitionSort comparer (less_than ls comparer l) @
l::partitionSort comparer (not_less_than ls comparer l);
*)

fun partition [] comparer search_val = ([],[])
 |  partition (l::ls) comparer search_val = if (comparer (l,search_val))
									then let val (smaller, larger) = (partition ls comparer search_val)
										in (smaller,l::larger)
										end
									else let val (smaller, larger) = partition ls comparer search_val
										in (l::smaller, larger)
										end;
 
fun partitionSort comparer [] = []
 |  partitionSort comparer (l:: ls) = let val (smaller, larger) = (partition ls comparer l)
								in (partitionSort comparer smaller) @ l::(partitionSort comparer larger)
								end;

partitionSort (fn(a,b) => length a < length b) [[1, 9, 3, 6], [1], [2,4,6], [5,5]];