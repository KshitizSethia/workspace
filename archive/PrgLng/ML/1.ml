(*fun less_than [] search_val = []
 |  less_than (l::ls) search_val = if l<search_val then l::less_than ls search_val  else less_than ls search_val;
fun greater_than [] search_val = []
 |  greater_than (l::ls) search_val = if l>search_val then l::greater_than ls search_val else greater_than ls search_val;
 *)
 fun intPartition [] search_val = ([],[])
  |  intPartition (l::ls) search_val = if l>search_val
									then let val (smaller, larger) = (intPartition ls search_val)
										in (smaller,l::larger)
										end
									else let val (smaller, larger) = intPartition ls search_val
										in (l::smaller, larger)
										end;
 
(*fun intPartitionSort [] = []
 |  intPartitionSort (l:: ls) = intPartitionSort (less_than ls l) @ l::intPartitionSort (greater_than ls l);
*)
 fun intPartitionSort [] = []
 |  intPartitionSort (l:: ls) = let val (smaller, larger) = (intPartition ls l)
								in (intPartitionSort smaller) @ l::(intPartitionSort larger)
								end;
 
 intPartitionSort [4,7,2,4,1];