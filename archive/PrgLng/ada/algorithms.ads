package algorithms is
	type int_array is array(Positive range<>) of Integer;
	procedure quicksort(input: in out int_array; first: in Integer; last: in Integer; spacer: String);
private
	function get_median(n1: Integer;n2: Integer;n3: Integer) return Integer;
end algorithms;
