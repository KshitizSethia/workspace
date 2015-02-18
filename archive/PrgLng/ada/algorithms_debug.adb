with Gnat.io;
use Gnat.io;

package body algorithms is
	procedure quicksort(input: in out int_array; first: in Integer; last: in Integer; spacer: String) is
	
	pivot, pivot_index: Integer;
	store_index: Integer;
	
	task sorter_low is
		entry start(start: Integer; finish: Integer);
	end sorter_low;
	
	task sorter_high is
		entry start(start: Integer; finish: Integer);
	end sorter_high;
	
	task body sorter_low is
	begin
		 --Put(spacer &"Inside sorter_low");New_Line;
		if(last>first) then
			accept start(start: Integer; finish: Integer) do
				 --Put(spacer &"sorter_low started with ");Put(start);Put(", ");Put(finish);New_Line;
				quicksort(input, start, finish, spacer &"   ");
				 --Put(spacer &"sorter_low finished with ");Put(start);Put(", ");Put(finish);New_Line;
			end start;
		end if;
	end sorter_low;
	
	task body sorter_high is
	begin
		 --Put(spacer &"Inside sorter_high");New_Line;
		if(last>first) then
			accept start(start: Integer; finish: Integer) do
				 --Put(spacer &"sorter_high started with ");Put(start);Put(", ");Put(finish);New_Line;
				quicksort(input, start, finish, spacer &"   ");
				 --Put(spacer &"sorter_high finished with ");Put(start);Put(", ");Put(finish);New_Line;
			end start;
		end if;
	end sorter_high;
	
	procedure swap(n1: Integer; n2: Integer) is
	temp: Integer;
	begin
		--Put("      swapping ");Put(n1);Put(" and ");Put(n2);New_Line;
		temp:= input(n1);
		input(n1):= input(n2);
		input(n2):= temp;
	end swap;
	
	begin
		--Put(spacer &"QS called for ");Put(first);Put(" to ");Put(last);New_Line;
		-- If n=1, then return
		if(last<=first)
		then
			return;
		end if;

		-- Get pivot
		if(last-first=1)
		then
			pivot:=Integer'Min(input(first), input(last));
		else
			pivot:= get_median(input(first), input((first+last)/2), input(last));
		end if;
		
		-- get pivot index
		if (pivot = input(first)) then pivot_index:=first;
		elsif(pivot = input(last)) then pivot_index:= last;
		else pivot_index:= (first+last)/2;
		end if;			
		--Put("      pivot is ");Put(pivot);Put(", pivot_index is ");Put(pivot_index);New_Line;
		
		-- partition
		swap(pivot_index, last);
		store_index := first;
		for index in first..last-1 loop
			if(input(index)<pivot) then
				swap(index, store_index);
				store_index:= store_index+1;
			end if;
		end loop;
		swap(store_index, last);
		
		--Put("      pivot placed, array state is: ");
		--for index in first..last loop
		--	Put(input(index));Put(" ");
		--end loop;
		--New_Line; 
		
		--call tasks in parallel to sort sub arrays
		--Put("   Calling QS for ");Put("(");Put(first);Put(", ");Put(store_index-1);Put(") and (");
		--Put(store_index+1);Put(", ");Put(last);Put(")");New_Line;
		sorter_low.start(first, store_index-1);
		sorter_high.start(store_index+1, last);
		--quicksort(input, first, store_index-1, spacer &"   ");
		--quicksort(input, store_index+1, last, spacer &"   ");
		
	end quicksort;
	
	function get_median(n1: Integer;n2: Integer;n3: Integer) return Integer is
	begin
		--Put("      getting median for ");Put(n1);Put(",");Put(n2);Put(",");Put(n3);New_Line;
		if((n1<=n2 and then n2<=n3)
			or else
			(n2<=n1 and then n3<=n2)) then
			return n2;
		elsif((n2<=n1 and then n1<=n3)
			or else
			(n3<=n1 and then n1<=n2)) then
			return n1;
		else --if((n1<=n3 and then n3<=n2)
			 --or else
			 --(n2<=n3 and then n3<=n1)) then
			return n3;
		end if;
	end get_median;
end algorithms;