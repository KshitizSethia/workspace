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
		if(last>first) then
			accept start(start: Integer; finish: Integer) do
				quicksort(input, start, finish, spacer &"   ");
			end start;
		end if;
	end sorter_low;
	
	task body sorter_high is
	begin
		if(last>first) then
			accept start(start: Integer; finish: Integer) do
				quicksort(input, start, finish, spacer &"   ");
			end start;
		end if;
	end sorter_high;
	
	procedure swap(n1: Integer; n2: Integer) is
	temp: Integer;
	begin
		temp:= input(n1);
		input(n1):= input(n2);
		input(n2):= temp;
	end swap;
	
	begin
		
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
		
		--call tasks in parallel to sort sub arrays
		sorter_low.start(first, store_index-1);
		sorter_high.start(store_index+1, last);
	end quicksort;
	
	function get_median(n1: Integer;n2: Integer;n3: Integer) return Integer is
	begin
		if((n1<=n2 and then n2<=n3)
			or else
			(n2<=n1 and then n3<=n2)) then
			return n2;
		elsif((n2<=n1 and then n1<=n3)
			or else
			(n3<=n1 and then n1<=n2)) then
			return n1;
		else
			return n3;
		end if;
	end get_median;
end algorithms;