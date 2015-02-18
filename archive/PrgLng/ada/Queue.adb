with Gnat.io; use Gnat.io;
with Ada.Containers.Vectors; use Ada.Containers;

package body queue is
	package Queue_Container is new Vectors(Natural, Element_T);
	q: Queue_Container.Vector;
	
	function extract return Element_T is
	result: Element_T;
	begin
		result := q.First_Element;
		q.Delete_First;
		return result;
	end extract;
	
	function insert(x: Element_T) return Void is
	begin
		q.append(x);
		return;
	end insert;
end queue;
