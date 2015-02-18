generic
	type Element_T is private;
package queue is
	Max: Positive;
	first: Integer;
	last: Integer;
	function extract() return Element_T;
	function insert(x: Element_T);
end queue