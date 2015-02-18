with Gnat.IO;
with algorithms;
use Gnat.io;

procedure hello is
	-- Declarations
	int_arr: algorithms.int_array(1..30);
	
	task printer is
		entry array_read;
		entry array_sorted;
		entry array_sum_ready(sum: in Integer);
	end printer;
	
	task sorter is
		entry unsorted_array_printed;
	end sorter;
	
	task adder is
		entry sorted_array_printed;
	end adder;
	
	-- Definitions
	
	-- task printer
	task body printer is
	sum: Integer:=0;
	begin
		accept array_read;
		for index in int_arr'Range loop
			Gnat.io.Put(int_arr(index));
			Gnat.io.Put(" ");
		end loop;	
		New_Line;
		
		sorter.unsorted_array_printed;
		accept array_sorted;
		for index in int_arr'Range loop
			Gnat.io.Put(int_arr(index));
			Gnat.io.Put(" ");
		end loop;	
		New_Line;
		
		adder.sorted_array_printed;
		accept array_sum_ready(sum: in Integer) do
			Put(sum);
		end array_sum_ready;
	end printer;
	
	-- task sorter
	task body sorter is
	begin
		accept unsorted_array_printed;
		algorithms.quicksort(int_arr, 1, int_arr'length, "");
		printer.array_sorted;
	end sorter;
	
	-- task adder
	task body adder is
	sum_1: Integer:=0;
	begin
		accept sorted_array_printed;
		
		for index in int_arr'Range loop
			sum_1 := sum_1 + int_arr(index);
		end loop;
		
		printer.array_sum_ready(sum => sum_1);
	end adder;
	
	-- contents of hello
begin
	for index in int_arr'Range loop
		Gnat.io.Get(int_arr(index));
	end loop;

	printer.array_read;
end Hello;
